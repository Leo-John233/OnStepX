"""验证共用复位引脚的切换及释放"""
import argparse
from run import method, run, source


def check(cxx):
    lifecycle(cxx)
    admission(cxx)
    run(cxx, 'addon_passthrough_timeout', r'''
#include <cassert>
#include <cstdint>
#define VLF(x)
uint32_t now=0,origin=0;
uint32_t millis(){return now;}
void delayMicroseconds(int){}
int yields=0;
void yield(){++yields;}
bool traffic=false;
struct Port {
 bool host;int writes=0;
 Port(bool isHost):host(isHost){}
 bool available(){
  if(!host){now+=1000;assert(uint32_t(now-origin)<200000);return false;}
  uint32_t elapsed=now-origin;
  return traffic && elapsed>=10000 && elapsed<=120000;
 }
 int read(){return 42;}
 void write(int v){assert(v==42);++writes;}
 void flush(){}
} host{true},device{false};
#define SERIAL_A host
#define SERIAL_PASSTHROUGH device
struct AddonFlasher {
 int flashes=0,resumes=0;
 bool go(bool,void (*)()=nullptr);
 bool beginSession(){return true;}
 void endSession(){run(true);}
 void flash(){++flashes;}
 void run(bool restore){assert(restore);++resumes;}
};
''' + method('src/telescope/addonFlasher/AddonFlasher.cpp', 'bool AddonFlasher::go(') + r'''
int main(){
 for(int wrap=0;wrap<2;++wrap) for(int active=0;active<2;++active){
  now=origin=wrap?0xffff0000U:0U;traffic=active;device.writes=0;yields=0;
  AddonFlasher f;f.go(true);
  uint32_t elapsed=now-origin;
  assert(elapsed==(active?151000U:91000U));
  assert(device.writes==(active?111:0));
  assert(f.flashes==1&&f.resumes==1);
  assert(yields==int(elapsed/1000));
 }
}
''')
    prefix = r'''
#include <cassert>
#include <vector>
#define ADDON_RESET_PIN 2
enum {LOW=0,HIGH=1,OUTPUT=2,INPUT_PULLUP=3};
std::vector<int> events;
void delay(int ms){assert(ms==200);events.push_back(10);}
void pinModeEx(int pin,int mode){assert(pin==2);events.push_back(mode);}
void digitalWriteEx(int pin,int value){assert(pin==2);events.push_back(value);}
struct AddonFlasher {void reset(bool releaseShared = true);};
'''
    body = method('src/telescope/addonFlasher/AddonFlasher.cpp', 'void AddonFlasher::reset(')
    for shared in (False, True):
        expected = '10,2,0,10,1,10,3' if shared else '10,0,10,1,10'
        run(cxx, 'addon_reset_shared' if shared else 'addon_reset_dedicated',
            prefix + body + '\nint main(){AddonFlasher f;for(int i=0;i<2;++i){events.clear();f.reset();assert(events==std::vector<int>({' + expected + '}));}}',
            ['-DADDON_RESET_SHARED_INPUT'] if shared else [])

    run(cxx, 'addon_reset_owned_during_flash', prefix + body + r'''
int main(){
 AddonFlasher f;
 f.reset(false);
 assert(events==std::vector<int>({10,2,0,10,1,10}));
 events.clear();f.reset(true);
 assert(events==std::vector<int>({10,2,0,10,1,10,3}));
}
''', ['-DADDON_RESET_SHARED_INPUT'])

    pinmap = source('src/pinmaps/Pins.MaxESP4.h')
    base = '#define ESP32\n#define OFF -1\n#define ON 1\n#define SERIAL_A_BAUD_DEFAULT OFF\n#define SERIAL_B_BAUD_DEFAULT OFF\n'
    run(cxx, 'maxesp4_spi_addon', base + '#define STEP_DIR_TMC_SPI_PRESENT\n' + pinmap + r'''
#ifndef ADDON_RESET_SHARED_INPUT
#error Missing shared reset handling
#endif
static_assert(ADDON_GPIO0_PIN==26,"ESP07 GPIO0");
static_assert(ADDON_RESET_PIN==2,"ESP07 RST jumper");
static_assert(SHARED_ENABLE_PIN==4,"Motor enable unchanged");
int main(){}
''')
    run(cxx, 'maxesp4_uart_addon', base + '#define STEP_DIR_TMC_UART_PRESENT\n' + pinmap + r'''
#if defined(ADDON_RESET_PIN) || defined(ADDON_GPIO0_PIN) || defined(ADDON_RESET_SHARED_INPUT)
#error SPI board addon wiring must not apply to UART board
#endif
int main(){}
''')
    run(cxx, 'maxesp4_custom_addon', base + '#define STEP_DIR_TMC_SPI_PRESENT\n#define ADDON_RESET_PIN 32\n#define ADDON_GPIO0_PIN 33\n' + pinmap + r'''
#ifdef ADDON_RESET_SHARED_INPUT
#error Dedicated reset must not use SPI sharing
#endif
static_assert(ADDON_RESET_PIN==32 && ADDON_GPIO0_PIN==33,"Preserve custom pins");
int main(){}
''')


def lifecycle(cxx):
    # 直接编译完整烧录模块 验证双向字节和退出后的引脚恢复
    run(cxx, 'addon_full_lifecycle', r'''
#include <cassert>
#include <cstdint>
#include <vector>
#define OFF -1
#define ON 1
#define HardSerial -9
#define SERIAL_PASSTHROUGH device
#define SERIAL_A host
#define SERIAL_PASSTHROUGH_BAUD_DEFAULT 9600
#define SERIAL_A_BAUD_DEFAULT 9600
#define SERIAL_PASSTHROUGH_RX 16
#define SERIAL_PASSTHROUGH_TX 17
#define SERIAL_8N1 0
#define ADDON_RESET_SHARED_INPUT
#define ADDON_RESET_PIN 2
#define ADDON_GPIO0_PIN 26
#define ADDON_TRIGR_PIN OFF
#define VF(x)
#define V(x)
#define VL(x)
#define VLF(x)
enum {LOW=0,HIGH=1,OUTPUT=2,INPUT_PULLUP=3};
uint32_t now=0;
int modes[40]={},levels[40]={},yields=0;
bool checking=false;
uint32_t millis(){return now;}
void delay(int ms){now+=ms;}
void delayMicroseconds(int){}
void pinModeEx(int p,int m){modes[p]=m;}
void digitalWriteEx(int p,int v){levels[p]=v;}
int digitalReadEx(int p){return levels[p];}
void yield(){
 ++yields;now+=1000;
 if(checking){assert(modes[2]==OUTPUT&&levels[2]==HIGH&&levels[26]==LOW);}
}
struct Port {
 int baud=9600;std::vector<int> input,output;
 void begin(int b){baud=b;}
 void begin(int b,int,int rx,int tx){assert(rx==16&&tx==17);baud=b;}
 bool available(){return !input.empty();}
 int read(){int v=input.front();input.erase(input.begin());return v;}
 void write(int v){output.push_back(v);}
 void flush(){}
} host,device;
''' + source('src/telescope/addonFlasher/AddonFlasher.h') +
        source('src/telescope/addonFlasher/AddonFlasher.cpp') + r'''
int main(){
 static bool allowed=false;
 addonFlasher.init([](){assert(!addonFlasher.canFlash());return allowed;});
 assert(modes[2]==INPUT_PULLUP);
 levels[26]=LOW;
 uint32_t before=now;
 assert(!addonFlasher.go(true));
 assert(now==before&&levels[26]==LOW&&modes[2]==INPUT_PULLUP);
 assert(host.baud==9600&&device.baud==9600);
 assert(addonFlasher.canFlash());
 allowed=true;
 for(int saved=LOW;saved<=HIGH;++saved){
  levels[26]=saved;host.input={0,255,192,219};device.input={192,0,255,219};
  host.output.clear();device.output.clear();checking=true;
  static bool notified=false;notified=false;
  assert(addonFlasher.go(true, [](){
   assert(!addonFlasher.canFlash());
   assert(host.baud==9600&&device.baud==9600);
   notified=true;
  }));checking=false;
  assert(notified);
  assert(host.output==std::vector<int>({192,0,255,219}));
  assert(device.output==std::vector<int>({0,255,192,219}));
  assert(host.baud==9600&&device.baud==9600);
  assert(modes[2]==INPUT_PULLUP&&levels[26]==saved);
  assert(addonFlasher.canFlash());
 }
 assert(yields>0);
}
''')


def admission(cxx):
    run(cxx, 'addon_mount_stop_before_flash', r'''
#include <cassert>
#include <cstdint>
#define MOUNT_PRESENT
#define ADDON_RESET_SHARED_INPUT
#define STEP_DIR_TMC_SPI_PRESENT
#define AXIS1_M2_PIN 23
#define AXIS2_M2_PIN 5
#define HIGH 1
enum {HS_NONE=0,GS_NONE=0,GU_NONE=0};
uint32_t now=0;
uint32_t millis(){return now;}
void delay(int ms){now+=ms;}
bool stuck=false;
int releases=0,polls=0;
struct Mount {
 bool trackingOn=true,slewing=true;
 bool isTracking(){return trackingOn;}
 bool isSlewing(){return slewing;}
 void tracking(bool on){assert(!on);trackingOn=on;}
} mount;
struct State {
 int state=1,aborts=0;
 void abort(){++aborts;}
 void abort(bool safety){assert(safety);++aborts;}
 void poll(){++polls;}
} home,goTo,guide;
struct Axis {
 float rate=50;int aborts=0;
 float getFrequencySteps(){return rate;}
 void autoSlewAbort(){++aborts;}
} axis1,axis2;
struct Tasks {
 void yield(){
  assert(home.aborts&&goTo.aborts&&guide.aborts);
  assert(axis1.aborts&&axis2.aborts&&!mount.trackingOn);
  if(!stuck){
   mount.slewing=false;
   home.state=goTo.state=guide.state=0;
   axis1.rate=axis2.rate=0;
  }
 }
} tasks;
void digitalWriteEx(int p,int v){
 assert((p==23||p==5)&&v==HIGH);
 assert(!mount.isTracking()&&!mount.isSlewing());
 assert(home.state==0&&goTo.state==0&&guide.state==0);
 assert(axis1.rate==0&&axis2.rate==0);
 ++releases;
}
''' + method('src/telescope/Telescope.cpp', 'bool addonFlashPrepare(') + r'''
int main(){
 assert(addonFlashPrepare());
 assert(now==1&&polls==1&&releases==2);
 assert(!mount.trackingOn);
 for(int wrap=0;wrap<2;++wrap){
  stuck=true;axis2.rate=1;releases=0;
  now=wrap?0xfffffff0U:0;
  uint32_t start=now;
  assert(!addonFlashPrepare());
  assert(uint32_t(now-start)==30000U&&releases==0);
 }
}
''')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--cxx', default='g++')
    check(parser.parse_args().cxx)
