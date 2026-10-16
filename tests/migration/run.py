"""运行迁移状态检查 直接编译源码中的类和方法"""
import argparse
from pathlib import Path
import re
import subprocess

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build' / 'migration-checks'
OUT.mkdir(parents=True, exist_ok=True)

def source(path):
    text = (ROOT / path).read_text(encoding='utf-8')
    return re.sub(r'^\s*#(?:include|pragma)[^\n]*', '', text, flags=re.M)

def run(cxx, name, text, flags=()):
    cpp = OUT / (name + '.cpp')
    exe = OUT / (name + '.exe')
    cpp.write_text(text, encoding='utf-8')
    subprocess.run([cxx, '-std=c++11', *flags, str(cpp), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
    print(name + ': PASS', flush=True)

def method(path, signature):
    text = (ROOT / path).read_text(encoding='utf-8')
    start = text.index(signature)
    brace = text.index('{', start)
    depth = 1
    end = brace + 1
    while depth:
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    return text[start:end]

def homing(cxx):
    path = 'src/telescope/mount/home/Home.cpp'
    methods = '\n'.join(method(path, s) for s in ['void Home::poll()', 'void Home::abort()', 'void Home::requestAborted()'])
    run(cxx, 'homing', '''
#include <cassert>
enum { HS_NONE, HS_HOMING, GU_NONE = 0, GS_NONE = 0, DIR_BOTH = 0, CE_NONE = 0 };
using CommandError = int;
unsigned long now = 0;
unsigned long millis() { return now; }
struct { bool trust = false; void requireHome() { trust = false; } bool trusted() { return trust; } } startupAuthority;
struct Axis {
  bool moving = false, target = true, sensed = false;
  bool motionErrorSensed(int) { return sensed; }
  bool atTarget() { return target; }
  bool gotoFailed() { return false; }
  void autoSlewAbort() { target = false; }
} axis1, axis2;
struct { int state = 0; void abort(bool = false) {} } guide, goTo;
struct {
  bool fault = false, syncFromOnStepToEncoders = false;
  bool motorFault() { return fault; }
  bool isSlewing() { return axis1.moving || axis2.moving; }
  void tracking(bool) {}
} mount;
struct Home {
  int state = HS_HOMING, resets = 0, resetResult = 0;
  bool failed = false, offsetActive = true, abortPending = false, isRequestWithReset = false;
  unsigned long offsetDeadline = 1000;
  void poll(); void abort(); void requestAborted();
  int reset(bool, bool authoritative) { ++resets; startupAuthority.trust = authoritative && !resetResult; return resetResult; }
};
''' + methods + '''
int main() {
  Home h;
  axis1.moving = true;
  h.poll();
  assert(h.state == HS_HOMING && h.resets == 0 && !startupAuthority.trust);
  axis1.moving = false;
  h.poll();
  assert(h.state == HS_NONE && h.resets == 1 && startupAuthority.trust);
  h = Home{}; startupAuthority.trust = false; axis2.target = false;
  h.poll();
  assert(h.failed && h.resets == 0 && !startupAuthority.trust);
  h = Home{}; axis1.target = axis2.target = true; axis1.moving = true;
  h.abort(); h.poll();
  assert(h.state == HS_HOMING && h.resets == 0);
  axis1.moving = false; h.poll();
  assert(h.failed && !startupAuthority.trust);
  h = Home{}; now = 1000; h.poll();
  assert(h.failed && h.resets == 0);
  h = Home{}; now = 0; axis1.target = axis2.target = true; h.resetResult = 1;
  h.poll(); assert(h.failed && !startupAuthority.trust);
}
''')

def authority(cxx):
    header = source('src/telescope/mount/startupAuthority/RecoveryPolicy.h') + source('src/telescope/mount/startupAuthority/StartupAuthority.h')
    body = source('src/telescope/mount/startupAuthority/StartupAuthority.cpp')
    run(cxx, 'authority', '''
#include <cassert>
#define MOUNT_PRESENT
#define ON 1
#define OFF 0
#define NV_INIT_ERROR_REVOKES_AUTHORITY ON
#define MOUNT_COORDS_MEMORY OFF
#define DLF(x)
enum CommandError { CE_NONE, CE_SLEW_ERR_UNSPECIFIED, CE_SLEW_ERR_IN_STANDBY, CE_SLEW_ERR_OUTSIDE_LIMITS };
struct { bool nv = false; } initError;
struct { bool on = false; void enabled(bool s) { on = s; } bool isEnabled() const { return on; } } limits;
struct { bool ready = true; bool isDateTimeReady() const { return ready; } } site;
struct { bool on = false; void tracking(bool s) { on = s; } } mount;
struct Motor { bool absolute = false; bool hasAbsoluteEncoder() const { return absolute; } } motor1, motor2;
struct Axis { Motor *motor; } axis1{&motor1}, axis2{&motor2};
struct { bool absoluteEncodersPresent = false; } goTo;
''' + header + body + '''
int main() {
  startupAuthority.begin();
  assert(!startupAuthority.trusted());
  startupAuthority.setTrusted(true);
  assert(startupAuthority.trusted() && limits.on);
  mount.on = true;
  startupAuthority.requireHome(true);
  assert(!startupAuthority.trusted() && startupAuthority.canReturnHome());
  assert(!limits.on && !mount.on);
  startupAuthority.setTrusted(true);
  assert(!startupAuthority.trusted() && !limits.on);
  assert(startupAuthority.validateManualMotion() == CE_NONE);
  startupAuthority.completeHome();
  assert(startupAuthority.trusted() && !startupAuthority.needsHome());
  startupAuthority.requireHome(false);
  assert(!startupAuthority.canReturnHome());
  startupAuthority.requireHome(true);
  assert(!startupAuthority.canReturnHome());
  startupAuthority.setTrusted(true);
  assert(!startupAuthority.canReturnHome());
  startupAuthority.completeHome();
  assert(startupAuthority.trusted());
  startupAuthority.requireHome(false, true);
  assert(startupAuthority.canRestorePark() && !startupAuthority.trusted());
  startupAuthority.requireHome();
  assert(!startupAuthority.canRestorePark());
  startupAuthority.requireHome(false, true);
  assert(!startupAuthority.canRestorePark());
  startupAuthority.completeHome();
  startupAuthority.requireHome(false, true);
  startupAuthority.setTrusted(false);
  assert(!startupAuthority.canRestorePark());
  startupAuthority.completeHome();
  startupAuthority.requireHome(true);
  initError.nv = true;
  startupAuthority.completeHome();
  assert(!startupAuthority.trusted() && startupAuthority.needsHome());
  initError.nv = false;
  startupAuthority.completeHome();
  assert(startupAuthority.trusted());
}
''')


def rates(cxx):
    path = 'src/telescope/mount/guide/Guide.cpp'
    methods = '\n'.join(method(path, sig) for sig in ['float Guide::homeRate(', 'float Guide::rateSelectToRate(', 'float Guide::limitGuideRate('])
    run(cxx, 'home_rates', '''
#include <cassert>
#include <cmath>
#include <cstdint>
#define ON 1
#define GOTO_FEATURE ON
#define AXIS2_SLEW_RATE_PERCENT 80
float degToRadF(float v) { return v*0.01745329252F; }
float radToDegF(float v) { return v/0.01745329252F; }
struct { float rate = degToRadF(4.0F); } goTo;
enum GuideRateSelect: uint8_t { GR_QUARTER, GR_HALF, GR_1X, GR_2X, GR_4X, GR_8X, GR_20X, GR_48X, GR_HALF_MAX, GR_MAX, GR_CUSTOM };
struct Guide {
 float customRateAxis1=0, customRateAxis2=0;
 float homeRate(uint8_t, uint8_t = 1);
 float rateSelectToRate(GuideRateSelect, uint8_t);
 float limitGuideRate(float);
} guide;
''' + methods + '''
int main() {
 assert(fabs(guide.homeRate(9)-goTo.rate)<1e-6);
 assert(fabs(guide.homeRate(8)-goTo.rate/2)<1e-6);
 assert(fabs(guide.homeRate(7)-degToRadF(0.2F))<1e-6);
 assert(fabs(guide.homeRate(7,2)-degToRadF(0.16F))<1e-6);
 goTo.rate = degToRadF(0.1F);
 assert(guide.homeRate(7)<=goTo.rate);
 assert(guide.homeRate(7,2)<=goTo.rate*0.8F);
}
''')


def physical_limits(cxx):
    path = 'src/lib/axis/Axis.cpp'
    methods = '\n'.join(method(path, sig) for sig in ['void Axis::updatePhysicalLimits()', 'bool Axis::physicalLimitReady()', 'bool Axis::physicalLimitBlocked('])
    run(cxx, 'physical_limits', '''
#include <cassert>
using ulong = unsigned long;
ulong now = 0; ulong millis() { return now; }
enum Direction { DIR_REVERSE=-1, DIR_BOTH=0, DIR_FORWARD=1 };
struct { bool pins[4] = {}; bool isOn(int i) { return pins[i]; } } sense;
struct Motor { float frequency = 0; Direction direction=DIR_FORWARD; float getFrequencySteps() { return frequency; } Direction getDirection() { return direction; } };
struct Axis {
 Motor storage; Motor *motor=&storage;
 int minSenseHandle=0, maxSenseHandle=1;
 struct { bool minLimitSensed=false, maxLimitSensed=false; } errors;
 bool commonMinMaxSense=false, limitLatched=false, limitReleasing=false, heldMinLimit=false, heldMaxLimit=false;
 ulong limitReleaseTime=0; Direction limitTripDirection=DIR_BOTH;
 void updatePhysicalLimits(); bool physicalLimitReady(); bool physicalLimitBlocked(Direction);
};
''' + methods + '''
int main() {
 Axis a, b; b.minSenseHandle=2; b.maxSenseHandle=3;
 a.commonMinMaxSense=true; a.motor->frequency=10;
 assert(a.physicalLimitReady());
 sense.pins[0]=true;
 assert(a.physicalLimitBlocked(DIR_FORWARD));
 assert(!a.physicalLimitBlocked(DIR_REVERSE));
 assert(a.physicalLimitBlocked(DIR_BOTH));
 assert(b.physicalLimitReady());
 a.motor->direction=DIR_REVERSE;
 assert(!a.physicalLimitBlocked(DIR_REVERSE));
 sense.pins[0]=false; now=100;
 assert(!a.physicalLimitReady()); now=599; assert(!a.physicalLimitReady());
 sense.pins[0]=true; a.updatePhysicalLimits(); sense.pins[0]=false;
 now=600; assert(!a.physicalLimitReady()); now=1100; assert(a.physicalLimitReady());
 a.motor->frequency=0; sense.pins[0]=true;
 assert(a.physicalLimitBlocked(DIR_FORWARD) && a.physicalLimitBlocked(DIR_REVERSE));
 sense.pins[2]=true;
 assert(b.physicalLimitBlocked(DIR_REVERSE) && !b.physicalLimitBlocked(DIR_FORWARD));
 sense.pins[3]=true; assert(b.physicalLimitBlocked(DIR_FORWARD));
}
''')


def goto_outcomes(cxx):
    path = 'src/telescope/mount/goto/Goto.cpp'
    methods = '\n'.join(method(path, sig) for sig in ['void Goto::abort(', 'void Goto::finish()'])
    methods += '\n' + method('src/telescope/mount/goto/Goto.h', 'void cancelAlignment()').replace('void cancelAlignment()', 'void Goto::cancelAlignment()')
    request = method(path, 'CommandError Goto::request(Coordinate')
    setup = request[request.index('trackingBeforeGoto ='):request.index('guide.backlashEnableControl')]
    run(cxx, 'goto_outcomes', '''
#include <cassert>
enum {GS_NONE, GS_GOTO, GG_NONE=0, GG_ABORT, GG_READY_ABORT, GG_DESTINATION=8, PS_PARKING=1, HS_HOMING=1};
struct { bool on=false; bool isTracking(){return on;} void tracking(bool v) { on=v; } void update() {} } mount;
struct { void setDurationComplete(int) {} } tasks;
struct { void soundAlert() {} } mountStatus;
struct Axis { int stops=0; void autoSlewAbort() {++stops;} void setFrequencyScale(float) {} } axis1,axis2;
struct Park { int state=0,done=0,failed=0; void requestDone(){++done;state=0;} void requestAborted(){++failed;state=0;mount.tracking(true);} } park;
struct Home { int state=0,done=0,failed=0; void requestDone(){++done;state=0;} void requestAborted(){++failed;state=0;mount.tracking(false);} } home;
struct Goto {
 int state=GS_GOTO,stage=GG_DESTINATION,taskHandle=1,pendingAlignStars=0;
 bool safetyAborted=false,trackingOnSuccess=true,trackingBeforeGoto=false,ordinaryGoto=true,firstGoto=true;
 struct { bool paused=false,resume=false; } meridianFlipHome;
 void abort(bool safety=false);void finish();void prepare();void cancelAlignment();
};
''' + methods + '\nvoid Goto::prepare() {\n' + setup + '\n}\n' + '''
int main() {
 Goto cancelled; cancelled.state=GS_NONE; cancelled.pendingAlignStars=2;
 cancelled.abort(); assert(cancelled.pendingAlignStars==0);
 cancelled.pendingAlignStars=2;cancelled.cancelAlignment();assert(cancelled.pendingAlignStars==0);
 Goto repeat; repeat.firstGoto=false; mount.on=false; repeat.prepare();
 assert(repeat.trackingOnSuccess && !mount.on); repeat.finish(); assert(mount.on);
 repeat=Goto(); repeat.firstGoto=false; mount.on=false; repeat.prepare();
 repeat.abort(); repeat.finish(); assert(!mount.on);
 park.state=PS_PARKING; repeat.prepare(); assert(!repeat.trackingOnSuccess); park.state=0;
 home.state=HS_HOMING; repeat.prepare(); assert(!repeat.trackingOnSuccess); home.state=0;
 Goto g; g.finish(); assert(mount.on && !g.firstGoto && g.state==GS_NONE);
 g=Goto();mount.on=false;g.abort();g.finish();assert(!mount.on && g.firstGoto);
 g=Goto();g.trackingBeforeGoto=true;g.abort();g.finish();assert(mount.on);
 g=Goto();g.trackingBeforeGoto=true;g.abort(true);g.abort();g.finish();assert(!mount.on);
 g=Goto();g.ordinaryGoto=false;park.state=PS_PARKING;g.abort(true);g.finish();assert(!mount.on && park.failed==1 && park.done==0);
 g=Goto();g.ordinaryGoto=false;home.state=HS_HOMING;g.abort(true);g.finish();assert(home.failed==1 && home.done==0);
 g=Goto();g.ordinaryGoto=false;home.state=HS_HOMING;g.finish();assert(home.done==1 && !mount.on);
}
''')


def st4_button(cxx):
    body = method('src/telescope/mount/st4/St4.cpp', 'bool St4::handleHomeButton(')
    run(cxx, 'st4_button', '''
#include <cassert>
enum { GS_NONE, HS_NONE=0, GU_NONE=0, CE_NONE=0 };
struct { int state=0; bool paused=false; bool isHomePaused(){return paused;} } goTo;
struct { bool moving=false; bool isSlewing(){return moving;} } mount;
struct { int state=0,stops=0; void stop(){++stops;} } guide;
struct { int state=0,requests=0; int request(){++requests;return CE_NONE;} } home;
struct { void soundBeep(){} } mountStatus;
struct St4 {
 bool homeButtonLatched=false,homeButtonPending=false;
 bool handleHomeButton(bool,bool,bool);
};
''' + body + '''
int main(){
 St4 s;
 assert(!s.handleHomeButton(true,false,false));assert(home.requests==0);
 mount.moving=true;guide.state=1;
 assert(s.handleHomeButton(true,true,false));assert(home.requests==0 && guide.stops==1);
 mount.moving=false;guide.state=GU_NONE;
 assert(s.handleHomeButton(true,true,false));assert(home.requests==1);
 s.handleHomeButton(true,true,false);assert(home.requests==1);
 home.state=1;s.handleHomeButton(false,false,true);assert(!s.homeButtonLatched);
 home.state=0;goTo.state=1;s.handleHomeButton(true,true,false);
 goTo.state=0;s.handleHomeButton(true,true,false);assert(home.requests==1);
 s.handleHomeButton(false,false,true);
 mount.moving=true;s.handleHomeButton(true,true,false);
 s.handleHomeButton(false,false,true);mount.moving=false;
 s.handleHomeButton(false,false,true);assert(home.requests==1);
 s.handleHomeButton(true,true,false);assert(home.requests==2);
}
''')


def home_offsets(cxx):
    body = method('src/telescope/mount/home/Home.cpp', 'void Home::guideDone(')
    for at_offsets in (0,1):
        run(cxx, 'home_offsets_' + str(at_offsets), '''
#include <cassert>
#define ON 1
#define OFF 0
#define AXIS1_SECTOR_GEAR OFF
#define AXIS2_TANGENT_ARM OFF
#define HOME_OFFSET_RATE 8
#define DF(x)
#define D(x)
#define DLF(x)
#define VF(x)
#define V(x)
#define VLF(x)
enum {CE_NONE, HS_NONE=0, HS_HOMING=1};
using CommandError=int;
unsigned long millis(){return 10;}
float arcsecToRad(float v){return v/1000;}
struct { bool trust=false; bool trusted(){return trust;} } startupAuthority;
struct { bool syncFromOnStepToEncoders=false; } mount;
struct { struct { struct { float sign=1; } latitude; } locationEx; } site;
struct { float homeRate(int, int=1){return 1;} } guide;
struct Axis {
 double position=0,target=0; int moves=0,error=0;
 double getInstrumentCoordinate(){return position;}
 void setTargetCoordinate(double v){target=v;}
 int autoGoto(float){++moves;return error;}
} axis1,axis2;
struct Home {
 int state=HS_HOMING,resets=0,resetError=0,aborts=0;
 bool offsetActive=false,failed=false,isRequestWithReset=false;
 unsigned long offsetDeadline=0;
 struct { struct { float senseOffset; } axis1{0},axis2{0}; } settings;
 bool useOffset(){return settings.axis1.senseOffset!=0||settings.axis2.senseOffset!=0;}
 int reset(bool, bool authoritative=false){++resets;startupAuthority.trust=authoritative && !resetError;return resetError;}
 void abort(){++aborts;startupAuthority.trust=false;}
 void requestAborted(){state=HS_NONE;failed=true;startupAuthority.trust=false;}
 void guideDone(bool);
};
''' + body + '''
int main(){
 Home h;h.guideDone(true);
 assert(startupAuthority.trust && !h.failed && axis1.moves==0 && axis2.moves==0);
 h=Home();h.settings.axis1.senseOffset=-1500;h.settings.axis2.senseOffset=1000;
 axis1.position=10;axis2.position=20;h.guideDone(true);
#if MOUNT_HOME_AT_OFFSETS == OFF
 assert(h.state==HS_HOMING && h.offsetActive && !startupAuthority.trust);
 assert(axis1.target==11.5 && axis2.target==19 && axis1.moves==1 && axis2.moves==1);
 h=Home();h.settings.axis1.senseOffset=1;axis2.error=1;h.guideDone(true);assert(h.aborts==1 && !startupAuthority.trust);
#else
 assert(h.state==HS_NONE && !h.offsetActive && startupAuthority.trust && axis1.moves==0);
#endif
 h=Home();h.guideDone(false);assert(h.failed && !startupAuthority.trust && h.resets==0);
 h=Home();h.resetError=1;h.guideDone(true);assert(h.failed && !startupAuthority.trust);
}
''', [f'-DMOUNT_HOME_AT_OFFSETS={at_offsets}'])


def offset_config(cxx):
    header = source('src/telescope/mount/home/Home.h')
    prefix = '''
#include <cassert>
#include <cstdint>
#define MOUNT_PRESENT
#define ON 1
#define OFF -1
#define LOW 0
#define HIGH 1
#define AXIS1_SECTOR_GEAR OFF
#define AXIS2_TANGENT_ARM OFF
#define AXIS1_SENSE_HOME LOW
#define AXIS2_SENSE_HOME HIGH
#define AXIS1_SENSE_HOME_OFFSET 123
#define AXIS2_SENSE_HOME_OFFSET 456
#define MOUNT_AUTO_HOME_DEFAULT OFF
using CommandError=int;
enum CoordReturn { CR_MOUNT_EQU };
struct Coordinate {};
'''
    for label, flags, a1, a2 in [('legacy', ['-DHOME_OFFSET_AXIS1=-1.5', '-DHOME_OFFSET_AXIS2=1'], -5400, -3600), ('native', [], 123, 456), ('zero', ['-DHOME_OFFSET_AXIS1=0', '-DHOME_OFFSET_AXIS2=0'], 0, 0)]:
        run(cxx, 'offset_config_' + label, prefix + header + f'''
int main() {{
 Home h;
 assert(h.settings.axis1.senseOffset == {a1});
 assert(h.settings.axis2.senseOffset == {a2});
}}
''', flags)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--cxx', default='g++')
    args = parser.parse_args()
    authority(args.cxx)
    rates(args.cxx)
    physical_limits(args.cxx)
    goto_outcomes(args.cxx)
    st4_button(args.cxx)
    home_offsets(args.cxx)
    offset_config(args.cxx)
    from recovery import check
    check(args.cxx)
    from addon_flasher import check as check_addon
    check_addon(args.cxx)
    from pec_storage import check as check_pec
    check_pec(args.cxx)
    from compatibility import startup, alignment, escape
    startup(args.cxx)
    alignment(args.cxx)
    escape(args.cxx)
    if 'void Home::poll()' in (ROOT / 'src/telescope/mount/home/Home.cpp').read_text(encoding='utf-8'):
        homing(args.cxx)
