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
