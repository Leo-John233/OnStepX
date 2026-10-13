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
