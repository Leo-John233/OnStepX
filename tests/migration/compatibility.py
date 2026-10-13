"""验证新增策略与原生启动 校准 导星流程的组合"""
from run import method, run

def startup(cxx):
    body = method('src/telescope/mount/Mount.cpp', 'void Mount::autostartPostponed()')
    body += method('src/telescope/mount/Mount.cpp', 'void Mount::finishAutostart()')
    prefix = r'''
#include <cassert>
#define ON 1
#define OFF 0
#define GOTO_FEATURE ON
#define VLF(x)
#define DLF(x)
#define DF(x)
#define DL(x)
using CommandError=int;
enum {CE_NONE,HS_HOMING,PS_PARKED};
struct {bool ready=true;} telescope;
struct {bool nv=false;}initError;
struct {void setDurationComplete(int){} int getHandleByName(const char*){return 1;}}tasks;
struct {bool isEquatorial(){return true;}}transform;
struct {bool isDateTimeReady(){return true;}}site;
struct {bool locked=true;bool needsHome(){return locked;}void requireHome(){locked=true;}}startupAuthority;
bool enabled=true,tracking=false,fault=false;
struct {int state=PS_PARKED,calls=0,error=0;int restore(bool){++calls;enabled=false;return error;}}park;
struct {struct{bool automaticAtBoot=false;}settings;int calls=0,state=0,error=0;bool failed=false,hasSense=true;
 int request(){++calls;if(error)return error;state=HS_HOMING;park.state=0;return CE_NONE;}}home;
struct Mount {bool autostartComplete=false;void tracking(bool v){if(!startupAuthority.locked)::tracking=v;}
 bool motorFault(){return fault;}void enable(bool v){enabled=v;}void autostartPostponed();void finishAutostart();};
'''
    cases = {
        'hold': 'm.autostartPostponed();assert(park.calls==1&&enabled&&!tracking&&startupAuthority.locked);',
        'hold_off': 'm.autostartPostponed();assert(!enabled&&!tracking);',
        'sensor': 'home.settings.automaticAtBoot=true;park.error=5;m.autostartPostponed();assert(park.calls==0&&home.calls==1&&!m.autostartComplete);home.state=0;startupAuthority.locked=false;m.autostartPostponed();assert(m.autostartComplete&&enabled&&tracking);',
        'sensor_failed': 'home.settings.automaticAtBoot=true;home.error=5;home.failed=true;enabled=false;m.autostartPostponed();assert(home.calls==1&&m.autostartComplete&&!enabled&&!tracking);',
        'no_sensor': 'home.hasSense=false;home.settings.automaticAtBoot=true;park.error=5;m.autostartPostponed();assert(park.calls==1&&home.calls==0&&!tracking);',
        'fault': 'fault=true;enabled=false;m.finishAutostart();assert(!enabled&&!tracking);',
        'nv_fault': 'initError.nv=true;enabled=false;m.finishAutostart();assert(!enabled&&!tracking);',
    }
    for name, test in cases.items():
        flags = ['-DMOTOR_HOLD_ON_BOOT=' + ('OFF' if name == 'hold_off' else 'ON'), '-DTRACK_AUTOSTART=' + ('ON' if name == 'sensor' else 'OFF')]
        run(cxx, 'startup_' + name, prefix + body + '\nint main(){Mount m;' + test + '}', flags)

def alignment(cxx):
    body = method('src/telescope/mount/goto/Goto.cpp', 'CommandError Goto::requestAlignment(')
    body += method('src/telescope/mount/goto/Goto.cpp', 'CommandError Goto::pollAlignment()')
    prefix = r'''
#include <cassert>
#include <cstdint>
#define ON 1
#define OFF 0
#define GOTO_FEATURE ON
#define ALIGN_MAX_NUM_STARS 9
#define VLF(x)
using CommandError=int;
enum {CE_NONE,CE_PARAM_RANGE,CE_SLEW_IN_MOTION,CE_SLEW_ERR_IN_STANDBY,CE_SLEW_ERR_HARDWARE_FAULT,CE_SLEW_ERR_OUTSIDE_LIMITS,CE_SLEW_ERR_IN_PARK,CE_SLEW_ERR_UNSPECIFIED};
enum{GS_NONE,GU_NONE=0,HS_NONE=0,HS_HOMING=1,PS_PARKED=1};
struct{bool ready=true;bool isDateTimeReady(){return ready;}}site;
struct{bool trust=true;bool trusted(){return trust;}}startupAuthority;
struct{bool moving=false,fault=false,on=false;bool isSlewing(){return moving;}bool motorFault(){return fault;}void tracking(bool v){on=v;}bool isTracking(){return on;}}mount;
struct Axis{bool ready=true;bool physicalLimitReady(){return ready;}}axis1,axis2;
struct{int state=0;}guide,park;
struct{int state=0,error=0,calls=0;bool failed=false;
 int reset(){++calls;return error;}
 int requestWithReset(){++calls;if(!error){state=HS_HOMING;startupAuthority.trust=false;}return error;}}home;
struct Goto{int state=GS_NONE;uint8_t pendingAlignStars=0;struct{int currentStar=0,lastStar=0;}alignState;
 int requestAlignment(uint8_t);int pollAlignment();};
'''
    for auto in ('ON', 'OFF'):
        test = r'''
int main(){
 Goto g;
 home.error=CE_SLEW_ERR_OUTSIDE_LIMITS;assert(g.requestAlignment(2)==home.error);assert(!g.pendingAlignStars&&!g.alignState.currentStar&&!mount.on);
 home.error=0;axis1.ready=false;assert(g.requestAlignment(2)==CE_SLEW_ERR_OUTSIDE_LIMITS);axis1.ready=true;
 #if ALIGN_AUTO_HOME == ON
 assert(g.requestAlignment(2)==CE_NONE);assert(!g.alignState.currentStar&&!mount.on&&g.pendingAlignStars==2);
 assert(g.requestAlignment(3)==CE_SLEW_IN_MOTION);assert(g.pollAlignment()==CE_SLEW_IN_MOTION);
 home.state=HS_NONE;home.failed=true;assert(g.pollAlignment()!=CE_NONE);assert(!g.pendingAlignStars&&!mount.on);
 home.failed=false;assert(g.requestAlignment(3)==CE_NONE);home.state=HS_NONE;startupAuthority.trust=true;
 assert(g.pollAlignment()==CE_NONE);assert(g.alignState.currentStar==1&&g.alignState.lastStar==3&&mount.on);
 #else
 startupAuthority.trust=false;int before=home.calls;assert(g.requestAlignment(2)==CE_SLEW_ERR_IN_STANDBY);assert(home.calls==before);
 startupAuthority.trust=true;assert(g.requestAlignment(2)==CE_NONE);assert(g.alignState.currentStar==1&&g.alignState.lastStar==2&&mount.on);
 #endif
}
'''
        run(cxx, 'alignment_' + auto.lower(), prefix + body + test, ['-DALIGN_AUTO_HOME=' + auto])


def escape(cxx):
    path = 'src/telescope/mount/guide/Guide.cpp'
    body = '\n'.join(method(path, sig) for sig in ['CommandError Guide::validate(', 'CommandError Guide::startAxis1(', 'CommandError Guide::startAxis2('])
    prefix = r"""
#include <cassert>
#define ON 1
#define GOTO_FEATURE ON
#define AXIS2_SLEW_RATE_PERCENT 100
#define DLF(x)
#define VF(x)
#define V(x)
#define VL(x)
using CommandError=int;using GuideAction=int;using GuideRateSelect=int;
enum Direction{DIR_FORWARD,DIR_REVERSE,DIR_BOTH};
enum{CE_NONE,CE_SLEW_ERR_OUTSIDE_LIMITS,CE_SLEW_IN_MOTION,CE_SLEW_ERR_IN_STANDBY,CE_SLEW_ERR_IN_PARK,CE_SLEW_ERR_HARDWARE_FAULT};
enum{GA_NONE,GA_FORWARD,GA_REVERSE,GA_SPIRAL,GA_HOME,GR_CUSTOM=10,GR_MAX=9};
enum{GU_NONE,GU_HOME_GUIDE,GU_HOME_GUIDE_ABORT,GU_GUIDE,GU_PULSE_GUIDE,HS_HOMING=1,PS_PARKED=1,GS_NONE=0,PIER_SIDE_WEST=1,CR_MOUNT=0};
float degToRadF(float x){return x;}unsigned long millis(){return 0;}
struct{int state=0;}home,park;
struct{int state=0;void abort(){}}goTo;
struct{bool on=true,error=true;bool isEnabled(){return on;}bool isError(){return error;}}limits;
struct{bool locked=false;bool needsHome(){return locked;}int validateManualMotion(){return CE_NONE;}}startupAuthority;
struct Axis{bool ready=false;Direction allowed=DIR_REVERSE;int slews=0;bool physicalLimitReady(){return ready;}
 bool physicalLimitBlocked(Direction d){return !ready&&d!=allowed;}bool motionError(Direction){return !ready;}
 void setPowerDownOverrideTime(unsigned long){}void setFrequencySlew(float){}}axis1,axis2;
struct{int side=0;bool fault=false;float trackingRateAxis1=0,trackingRateAxis2=0;
 bool isEnabled(){return true;}void enable(bool){}bool isSlewing(){return false;}bool motorFault(){return fault;}
 struct Position{int pierSide;};Position getMountPosition(int){return {side};}void update(){}}mount;
struct{void wake(){}}mountStatus;
struct Guide{int state=GU_NONE,pierSide=0,guideActionAxis1=0,guideActionAxis2=0;float rateAxis1=0,rateAxis2=0;unsigned long guideFinishTimeAxis1=0,guideFinishTimeAxis2=0;
 struct{int axis1RateSelect=0,axis2RateSelect=0;}settings;
 bool validAxis1(int){return false;}bool validAxis2(int){pierSide=mount.side;return false;}
 int validate(int,int);int startAxis1(int,int,unsigned long,bool);int startAxis2(int,int,unsigned long,bool);
 float rateSelectToRate(int,int){return 1;}void finalizeTimeLimit(unsigned long&){}void backlashEnableControl(bool){}
 void axis1AutoSlew(int){++axis1.slews;}void axis2AutoSlew(int){++axis2.slews;}
};
"""
    run(cxx, 'independent_escape', prefix + body + r"""
int main(){
 Guide g;
 assert(g.startAxis1(GA_REVERSE,0,1000,false)==CE_NONE&&axis1.slews==1&&g.state==GU_GUIDE);
 assert(g.startAxis1(GA_FORWARD,0,1000,false)==CE_SLEW_ERR_OUTSIDE_LIMITS);
 assert(g.startAxis1(GA_REVERSE,0,1000,true)==CE_SLEW_ERR_OUTSIDE_LIMITS);
 assert(g.startAxis2(GA_REVERSE,0,1000,false)==CE_NONE&&axis2.slews==1&&g.state==GU_GUIDE);
 mount.side=PIER_SIDE_WEST;
 assert(g.startAxis2(GA_FORWARD,0,1000,false)==CE_NONE&&axis2.slews==2);
 assert(g.startAxis2(GA_REVERSE,0,1000,false)==CE_SLEW_ERR_OUTSIDE_LIMITS);
 axis1.allowed=DIR_BOTH;assert(g.startAxis1(GA_REVERSE,0,1000,false)==CE_SLEW_ERR_OUTSIDE_LIMITS);
 axis1.allowed=DIR_REVERSE;mount.fault=true;assert(g.startAxis1(GA_REVERSE,0,1000,false)==CE_SLEW_ERR_HARDWARE_FAULT);
 mount.fault=false;startupAuthority.locked=true;assert(g.startAxis1(GA_REVERSE,0,1000,false)==CE_NONE);
}
""")
