// -------------------------------------------------------------------------------------------------
// OnStep MaxESP 3.x 版本引脚映射 ESP32S
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

// 以下设置适用于所有可能存在的 TMC UART 驱动器 TMC2209
#if defined(STEP_DIR_TMC_UART_PRESENT) || defined(SERVO_TMC2209_PRESENT)
  #if defined(SERIAL_TMC_HARDWARE_UART)
    #define SERIAL_TMC          Serial1          // 使用一个硬件串口连接最多四个驱动器
    #define SERIAL_TMC_BAUD     460800           // 波特率
    #define SERIAL_TMC_RX       39               // 接收数据
    #define SERIAL_TMC_TX       23               // 发送数据
    #define SERIAL_TMC_ADDRESS_MAP(x) ((x==4)?2 : x) // Axis1(0) 为 0 Axis2(1) 为 1 Axis3(2) 为 2 Axis4(3) 为 3 Axis5(4) 为 2
  #endif
#endif

// 使用 ESP32 默认 I2C 引脚 GPIO21 SDA 和 GPIO22 SCL

// 多功能引脚 在硬件支持时 Aux3..Aux8 可用于模拟量 PWM 或 DAC
#define AUX2_PIN                4                // ESP8266 RST 控制 或 Axis1 和 Axis2 的 MISO 或 Axis4 EN 支持
#define AUX3_PIN                21               // Axis1 原点开关 或 I2C SDA
#define AUX4_PIN                22               // Axis2 原点开关 或 I2C SCL
#define AUX7_PIN                39               // 限位开关 PPS 等
#define AUX8_PIN                25               // 单总线 状态 LED 分划板 LED 蜂鸣音等

// 其他引脚
#ifndef ONE_WIRE_PIN
  #define ONE_WIRE_PIN          AUX8_PIN         // 单总线的默认引脚
#endif
#define ADDON_GPIO0_PIN         26               // ESP8266 GPIO0 Dir2
#ifndef ADDON_RESET_PIN
  #define ADDON_RESET_PIN       AUX2_PIN         // ESP8266 复位
#endif

// PEC 索引感应为逻辑电平输入 在上升沿复位 PEC 索引 随后等待 60 秒才允许再次复位
#ifndef PEC_SENSE_PIN
  #define PEC_SENSE_PIN         36               // 仅输入引脚 36 PEC 感应 可使用模拟 A0 或数字 GPIO36
#endif

// 状态 LED 使用双线跳线并串联 2k 电阻以限制 LED 电流
#ifndef STATUS_LED_PIN
  #define STATUS_LED_PIN        AUX8_PIN         // 默认 LED 阴极 -
#endif
#define MOUNT_LED_PIN           STATUS_LED_PIN   // 默认 LED 阴极 -
#ifndef RETICLE_LED_PIN 
  #define RETICLE_LED_PIN       STATUS_LED_PIN   // 默认 LED 阴极 -
#endif

// 用于压电蜂鸣器
#ifndef STATUS_BUZZER_PIN
  #define STATUS_BUZZER_PIN     AUX8_PIN         // 蜂鸣音
#endif

// PPS 引脚为 3.3V 逻辑输入 OnStep 测量上升沿之间的时间并调整内部恒星时时钟频率
#ifndef PPS_SENSE_PIN
  #define PPS_SENSE_PIN         AUX7_PIN         // PPS 时间源 例如 GPS
#endif

// 限位开关感应为逻辑电平输入 通常通过 2k 电阻上拉 接地短路时停止自动寻星和跟踪
#ifndef LIMIT_SENSE_PIN
  #define LIMIT_SENSE_PIN       AUX7_PIN
#endif

#define SHARED_DIRECTION_PINS                    // 表示方向引脚共用
#define SHARED_ENABLE_PIN       12               // 表示使能引脚共用

// Axis1 赤经或方位步进方向驱动器
#define AXIS1_ENABLE_PIN        SHARED           // 启动时必须为低电平 12
#define AXIS1_M0_PIN            13               // SPI MOSI
#define AXIS1_M1_PIN            14               // SPI SCK
#define AXIS1_M2_PIN            23               // SPI CS UART TX
#if AXIS4_POWER_DOWN != ON
  #define AXIS1_M3_PIN          AUX2_PIN         // SPI MISO UART RX
#endif
#define AXIS1_STEP_PIN          18
#define AXIS1_DIR_PIN           0                // 启动时必须为高电平 0
#ifndef AXIS1_SENSE_HOME_PIN
  #define AXIS1_SENSE_HOME_PIN  AUX3_PIN
#endif

// Axis2 赤纬或高度步进方向驱动器
#define AXIS2_ENABLE_PIN        SHARED
#define AXIS2_M0_PIN            13               // SPI MOSI
#define AXIS2_M1_PIN            14               // SPI SCK
#define AXIS2_M2_PIN            5                // SPI CS UART TX
#if AXIS4_POWER_DOWN != ON
  #define AXIS2_M3_PIN          AUX2_PIN         // SPI MISO UART RX
#endif
#define AXIS2_STEP_PIN          27
#define AXIS2_DIR_PIN           26
#ifndef AXIS2_SENSE_HOME_PIN
  #define AXIS2_SENSE_HOME_PIN  AUX4_PIN
#endif

// 用于旋转器步进驱动器
#define AXIS3_ENABLE_PIN        OFF              // 无使能引脚控制 始终启用
#define AXIS3_M0_PIN            OFF              // SPI MOSI
#define AXIS3_M1_PIN            OFF              // SPI SCK
#define AXIS3_M2_PIN            OFF              // SPI CS UART TX
#define AXIS3_M3_PIN            OFF              // SPI MISO UART RX
#define AXIS3_STEP_PIN          2                // 启动时必须为低电平 2
#define AXIS3_DIR_PIN           15

// 用于调焦器1步进驱动器
#if !defined(SERIAL_TMC_HARDWARE_UART) && AXIS4_POWER_DOWN == ON
  #define AXIS4_ENABLE_PIN      AUX2_PIN
#endif
#define AXIS4_M0_PIN            OFF              // SPI MOSI
#define AXIS4_M1_PIN            OFF              // SPI SCK
#define AXIS4_M2_PIN            OFF              // SPI CS UART TX
#define AXIS4_M3_PIN            OFF              // SPI MISO UART RX
#define AXIS4_STEP_PIN          19
#define AXIS4_DIR_PIN           15

// 用于调焦器2步进驱动器
#define AXIS5_ENABLE_PIN        OFF              // 无使能引脚控制 始终启用
#define AXIS5_M0_PIN            OFF              // SPI MOSI
#define AXIS5_M1_PIN            OFF              // SPI SCK
#define AXIS5_M2_PIN            OFF              // SPI CS UART TX
#define AXIS5_M3_PIN            OFF              // SPI MISO UART RX
#define AXIS5_STEP_PIN          2
#define AXIS5_DIR_PIN           15

// ST4 接口
#define ST4_RA_W_PIN            34               // 仅输入引脚 34 ST4 RA- 西
#define ST4_DEC_S_PIN           32               // ST4 DE- 南
#define ST4_DEC_N_PIN           33               // ST4 DE+ 北
#define ST4_RA_E_PIN            35               // 仅输入引脚 35 ST4 RA+ 东

#else
#error "Wrong processor for this configuration!"

#endif
