// -------------------------------------------------------------------------------------------------
// OnStep MaxESP4 引脚映射 ESP32S
#pragma once

#if defined(ESP32)

// Config.h 选择 TMC5160 使用图纸 SPI 接线 选择 TMC2209 使用原 UART 接线
#if defined(STEP_DIR_TMC_SPI_PRESENT) && (defined(STEP_DIR_TMC_UART_PRESENT) || defined(SERVO_TMC2209_PRESENT))
  #error "MaxESP4 does not support mixed SPI/UART wiring"
#endif

// Serial0 接收 GPIO3 发送 GPIO1 用于 USB 串口
// Serial1 由下方 UART 设置指定引脚
// Serial2 接收 GPIO16 发送 GPIO17

#if SERIAL_A_BAUD_DEFAULT != OFF
  #define SERIAL_A                         Serial
#endif
#if SERIAL_B_BAUD_DEFAULT != OFF
  #define SERIAL_B                         Serial2
  #define SERIAL_B_RX                      16
  #define SERIAL_B_TX                      17
#endif

// TMC UART 驱动器设置 适用于 TMC2209
#if defined(STEP_DIR_TMC_UART_PRESENT) || defined(SERVO_TMC2209_PRESENT)
  #define SERIAL_TMC_HARDWARE_UART
  #define SERIAL_TMC                       Serial1                // 最多连接四个驱动器
  #define SERIAL_TMC_BAUD                  460800                 // 波特率
  #define SERIAL_TMC_RX                    39                     // 接收数据
  #ifndef SERIAL_TMC_TX
    #define SERIAL_TMC_TX                  4                      // 发送数据
  #endif
  #define SERIAL_TMC_ADDRESS_MAP(x)        ((x==4)?2 : x)         // 轴1至轴5的 UART 地址依次为 0 1 2 3 2
#endif

// 使用 ESP32 默认 I2C 引脚 GPIO21 为 SDA GPIO22 为 SCL

// 多功能引脚
#if defined(STEP_DIR_TMC_SPI_PRESENT)
  #define AUX2_PIN                         2                      // SPI 接收
  #define AUX3_PIN                         25                     // 赤经原点
  #define AUX4_PIN                         15                     // 赤纬原点
  #define AUX7_PIN                         39                     // 赤纬限位
  #define AUX8_PIN                         12                     // 状态灯
#else
  #define AUX2_PIN                         39                     // UART 接收
  #define AUX3_PIN                         14                     // 赤经原点
  #define AUX4_PIN                         13                     // 赤纬原点
  #define AUX7_PIN                         23                     // 共用限位或 PPS
  #define AUX8_PIN                         25                     // 单总线或辅助输出
#endif
#define AUX5_PIN                           1                      // USB 串口发送
#define AUX6_PIN                           3                      // USB 串口接收

// 其他引脚
#if defined(STEP_DIR_TMC_SPI_PRESENT)
  // ESP07 烧录前停止轴运动 烧录时短接 RST 跳线 正常运行时断开
  #ifndef ADDON_GPIO0_PIN
    #define ADDON_GPIO0_PIN                26                     // ESP07 GPIO0 与赤纬方向共用
  #endif
  #ifndef ADDON_RESET_PIN
    #define ADDON_RESET_PIN                2                      // ESP07 RST 经跳线连接 SPI MISO
  #endif
  #if ADDON_RESET_PIN == 2
    #define ADDON_RESET_SHARED_INPUT                              // 复位后释放共用输入引脚
  #endif
#endif
#ifndef ONE_WIRE_PIN
  #define ONE_WIRE_PIN                     AUX8_PIN               // 单总线默认引脚
#endif

// PEC 索引输入 上升沿触发 间隔至少 60 秒
#ifndef PEC_SENSE_PIN
  #if defined(STEP_DIR_TMC_SPI_PRESENT)
    #define PEC_SENSE_PIN                  OFF                    // 沿用 OnStep MaxESP3 的 PEC 设置
  #else
    #define PEC_SENSE_PIN                  36                     // PEC 输入 GPIO36 仅可输入
  #endif
#endif

// 状态灯
#ifdef STATUS_LED_ON_STATE
  #undef STATUS_LED_ON_STATE
#endif
#if defined(STEP_DIR_TMC_SPI_PRESENT)
  #define STATUS_LED_ON_STATE              LOW                    // 图纸状态灯低电平点亮
#else
  #define STATUS_LED_ON_STATE              HIGH
#endif
#ifndef STATUS_LED_PIN
  #define STATUS_LED_PIN                   12                     // 状态灯输出
#endif
#ifdef MOUNT_LED_ON_STATE
  #undef MOUNT_LED_ON_STATE
#endif
#define MOUNT_LED_ON_STATE                 STATUS_LED_ON_STATE
#define MOUNT_LED_PIN                      STATUS_LED_PIN         // 状态灯输出
#ifndef RETICLE_LED_PIN
  #define RETICLE_LED_PIN                  AUX8_PIN               // 默认连接 LED 负极 -
#endif

// 压电蜂鸣器
#ifndef STATUS_BUZZER_PIN
  #define STATUS_BUZZER_PIN                12                     // 蜂鸣器 GPIO12 启动时必须为低电平
#endif

// PPS 输入 3.3V 上升沿校准时钟
#ifndef PPS_SENSE_PIN
  #define PPS_SENSE_PIN                    AUX7_PIN               // PPS 时间源 例如 GPS
#endif

// 默认限位输入 接地触发 各轴可单独指定引脚
#ifndef LIMIT_SENSE_PIN
  #define LIMIT_SENSE_PIN                  AUX7_PIN
#endif

#if !defined(STEP_DIR_TMC_SPI_PRESENT)
  #define SHARED_DIRECTION_PINS                                   // 原 UART 接线共用方向引脚
#endif
#ifndef SHARED_ENABLE_PIN
  #if defined(STEP_DIR_TMC_SPI_PRESENT)
    #define SHARED_ENABLE_PIN              4                      // 两轴共用使能
  #else
    #define SHARED_ENABLE_PIN              5                      // 使能引脚共用
  #endif
#endif

// 轴1 赤经或方位
#define AXIS1_ENABLE_PIN                   SHARED                 // 使能控制
#if defined(STEP_DIR_TMC_SPI_PRESENT)
  #define AXIS1_M0_PIN                     13                     // SPI MOSI
  #define AXIS1_M1_PIN                     14                     // SPI SCK
  #define AXIS1_M2_PIN                     23                     // SPI 片选
  #define AXIS1_M3_PIN                     2                      // SPI MISO
#else
  #define AXIS1_M0_PIN                     OFF                    // 硬件固定 TMC UART 地址 0
  #define AXIS1_M1_PIN                     OFF                    // 硬件固定 TMC UART 地址 0
  #define AXIS1_M2_PIN                     OFF                    // UART 发送
  #define AXIS1_M3_PIN                     OFF                    // UART 接收
#endif
#define AXIS1_STEP_PIN                     18
#if defined(STEP_DIR_TMC_SPI_PRESENT)
  #define AXIS1_DIR_PIN                    19
#else
  #define AXIS1_DIR_PIN                    0                      // GPIO0 启动时必须为高电平
#endif
#ifndef AXIS1_SENSE_HOME_PIN
  #define AXIS1_SENSE_HOME_PIN             AUX3_PIN
#endif
#if defined(STEP_DIR_TMC_SPI_PRESENT)
  #define AXIS1_SENSE_LIMIT_MIN_PIN        36                     // 本轴限位输入 外部上拉
  #define AXIS1_SENSE_LIMIT_MAX_PIN        36
#endif

// 轴2 赤纬或高度
#define AXIS2_ENABLE_PIN                   SHARED
#if defined(STEP_DIR_TMC_SPI_PRESENT)
  #define AXIS2_M0_PIN                     13                     // SPI MOSI
  #define AXIS2_M1_PIN                     14                     // SPI SCK
  #define AXIS2_M2_PIN                     5                      // SPI 片选
  #define AXIS2_M3_PIN                     2                      // SPI MISO
#else
  #define AXIS2_M0_PIN                     OFF                    // 硬件固定 TMC UART 地址 1
  #define AXIS2_M1_PIN                     OFF                    // 硬件固定 TMC UART 地址 1
  #define AXIS2_M2_PIN                     OFF                    // 未连接
  #define AXIS2_M3_PIN                     OFF                    // UART 接收
#endif
#define AXIS2_STEP_PIN                     27
#define AXIS2_DIR_PIN                      26
#ifndef AXIS2_SENSE_HOME_PIN
  #define AXIS2_SENSE_HOME_PIN             AUX4_PIN
#endif
#if defined(STEP_DIR_TMC_SPI_PRESENT)
  #define AXIS2_SENSE_LIMIT_MIN_PIN        39                     // 本轴限位输入 外部上拉
  #define AXIS2_SENSE_LIMIT_MAX_PIN        39
#endif
#ifndef AXIS2_ENCODER_B_PIN
  #if defined(STEP_DIR_TMC_SPI_PRESENT)
    #define AXIS2_ENCODER_B_PIN            OFF                    // GPIO2 用于 SPI 接收
  #else
    #define AXIS2_ENCODER_B_PIN            AUX2_PIN
  #endif
#endif

// 旋转器步进驱动器
#define AXIS3_M0_PIN                       OFF                    // 硬件固定 TMC UART 地址 2
#define AXIS3_M1_PIN                       OFF                    // 硬件固定 TMC UART 地址 2
#define AXIS3_M2_PIN                       OFF                    // 未连接
#define AXIS3_M3_PIN                       OFF                    // UART 接收
#if defined(STEP_DIR_TMC_SPI_PRESENT)
  #define AXIS3_ENABLE_PIN                 OFF                    // 图纸未连接此轴
  #define AXIS3_STEP_PIN                   OFF
  #define AXIS3_DIR_PIN                    OFF
#else
  #define AXIS3_ENABLE_PIN                 SHARED
  #define AXIS3_STEP_PIN                   2                      // GPIO2 启动时必须为低电平
  #define AXIS3_DIR_PIN                    15
#endif

// 调焦器1步进驱动器
#define AXIS4_M0_PIN                       OFF                    // 硬件固定 TMC UART 地址 3
#define AXIS4_M1_PIN                       OFF                    // 硬件固定 TMC UART 地址 3
#define AXIS4_M2_PIN                       OFF                    // 未连接
#define AXIS4_M3_PIN                       OFF                    // UART 接收
#if defined(STEP_DIR_TMC_SPI_PRESENT)
  #define AXIS4_ENABLE_PIN                 OFF                    // 图纸未连接此轴
  #define AXIS4_STEP_PIN                   OFF
  #define AXIS4_DIR_PIN                    OFF
#else
  #define AXIS4_ENABLE_PIN                 SHARED
  #define AXIS4_STEP_PIN                   19
  #define AXIS4_DIR_PIN                    15
#endif

// 调焦器2步进驱动器
#define AXIS5_M0_PIN                       OFF                    // 硬件固定 TMC UART 地址 2
#define AXIS5_M1_PIN                       OFF                    // 硬件固定 TMC UART 地址 2
#define AXIS5_M2_PIN                       OFF                    // 未连接
#define AXIS5_M3_PIN                       OFF                    // UART 接收
#if defined(STEP_DIR_TMC_SPI_PRESENT)
  #define AXIS5_ENABLE_PIN                 OFF                    // 图纸未连接此轴
  #define AXIS5_STEP_PIN                   OFF
  #define AXIS5_DIR_PIN                    OFF
#else
  #define AXIS5_ENABLE_PIN                 SHARED
  #define AXIS5_STEP_PIN                   2
  #define AXIS5_DIR_PIN                    15
#endif

// ST4 接口
#define ST4_RA_W_PIN                       34                     // ST4 赤经负向 西 GPIO34 仅可输入
#define ST4_DEC_S_PIN                      32                     // ST4 赤纬负向 南
#define ST4_DEC_N_PIN                      33                     // ST4 赤纬正向 北
#define ST4_RA_E_PIN                       35                     // ST4 赤经正向 东 GPIO35 仅可输入

#else
#error "Wrong processor for this configuration!"

#endif
