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
  void autoSlewAbort() { target = false; }
} axis1, axis2;
struct { int state = 0; void abort() {} } guide, goTo;
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
    header = source('src/telescope/mount/startupAuthority/StartupAuthority.h')
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
  startupAuthority.completeHome();
  assert(startupAuthority.trusted());
  startupAuthority.requireHome(true);
  initError.nv = true;
  startupAuthority.completeHome();
  assert(!startupAuthority.trusted() && startupAuthority.needsHome());
  initError.nv = false;
  startupAuthority.completeHome();
  assert(startupAuthority.trusted());
}
''')

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--cxx', default='g++')
    args = parser.parse_args()
    authority(args.cxx)
    if 'void Home::poll()' in (ROOT / 'src/telescope/mount/home/Home.cpp').read_text(encoding='utf-8'):
        homing(args.cxx)
