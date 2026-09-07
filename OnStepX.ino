/*
 * 标题        OnStepX
 * 作者        Howard Dutton
 *
 * 版权所有 (C) 2021-2026 Howard Dutton
 *
 * 本程序是自由软件 您可以依据自由软件基金会发布的 GNU 通用公共许可证
 * 第 3 版或您选择的任何后续版本重新发布或修改本程序
 *
 * 发布本程序是希望它能够发挥作用
 * 但不提供任何担保 包括对适销性或特定用途适用性的默示担保
 * 更多详情请查看 GNU 通用公共许可证
 *
 * 您应已随本程序收到一份 GNU 通用公共许可证副本
 * 如果没有 请查看 <http://www.gnu.org/licenses/>
 *
 * 功能说明
 *   功能完整的望远镜控制系统 支持
 *   赤道仪架台和地平式架台
 *   旋转器
 *   调焦器
 *   附件 自动镜盖和除露加热器等
 *
 * 作者 Howard Dutton
 *   http://www.stellarjourney.com
 *   hjd1964@gmail.com
 *
 * 修订历史和新版本
 *   请查看 GitHub https://github.com/hjd1964/OnStep
 *
 * 文档
 *   https://groups.io/g/onstep/wiki/home
 *
 * 讨论和问题等
 *   https://groups.io/g/onstep
 */

// 使用 "Config..." 标签页按需配置 OnStep

// 固件版本 -------------------------------------------------------------------------------------------------------------------------
#define FirmwareName                "On-Step"
#define FirmwareVersionMajor        10
#define FirmwareVersionMinor        28     // 次版本号范围为 00 至 99
#define FirmwareVersionPatch        "w"    // 主版本号和次版本号及补丁版本示例 10.03c
#define FirmwareVersionConfig       6      // 内部版本号 用于跟踪配置文件变更

#include "src/Common.h"
#include "src/Validate.h"
#include "src/lib/nv/Nv.h"
#include "src/lib/analog/Analog.h"
#include "src/lib/sense/Sense.h"
#include "src/lib/tasks/OnTask.h"

#include "src/telescope/Telescope.h"
extern Telescope telescope;

#include "src/plugins/Plugins.config.h"

#if DEBUG == PROFILER
  extern void profiler();
#endif

void sensesPoll() {
  sense.poll();
}

void setup() {
  #if ADDON_SELECT_PIN != OFF
    pinMode(ADDON_SELECT_PIN, OUTPUT);
    digitalWrite(ADDON_SELECT_PIN, HIGH);
  #endif

  #if DEBUG != OFF
    SERIAL_DEBUG.begin(SERIAL_DEBUG_BAUD);
    delay(2000);
  #endif

  // 执行引脚映射所需的特殊处理
  #ifdef PIN_INIT
    PIN_INIT();
  #endif

  // 输出启动信息
  VLF("");
  VF("MSG: OnStepX, version "); V(FirmwareVersionMajor); V("."); V(FirmwareVersionMinor); VL(FirmwareVersionPatch);
  VF("MSG: OnStepX, MCU "); VLF(MCU_STR);
  VF("MSG: OnStepX, pinmap "); VLF(PINMAP_STR);

  // 初始化底层硬件
  VLF("MSG: System, HAL initialize");
  HAL_INIT();
  WIRE_INIT();

  analog.begin();

  nv().setGate(&xBusy);
  if (!nv().init()) {
    DLF("ERR: Setup, NV (EEPROM/FRAM/Flash/etc.) device not found!");
  }
  delay(2000);

  #if defined(NV_WIPE) && NV_WIPE == ON
    nv().wipe();
  #endif

  // 启动输入感应轮询任务
  int pollingRate = round((1000.0F/HAL_FRACTIONAL_SEC)/2.0F);
  if (pollingRate < 1) pollingRate = 1;
  VF("MSG: System, start input sense service task (rate "); V(pollingRate); VF("ms priority 7)... ");
  if (tasks.add(pollingRate, 0, true, 7, sensesPoll, "SysSens")) { VLF("success"); } else { VLF("FAILED!"); }

  // 初始化望远镜对象
  telescope.init(FirmwareName, FirmwareVersionMajor, FirmwareVersionMinor, FirmwareVersionPatch, FirmwareVersionConfig);

  // 启动命令通道任务
  commandChannelInit();

  tasks.yield(2000);

  // 启动所有已启用的插件
  #if PLUGIN1 != OFF
    PLUGIN1.init();
  #endif
  #if PLUGIN2 != OFF
    PLUGIN2.init();
  #endif
  #if PLUGIN3 != OFF
    PLUGIN3.init();
  #endif
  #if PLUGIN4 != OFF
    PLUGIN4.init();
  #endif
  #if PLUGIN5 != OFF
    PLUGIN5.init();
  #endif
  #if PLUGIN6 != OFF
    PLUGIN6.init();
  #endif
  #if PLUGIN7 != OFF
    PLUGIN7.init();
  #endif
  #if PLUGIN8 != OFF
    PLUGIN8.init();
  #endif

  // 启动任务管理器调试事件
  #if DEBUG == PROFILER
    tasks.add(142, 0, true, 7, profiler, "Profilr");
  #endif

  sense.poll();

  telescope.ready = true;
}

void loop() {
  tasks.yield();
}
