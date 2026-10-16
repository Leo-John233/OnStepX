"""复现 PEC 容量错误并验证当前配置关闭 PEC"""
import argparse
import re
from run import ROOT, method, run


def check(cxx):
    config = (ROOT / 'Config.h').read_text(encoding='utf-8')
    names = ('AXIS1_STEPS_PER_DEGREE', 'PEC_STEPS_PER_WORM_ROTATION', 'PEC_BUFFER_SIZE_LIMIT')
    defines = '\n'.join(re.search(r'^#define\s+' + name + r'\s+[^/\n]+', config, re.M).group(0) for name in names)
    # 编译原生开关与启动调用 确保关闭后不读取旧 PEC 参数
    header = (ROOT / 'src/telescope/mount/pec/Pec.h').read_text(encoding='utf-8')
    switch = re.search(r'#if PEC_BUFFER_SIZE_LIMIT.*?#endif', header, re.S).group(0)
    mount = (ROOT / 'src/telescope/mount/Mount.cpp').read_text(encoding='utf-8')
    init_call = re.search(r'#if AXIS1_PEC == ON\s+pec.init\(\);\s+#endif', mount).group(0)
    run(cxx, 'pec_disabled_config', '#define OFF -1\n#define ON -2\n' + defines + '\n' + switch + r'''
#if AXIS1_PEC != OFF
#error PEC must remain disabled in this configuration
#endif
int main(){
''' + init_call + '\n}\n')
    # 保留迁移时的参数 复现容量不足及足够容量的区别
    defines = re.sub(r'(#define\s+PEC_STEPS_PER_WORM_ROTATION)\s+[^\n]+', r'\g<1> 51200', defines)
    run(cxx, 'pec_storage_migration', defines + r'''
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#define DLF(x)
#define VLF(x)
#define VF(x)
#define V(x)
#define OFF 0
#define ON 1
#define PEC_SENSE OFF
#define GOTO_FEATURE ON
#define RAD_DEG_RATIO 57.29577951308232
#define SIDEREAL_RATIO 1.00273790935
#define constrain(x,lo,hi) ((x)<(lo)?(lo):((x)>(hi)?(hi):(x)))
enum PecState {PEC_NONE, PEC_READY_PLAY, PEC_PLAY, PEC_READY_RECORD, PEC_RECORD};
struct Settings {bool recorded=false; PecState state=PEC_NONE; long wormRotationSteps=PEC_STEPS_PER_WORM_ROTATION;};
struct {bool value=false;} initError;
struct NvVolume {bool isMounted(){return true;}};
struct Kv {int computeKey(const char*){return 1;} bool getOrInit(int,Settings&){return true;}};
struct Nv {NvVolume v; Kv k; NvVolume& volume(){return v;} Kv& kv(){return k;}} storage;
Nv& nv(){return storage;}
struct Partition {
 int capacity=0;
 bool init(NvVolume&,const char*){return true;}
 int sizeBytes(){return capacity;}
 void readBytes(int,void* p,int n){assert(n<=capacity);memset(p,0,n);}
 void writeBytes(int,const void*,int n){assert(n<=capacity);}
};
struct {double getStepsPerMeasure(){return AXIS1_STEPS_PER_DEGREE*RAD_DEG_RATIO;}} axis1;
struct {struct {int wormSensePositionSteps=0;} settings;} park;
void pecWrapper(){}
struct {int add(int,int,bool,int,void(*)(),const char*){return 1;}} tasks;
struct Pec {
 void init();
 int nvKey=0,monitorHandle=0;
 Settings settings;
 Partition nvIv;
 double stepsPerSiderealSecond=0,stepsPerMicroSecond=0;
 long stepsPerSiderealSecondI=0,wormRotationSeconds=0,bufferSize=0;
 int8_t* buffer=nullptr;
 ~Pec(){free(buffer);}
};
''' + method('src/telescope/mount/pec/Pec.cpp', 'void Pec::init()') + r'''
int main(){
 const long required=lround(PEC_STEPS_PER_WORM_ROTATION/(AXIS1_STEPS_PER_DEGREE/240.0));
 assert(required==864);
 {Pec p;p.nvIv.capacity=720;p.init();assert(initError.value && p.bufferSize==0);}
 initError.value=false;
 {Pec p;p.nvIv.capacity=864;p.init();
  assert(!initError.value && p.bufferSize==required && p.monitorHandle!=0);}
 // 只换固件不会改变已保存的分区大小
 initError.value=false;
 {Pec p;p.nvIv.capacity=720;p.init();assert(initError.value);}
}
''')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--cxx', default='g++')
    check(parser.parse_args().cxx)
