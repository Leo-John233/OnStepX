"""验证共用复位引脚的切换及释放"""
import argparse
from run import method, run, source


def check(cxx):
    run(cxx, 'addon_passthrough_timeout', r'''
#include <cassert>
#include <cstdint>
#define VLF(x)
uint32_t now=0,origin=0;
uint32_t millis(){return now;}
void delayMicroseconds(int){}
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
} host{true},device{false};
#define SERIAL_A host
#define SERIAL_PASSTHROUGH device
struct AddonFlasher {
 int flashes=0,resumes=0;
 void go(bool);
 void flash(){++flashes;}
 void run(bool restore){assert(restore);++resumes;}
};
''' + method('src/telescope/addonFlasher/AddonFlasher.cpp', 'void AddonFlasher::go(') + r'''
int main(){
 for(int wrap=0;wrap<2;++wrap) for(int active=0;active<2;++active){
  now=origin=wrap?0xffff0000U:0U;traffic=active;device.writes=0;
  AddonFlasher f;f.go(true);
  uint32_t elapsed=now-origin;
  assert(elapsed==(active?151000U:91000U));
  assert(device.writes==(active?111:0));
  assert(f.flashes==1&&f.resumes==1);
 }
}
''')
    prefix = r'''
#include <cassert>
#include <vector>
#define ADDON_RESET_PIN 2
enum {LOW=0,HIGH=1,OUTPUT=2,INPUT_PULLUP=3};
std::vector<int> events;
struct {void yield(int ms){assert(ms==200);events.push_back(10);}} tasks;
void pinModeEx(int pin,int mode){assert(pin==2);events.push_back(mode);}
void digitalWriteEx(int pin,int value){assert(pin==2);events.push_back(value);}
struct AddonFlasher {void reset();};
'''
    body = method('src/telescope/addonFlasher/AddonFlasher.cpp', 'void AddonFlasher::reset()')
    for shared in (False, True):
        expected = '10,2,0,10,1,10,3' if shared else '10,0,10,1,10'
        run(cxx, 'addon_reset_shared' if shared else 'addon_reset_dedicated',
            prefix + body + '\nint main(){AddonFlasher f;for(int i=0;i<2;++i){events.clear();f.reset();assert(events==std::vector<int>({' + expected + '}));}}',
            ['-DADDON_RESET_SHARED_INPUT'] if shared else [])

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


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--cxx', default='g++')
    check(parser.parse_args().cxx)
