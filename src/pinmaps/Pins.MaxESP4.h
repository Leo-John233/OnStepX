// -------------------------------------------------------------------------------------------------
// OnStep MaxESP4 引脚映射 ESP32S
#pragma once

#if defined(ESP32)

// Serial0 RX 引脚 GPIO3 TX 引脚 GPIO1 连接 USB 串口适配器
// Serial1 RX1 引脚 GPIO10 TX1 引脚 GPIO9 位于 SPI Flash 引脚上 使用前必须改接
// Serial2 RX2 引脚 GPIO16 TX2 引脚 GPIO17

#if SERIAL_A_BAUD_DEFAULT != OFF
  #define SERIAL_A              Serial
#endif
#if SERIAL_B_BAUD_DEFAULT != OFF
  #define SERIAL_B              Serial2
  #define SERIAL_B_RX           16
  #define SERIAL_B_TX           17
#endif

// 驱动模式拨码
// UART / TMC2209  1=ON  2=OFF 3=OFF 4=ON  5=OFF
// SPI  / TMC5160  1=OFF 2=ON  3=ON  4=OFF 5=ON
// 可切换 IO GPIO13=M0/SPI MOSI GPIO14=M1/SPI SCK GPIO23=TMC UART TX/Axis1 SPI CS GPIO5=Axis2 SPI CS GPIO2=TMC UART RX/SPI MISO

// 以下设置适用于所有可能存在的 TMC UART 驱动器 TMC2209
#if defined(STEP_DIR_TMC_UART_PRESENT) || defined(SERVO_TMC2209_PRESENT)
  #define SERIAL_TMC_HARDWARE_UART
  #define SERIAL_TMC            Serial1          // 使用一个硬件串口连接最多四个驱动器
  #define SERIAL_TMC_BAUD       460800           // 波特率
  #define SERIAL_TMC_RX         2                // 可切换 IO UART 接收数据 SPI 模式为 MISO
  #ifndef SERIAL_TMC_TX
    #define SERIAL_TMC_TX       23               // 可切换 IO UART 发送数据 SPI 模式为 Axis1 CS
  #endif
  #define SERIAL_TMC_ADDRESS_MAP(x) ((x==4)?2 : x) // Axis1(0) 为 0 Axis2(1) 为 1 Axis3(2) 为 2 Axis4(3) 为 3 Axis5(4) 为 2
#endif

// 使用 ESP32 默认 I2C 引脚 GPIO21 SDA 和 GPIO22 SCL

// 多功能引脚 在硬件支持时 Aux3..Aux8 可用于模拟量 PWM 或 DAC
#define AUX2_PIN                2                // 可切换 IO TMC UART RX / SPI MISO
#define AUX3_PIN                25               // Axis1 原点开关
#define AUX4_PIN                15               // Axis2 原点开关
#define AUX5_PIN                1                // TX0 主 USB 等
#define AUX6_PIN                3                // RX0 主 USB 等
#define AUX7_PIN                39               // 限位开关等
#define AUX8_PIN                12               // 单总线 状态 LED 分划板 LED 蜂鸣音等

// 其他引脚
#ifndef ONE_WIRE_PIN
  #define ONE_WIRE_PIN          AUX8_PIN         // 单总线的默认引脚
#endif

// PEC 索引感应为逻辑电平输入 在上升沿复位 PEC 索引 随后等待 60 秒才允许再次复位
#ifndef PEC_SENSE_PIN
  #define PEC_SENSE_PIN         OFF              // ESP3 硬件未使用 PEC 感应 GPIO36 保留给 PPS
#endif

// 状态 LED 串联 4.7k 电阻以限制 LED 电流
#ifdef STATUS_LED_ON_STATE
  #undef STATUS_LED_ON_STATE
#endif
#define STATUS_LED_ON_STATE HIGH
#ifndef STATUS_LED_PIN
  #define STATUS_LED_PIN        12               // 默认 LED 阳极 +
#endif
#ifdef MOUNT_LED_ON_STATE
  #undef MOUNT_LED_ON_STATE
#endif
#define MOUNT_LED_ON_STATE      HIGH
#define MOUNT_LED_PIN           STATUS_LED_PIN   // 默认 LED 阳极 +
#ifndef RETICLE_LED_PIN
  #define RETICLE_LED_PIN       AUX8_PIN         // 默认 LED 阴极 -
#endif

// 用于压电蜂鸣器
#ifndef STATUS_BUZZER_PIN
  #define STATUS_BUZZER_PIN     12               // 启动时必须为低电平 12 蜂鸣音
#endif

// PPS 引脚为 3.3V 逻辑输入 OnStep 测量上升沿之间的时间并调整内部恒星时时钟频率
#ifndef PPS_SENSE_PIN
  #define PPS_SENSE_PIN         36               // PPS 时间源 例如 GPS 仅输入引脚 GPIO36
#endif

// 限位开关感应为逻辑电平输入 通常通过 2k 电阻上拉 接地短路时停止自动寻星和跟踪
#ifndef LIMIT_SENSE_PIN
  #define LIMIT_SENSE_PIN       AUX7_PIN         // GPIO39 仅输入引脚
#endif

#ifndef SHARED_ENABLE_PIN
  #define SHARED_ENABLE_PIN     4                // 表示使能引脚共用
#endif

// Axis1 赤经或方位步进方向驱动器
#define AXIS1_ENABLE_PIN        SHARED           // 使能引脚控制
#if defined(STEP_DIR_TMC_UART_PRESENT) || defined(SERVO_TMC2209_PRESENT)
  #define AXIS1_M0_PIN          OFF              // UART 模式硬件下拉设置 TMC UART 地址 0 SPI 模式切换为 GPIO13 MOSI
  #define AXIS1_M1_PIN          OFF              // UART 模式硬件下拉设置 TMC UART 地址 0 SPI 模式切换为 GPIO14 SCK
  #define AXIS1_M2_PIN          OFF              // 可切换 IO UART TX 使用 GPIO23 SPI 模式切换为 GPIO23 CS
  #define AXIS1_M3_PIN          OFF              // 可切换 IO UART RX 使用 GPIO2 SPI 模式切换为 GPIO2 MISO
#else
  #define AXIS1_M0_PIN          13               // 可切换 IO SPI MOSI UART 模式为 TMC UART 地址 0
  #define AXIS1_M1_PIN          14               // 可切换 IO SPI SCK UART 模式为 TMC UART 地址 0
  #define AXIS1_M2_PIN          23               // 可切换 IO SPI CS UART 模式为 TMC UART TX
  #define AXIS1_M3_PIN          AUX2_PIN         // 可切换 IO SPI MISO UART 模式为 TMC UART RX
#endif
#define AXIS1_STEP_PIN          18
#define AXIS1_DIR_PIN           19
#ifndef AXIS1_SENSE_HOME_PIN
  #define AXIS1_SENSE_HOME_PIN  AUX3_PIN
#endif

// Axis2 赤纬或高度步进方向驱动器
#define AXIS2_ENABLE_PIN        SHARED
#if defined(STEP_DIR_TMC_UART_PRESENT) || defined(SERVO_TMC2209_PRESENT)
  #define AXIS2_M0_PIN          OFF              // UART 模式拨码接 3.3V 设置 TMC UART 地址 1 SPI 模式切换为 GPIO13 MOSI
  #define AXIS2_M1_PIN          OFF              // UART 模式硬件下拉设置 TMC UART 地址 1 SPI 模式切换为 GPIO14 SCK
  #define AXIS2_M2_PIN          OFF              // 可切换 IO UART 模式 PDN 断开 SPI 模式切换为 GPIO5 CS
  #define AXIS2_M3_PIN          OFF              // 可切换 IO UART RX 使用 GPIO2 SPI 模式切换为 GPIO2 MISO
#else
  #define AXIS2_M0_PIN          13               // 可切换 IO SPI MOSI UART 模式由拨码设置 TMC UART 地址 1
  #define AXIS2_M1_PIN          14               // 可切换 IO SPI SCK UART 模式为 TMC UART 地址 1
  #define AXIS2_M2_PIN          5                // 可切换 IO SPI CS UART 模式 PDN 断开
  #define AXIS2_M3_PIN          AUX2_PIN         // 可切换 IO SPI MISO UART 模式为 TMC UART RX
#endif
#define AXIS2_STEP_PIN          27
#define AXIS2_DIR_PIN           26
#ifndef AXIS2_SENSE_HOME_PIN
  #define AXIS2_SENSE_HOME_PIN  AUX4_PIN
#endif
#ifndef AXIS2_ENCODER_B_PIN
  #define AXIS2_ENCODER_B_PIN   OFF              // GPIO2 已用于 TMC UART RX / SPI MISO
#endif

// Axis1/Axis2 的 M0/M1 共用 SPI MOSI/SCK 总线 M3 共用 SPI MISO/UART RX 总线 M2 使用独立 CS

// 用于旋转器步进驱动器
#define AXIS3_ENABLE_PIN        OFF              // 未使用
#define AXIS3_M0_PIN            OFF              // 未使用
#define AXIS3_M1_PIN            OFF              // 未使用
#define AXIS3_M2_PIN            OFF              // 未使用
#define AXIS3_M3_PIN            OFF              // 未使用
#define AXIS3_STEP_PIN          OFF              // ESP3 原始值 IO2 当前用于 TMC UART RX / SPI MISO
#define AXIS3_DIR_PIN           OFF              // ESP3 原始值 IO15 当前用于 Axis2 原点开关

// 用于调焦器1步进驱动器
#define AXIS4_ENABLE_PIN        OFF              // 未使用
#define AXIS4_M0_PIN            OFF              // 未使用
#define AXIS4_M1_PIN            OFF              // 未使用
#define AXIS4_M2_PIN            OFF              // 未使用
#define AXIS4_M3_PIN            OFF              // 未使用
#define AXIS4_STEP_PIN          OFF              // ESP3 原始值 IO19 当前用于 Axis1 DIR
#define AXIS4_DIR_PIN           OFF              // ESP3 原始值 IO15 当前用于 Axis2 原点开关

// 用于调焦器2步进驱动器
#define AXIS5_ENABLE_PIN        OFF              // 未使用
#define AXIS5_M0_PIN            OFF              // 未使用
#define AXIS5_M1_PIN            OFF              // 未使用
#define AXIS5_M2_PIN            OFF              // 未使用
#define AXIS5_M3_PIN            OFF              // 未使用
#define AXIS5_STEP_PIN          OFF              // ESP3 原始值 IO2 当前用于 TMC UART RX / SPI MISO
#define AXIS5_DIR_PIN           OFF              // ESP3 原始值 IO5 当前用于 Axis2 SPI CS

// ST4 接口
#define ST4_RA_W_PIN            34               // 仅输入引脚 34 ST4 RA- 西
#define ST4_DEC_S_PIN           32               // ST4 DE- 南
#define ST4_DEC_N_PIN           33               // ST4 DE+ 北
#define ST4_RA_E_PIN            35               // 仅输入引脚 35 ST4 RA+ 东

#else
#error "Wrong processor for this configuration!"

#endif
