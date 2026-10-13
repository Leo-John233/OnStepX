// -------------------------------------------------------------------------------------------------
// OnStep MaxESP4 引脚映射 ESP32S
#pragma once

#if defined(ESP32)

// Serial0 接收 GPIO3 发送 GPIO1 连接 USB 串口适配器
// Serial1 接收 GPIO10 发送 GPIO9 与 SPI Flash 共用引脚 使用前需改接
// Serial2 接收 GPIO16 发送 GPIO17

#if SERIAL_A_BAUD_DEFAULT != OFF
  #define SERIAL_A              Serial
#endif
#if SERIAL_B_BAUD_DEFAULT != OFF
  #define SERIAL_B              Serial2
  #define SERIAL_B_RX           16
  #define SERIAL_B_TX           17
#endif

// TMC UART 驱动器设置 适用于 TMC2209
#if defined(STEP_DIR_TMC_UART_PRESENT) || defined(SERVO_TMC2209_PRESENT)
  #define SERIAL_TMC_HARDWARE_UART
  #define SERIAL_TMC            Serial1          // 一个硬件串口最多连接四个驱动器
  #define SERIAL_TMC_BAUD       460800           // 波特率
  #define SERIAL_TMC_RX         39               // 接收数据
  #ifndef SERIAL_TMC_TX
    #define SERIAL_TMC_TX       4                // 发送数据
  #endif
  #define SERIAL_TMC_ADDRESS_MAP(x) ((x==4)?2 : x) // 轴1至轴5的 UART 地址依次为 0 1 2 3 2
#endif

// 使用 ESP32 默认 I2C 引脚 GPIO21 为 SDA GPIO22 为 SCL

// 多功能引脚 AUX3 至 AUX8 可在硬件支持时用于 PWM 或 DAC 输出
#define AUX2_PIN                39               // M3 引脚 TMC UART 接收
#define AUX3_PIN                14               // 轴1原点开关
#define AUX4_PIN                13               // 轴2原点开关
#define AUX5_PIN                1                // TX0 主 USB 串口发送等
#define AUX6_PIN                3                // RX0 主 USB 串口接收等
#define AUX7_PIN                23               // 限位开关 PPS 等
#define AUX8_PIN                25               // 单总线 状态灯 分划板照明 蜂鸣器等

// 其他引脚
#ifndef ONE_WIRE_PIN
  #define ONE_WIRE_PIN          AUX8_PIN         // 单总线默认引脚
#endif

// PEC 索引检测采用逻辑电平输入 上升沿复位索引 两次复位至少间隔 60 秒
#ifndef PEC_SENSE_PIN
  #define PEC_SENSE_PIN         36               // PEC 检测 支持模拟输入 A0 或数字输入 GPIO36 仅可输入
#endif

// 状态灯串联 4.7kΩ 电阻限流
#ifdef STATUS_LED_ON_STATE
  #undef STATUS_LED_ON_STATE
#endif
#define STATUS_LED_ON_STATE HIGH
#ifndef STATUS_LED_PIN
  #define STATUS_LED_PIN        12               // 默认连接 LED 正极 +
#endif
#ifdef MOUNT_LED_ON_STATE
  #undef MOUNT_LED_ON_STATE
#endif
#define MOUNT_LED_ON_STATE      HIGH
#define MOUNT_LED_PIN           STATUS_LED_PIN   // 默认连接 LED 正极 +
#ifndef RETICLE_LED_PIN
  #define RETICLE_LED_PIN       AUX8_PIN         // 默认连接 LED 负极 -
#endif

// 压电蜂鸣器
#ifndef STATUS_BUZZER_PIN
  #define STATUS_BUZZER_PIN     12               // 蜂鸣器 GPIO12 启动时必须为低电平
#endif

// PPS 为 3.3V 逻辑输入 OnStep 根据上升沿间隔校准内部恒星时时钟
#ifndef PPS_SENSE_PIN
  #define PPS_SENSE_PIN         AUX7_PIN         // PPS 时间源 例如 GPS
#endif

// 限位输入通过 2kΩ 电阻上拉 接地时停止自动寻星和跟踪
#ifndef LIMIT_SENSE_PIN
  #define LIMIT_SENSE_PIN       AUX7_PIN
#endif

#define SHARED_DIRECTION_PINS                    // 方向引脚共用
#ifndef SHARED_ENABLE_PIN
  #define SHARED_ENABLE_PIN     5                // 使能引脚共用
#endif

// 轴1 赤经或方位 步进脉冲与方向引脚
#define AXIS1_ENABLE_PIN        SHARED           // 使能控制
#define AXIS1_M0_PIN            OFF              // 硬件固定 TMC UART 地址 0
#define AXIS1_M1_PIN            OFF              // 硬件固定 TMC UART 地址 0
#define AXIS1_M2_PIN            OFF              // UART 发送
#define AXIS1_M3_PIN            OFF              // UART 接收
#define AXIS1_STEP_PIN          18
#define AXIS1_DIR_PIN           0                // GPIO0 启动时必须为高电平
#ifndef AXIS1_SENSE_HOME_PIN
  #define AXIS1_SENSE_HOME_PIN  AUX3_PIN
#endif

// 轴2 赤纬或高度 步进脉冲与方向引脚
#define AXIS2_ENABLE_PIN        SHARED
#define AXIS2_M0_PIN            OFF              // 硬件固定 TMC UART 地址 1
#define AXIS2_M1_PIN            OFF              // 硬件固定 TMC UART 地址 1
#define AXIS2_M2_PIN            OFF              // 未连接
#define AXIS2_M3_PIN            OFF              // UART 接收
#define AXIS2_STEP_PIN          27
#define AXIS2_DIR_PIN           26
#ifndef AXIS2_SENSE_HOME_PIN
  #define AXIS2_SENSE_HOME_PIN  AUX4_PIN
#endif
#ifndef AXIS2_ENCODER_B_PIN
  #define AXIS2_ENCODER_B_PIN   AUX2_PIN
#endif

// 旋转器步进驱动器
#define AXIS3_ENABLE_PIN        SHARED
#define AXIS3_M0_PIN            OFF              // 硬件固定 TMC UART 地址 2
#define AXIS3_M1_PIN            OFF              // 硬件固定 TMC UART 地址 2
#define AXIS3_M2_PIN            OFF              // 未连接
#define AXIS3_M3_PIN            OFF              // UART 接收
#define AXIS3_STEP_PIN          2                // GPIO2 启动时必须为低电平
#define AXIS3_DIR_PIN           15

// 调焦器1步进驱动器
#define AXIS4_ENABLE_PIN        SHARED
#define AXIS4_M0_PIN            OFF              // 硬件固定 TMC UART 地址 3
#define AXIS4_M1_PIN            OFF              // 硬件固定 TMC UART 地址 3
#define AXIS4_M2_PIN            OFF              // 未连接
#define AXIS4_M3_PIN            OFF              // UART 接收
#define AXIS4_STEP_PIN          19
#define AXIS4_DIR_PIN           15

// 调焦器2步进驱动器
#define AXIS5_ENABLE_PIN        SHARED
#define AXIS5_M0_PIN            OFF              // 硬件固定 TMC UART 地址 2
#define AXIS5_M1_PIN            OFF              // 硬件固定 TMC UART 地址 2
#define AXIS5_M2_PIN            OFF              // 未连接
#define AXIS5_M3_PIN            OFF              // UART 接收
#define AXIS5_STEP_PIN          2
#define AXIS5_DIR_PIN           15

// ST4 接口
#define ST4_RA_W_PIN            34               // ST4 赤经负向 西 GPIO34 仅可输入
#define ST4_DEC_S_PIN           32               // ST4 赤纬负向 南
#define ST4_DEC_N_PIN           33               // ST4 赤纬正向 北
#define ST4_RA_E_PIN            35               // ST4 赤经正向 东 GPIO35 仅可输入

#else
#error "Wrong processor for this configuration!"

#endif
