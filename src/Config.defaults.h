// -----------------------------------------------------------------------------------
// 控制器默认配置
#pragma once

// 控制器主机名 以下功能启用且受支持时默认使用此名称
// PRODUCT_DESCRIPTION    设备显示名称 用于网页等界面
// HOST_NAME              ESP WiFi 向 DHCP 服务器提供的名称 以太网不支持
// MDNS_NAME              mDNS 客户端解析 IP 地址时使用的名称
// AP_SSID                ESP WiFi 热点对外显示的名称
// SERIAL_BT_NAME         ESP32 蓝牙客户端对蓝牙服务器显示的名称
#ifndef HOST_NAME
#define HOST_NAME                           "OnStepX"      // 本设备的主机名 最多 16 个字符
#endif

// 设备名称
#ifndef PRODUCT_DESCRIPTION
#define PRODUCT_DESCRIPTION                 HOST_NAME      // 设备显示名称
#endif

// 使用硬件抽象层指定的默认非易失存储驱动
#ifndef NV_DRIVER
#define NV_DRIVER                           NV_DEFAULT     // 使用平台默认的非易失性存储设备保存运行时设置
#endif

// 引脚映射
#ifndef PINMAP
#define PINMAP                              OFF            // 可选值 MiniPCB, MiniPCB2, MaxPCB4, MaxESP4, MaxSTM3, FYSETC_E4, 默认 OFF
#endif

// 调试设置
#ifndef DEBUG
#define DEBUG                               OFF            // 默认 OFF | ON 仅输出后台错误信息 VERBOSE 输出全部错误和状态信息
#endif
#ifndef DEBUG_ECHO_COMMANDS
#define DEBUG_ECHO_COMMANDS                 OFF            // ON 或 ERRORS_ONLY 将命令记录到调试串口
#endif
#ifndef SERIAL_DEBUG
#define SERIAL_DEBUG                        Serial         // 使用任意可用的硬件串口 例如 Serial1 或 Serial2
#endif
#ifndef SERIAL_DEBUG_BAUD
#define SERIAL_DEBUG_BAUD                   9600           // 调试串口波特率 默认 9600
#endif

// 标记硬件 SPI 已启用
#ifdef DRIVER_TMC_STEPPER_HW_SPI
#ifndef DRIVER_TMC_STEPPER
#define DRIVER_TMC_STEPPER                                 // 启用 TMCStepper 驱动库
#endif
#define USES_HW_SPI                                        // 标记使用硬件 SPI
#endif

// 串口设置
#ifndef SERIAL_A_BAUD_DEFAULT
#define SERIAL_A_BAUD_DEFAULT               9600           // 其中 n=9600,19200,57600,115200,230400,460800 为常用波特率
#endif
#ifndef SERIAL_B_BAUD_DEFAULT
#define SERIAL_B_BAUD_DEFAULT               OFF            // 波特率同上 串口分配请查看 src/pinmaps/
#endif
#ifndef SERIAL_B_ESP_FLASHING
#define SERIAL_B_ESP_FLASHING               OFF            // ON 通过 SERIAL_B 使用 :ESPFLASH# 命令上传 ESP8266 WiFi 固件
#endif
#ifndef SERIAL_C_BAUD_DEFAULT
#define SERIAL_C_BAUD_DEFAULT               OFF            // 波特率同上 串口分配请查看 src/pinmaps/
#endif
#ifndef SERIAL_D_BAUD_DEFAULT
#define SERIAL_D_BAUD_DEFAULT               OFF            // 波特率同上 串口分配请查看 src/pinmaps/
#endif
#ifndef SERIAL_E_BAUD_DEFAULT
#define SERIAL_E_BAUD_DEFAULT               OFF            // 波特率同上 串口分配请查看 src/pinmaps/
#endif
#ifndef SERIAL_GPS_BAUD
#define SERIAL_GPS_BAUD                     9600           // GPS 串口波特率
#endif

// ESP32 根据蓝牙或 IP 模式自动配置无线通信
#ifdef SERIAL_RADIO
#if SERIAL_RADIO == BLUETOOTH
#define SERIAL_BT_MODE                      SLAVE          // 设为 SLAVE 启用接口 仅支持 ESP32
#elif SERIAL_RADIO == WIFI_ACCESS_POINT
// 使用以下设置启用接口
#define SERIAL_IP_MODE                      WIFI_ACCESS_POINT
#define WEB_SERVER                          ON             // 启用网页服务
#elif SERIAL_RADIO == WIFI_STATION
#define SERIAL_IP_MODE                      WIFI_STATION   // 使用以下设置启用接口
#define WEB_SERVER                          ON             // 启用网页服务
#endif
#endif

// ESP32 蓝牙虚拟串口命令通道
#ifndef SERIAL_BT_MODE
#define SERIAL_BT_MODE                      OFF            // 设为 SLAVE 启用接口 仅支持 ESP32
#endif
#ifndef SERIAL_BT_NAME
#define SERIAL_BT_NAME                      HOST_NAME      // 命令通道的蓝牙名称
#endif
#ifndef SERIAL_BT_PASSKEY
#define SERIAL_BT_PASSKEY                   ""             // 蓝牙四位配对码
#endif

// 启用并配置 WiFi 功能
// 其他默认 IP 设置见以下文件
// 配置文件 src/lib/wifi/WifiManager.defaults.h

#ifndef MDNS_SERVER
#define MDNS_SERVER                         ON             // 启用 mDNS 服务
#endif

#ifndef MDNS_NAME
#define MDNS_NAME                           HOST_NAME      // mDNS 设备名称
#endif

// 将 Config.h 的 IP 设置转换为底层库设置
#ifndef SERIAL_IP_MODE
#define SERIAL_IP_MODE                      OFF            // 使用以下设置启用接口
#endif
#ifndef SERIAL_SERVER
#define SERIAL_SERVER                       BOTH           // STANDARD 使用端口 9999 PERSISTENT 使用端口 9996 至 9998
#endif

#if SERIAL_IP_MODE == ETHERNET_W5500
// 网络工作模式
#define OPERATIONAL_MODE                    ETHERNET_W5500
#elif SERIAL_IP_MODE == ETHERNET_W5100
// 网络工作模式
#define OPERATIONAL_MODE                    ETHERNET_W5100
#elif SERIAL_IP_MODE == WIFI_ACCESS_POINT
#define OPERATIONAL_MODE                    WIFI           // 网络工作模式
#define AP_ENABLED                          true           // 启用 WiFi 热点
#elif SERIAL_IP_MODE == WIFI_STATION
#define OPERATIONAL_MODE                    WIFI           // 网络工作模式
#define STA_ENABLED                         true           // 启用 WiFi 连接路由器模式
#elif SERIAL_IP_MODE == BOTH
#define OPERATIONAL_MODE                    WIFI           // 网络工作模式
#define AP_ENABLED                          true           // 启用 WiFi 热点
#define STA_ENABLED                         true           // 启用 WiFi 连接路由器模式
#endif

#if OPERATIONAL_MODE == WIFI
#ifndef NV_WIFI_SETTINGS
#define NV_WIFI_SETTINGS                                   // 允许将 WiFi 设置保存到非易失存储
#endif
#endif

#ifndef STA_AP_FALLBACK
#define STA_AP_FALLBACK                     true           // 连接路由器失败时启用热点
#endif

#ifndef STA_AUTO_RECONNECT
#define STA_AUTO_RECONNECT                  true           // 连接断开后自动重连
#endif

#ifndef TIME_IP_ADDR
#define TIME_IP_ADDR                        {129,6,15,28}  // 启用 NTP 时可使用以下服务器地址
#endif  // 示例 time-a-g.nist.gov 地址为 129,6,15,28 或 129,6,15,29 或 129,6,15,30

// CAN 默认设置
#ifndef CAN_PLUS
#define CAN_PLUS                            OFF            // 可选 CAN_SAN CAN_ESP32 CAN_MCP2515 CANn_TEENSY4
#endif
#ifndef CAN_BAUD
#define CAN_BAUD                            1000000        // 默认波特率 1000000
#endif
#ifndef CAN_SEND_RATE_MS
#define CAN_SEND_RATE_MS                    10             // CAN 消息发送处理频率 100 Hz
#endif
#ifndef CAN_RECV_RATE_MS
#define CAN_RECV_RATE_MS                    10             // CAN 消息接收处理频率 100 Hz
#endif
#ifndef CAN_RX_PIN
#define CAN_RX_PIN                          OFF            // ESP32 CAN 接口设置
#endif
#ifndef CAN_TX_PIN
#define CAN_TX_PIN                          OFF            // ESP32 CAN 接口设置
#endif
#ifndef CAN_CS_PIN
#define CAN_CS_PIN                          OFF            // MCP2515 SPI CAN 控制器设置
#endif
#ifndef CAN_INT_PIN
#define CAN_INT_PIN                         OFF            // MCP2515 SPI CAN 控制器设置
#endif

// 传感器设置
#ifndef WEATHER
#define WEATHER                             OFF            // BME280 I2C 0x77, BME280_0x76, BME280_SPI 片选引脚请查看引脚映射
#endif

// 步进信号
#ifndef STEP_WAVE_FORM
#define STEP_WAVE_FORM                      PULSE          // PULSE 高速运行时的步进信号波形 SQUARE 的信号完整性最佳
#endif

#if AXIS1_STEP_STATE == AXIS2_STEP_STATE == AXIS3_STEP_STATE == \
    AXIS4_STEP_STATE == AXIS5_STEP_STATE == AXIS6_STEP_STATE == \
    AXIS7_STEP_STATE == AXIS8_STEP_STATE == AXIS9_STEP_STATE == HIGH
#define DRIVER_STEP_DEFAULTS                               // 启用默认步进驱动配置
#endif

// GPIO 扩展设备
// DS2413 使用单总线扩展 2 路或 4 路 GPIO 对应一个或两个设备
// SWS 使用串口扩展 8 路 GPIO 通常使用 4 个空闲编码器引脚
// MCP23008 使用 I2C 扩展 8 路 GPIO
// MCP23017 X9555 或 X8575 使用 I2C 扩展 16 路 GPIO
// SSR74HC595 使用串行移位寄存器扩展最多 32 路 GPIO 仅支持输出
// 支持多数 OnStep 功能 通道引脚编号从 512 起依次分配
#ifndef GPIO_DEVICE
#define GPIO_DEVICE                         OFF            // GPIO 扩展设备型号
#endif

#ifndef FileVersionConfig
#warning "Configuration (Config.h): FileVersionConfig is undefined, assuming version 5."
#define FileVersionConfig                   5              // 配置文件版本号
#endif

// 驱动器共用使能引脚设置
#ifndef SHARED_ENABLE_STATE
#define SHARED_ENABLE_STATE                 LOW            // 电机通电时共用使能引脚的默认电平
#endif
#ifndef SHARED2_ENABLE_STATE
#define SHARED2_ENABLE_STATE                LOW            // 第二组共用使能引脚的有效电平
#endif
#ifndef SHARED3_ENABLE_STATE
#define SHARED3_ENABLE_STATE                LOW            // 第三组共用使能引脚的有效电平
#endif

// -----------------------------------------------------------------------------------
// 赤道仪设置

// 轴设置
#ifndef AXIS1_DRIVER_MODEL
#define AXIS1_DRIVER_MODEL                  OFF            // 指定驱动型号以启用
#endif
#ifndef AXIS1_STEPS_PER_DEGREE
#define AXIS1_STEPS_PER_DEGREE              12800.0        // 每度步数或微步数
#endif
#ifndef AXIS1_COUNTS_PER_DEGREE
// 每度编码器计数
#define AXIS1_COUNTS_PER_DEGREE             AXIS1_STEPS_PER_DEGREE
#endif
#ifndef AXIS1_REVERSE
#define AXIS1_REVERSE                       OFF            // 反转旋转方向
#endif
#ifndef AXIS1_POWER_DOWN
#define AXIS1_POWER_DOWN                    OFF            // 静止时关闭电机电源
#endif
#ifndef AXIS1_POWER_DOWN_TIME
#define AXIS1_POWER_DOWN_TIME               30000          // 断电延时 单位毫秒
#endif
#ifndef AXIS1_ENABLE_STATE
#define AXIS1_ENABLE_STATE                  LOW            // 电机通电时使能引脚的默认电平
#endif
#ifndef AXIS1_LIMIT_MIN
#define AXIS1_LIMIT_MIN                     -180           // 单位度
#endif
#ifndef AXIS1_LIMIT_MAX
#define AXIS1_LIMIT_MAX                     180            // 单位度
#endif
#ifndef AXIS1_LIMIT_SYNC
#define AXIS1_LIMIT_SYNC                    OFF            // 同步距离限制 单位度 OFF 为关闭
#endif
#ifndef AXIS1_SENSE_HOME
#define AXIS1_SENSE_HOME                    OFF            // 从正面看位于原点顺时针方向时的电平 HIGH 或 LOW
#endif
#ifndef AXIS1_SENSE_HOME_INIT
#define AXIS1_SENSE_HOME_INIT               INPUT_PULLUP   // 原点检测引脚模式
#endif
#ifndef AXIS1_SENSE_HOME_OFFSET
#define AXIS1_SENSE_HOME_OFFSET             0              // 传感器位置到原点的默认偏置 单位角秒
#endif
#ifndef AXIS1_SENSE_HOME_DIST_LIMIT
#define AXIS1_SENSE_HOME_DIST_LIMIT         180.0          // 最大距离 单位度
#endif
#ifndef AXIS1_SENSE_LIMIT_MIN
#define AXIS1_SENSE_LIMIT_MIN               LIMIT_SENSE    // 最小限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS1_SENSE_LIMIT_MAX
#define AXIS1_SENSE_LIMIT_MAX               LIMIT_SENSE    // 最大限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS1_SENSE_LIMIT_INIT
// 限位检测引脚模式
#define AXIS1_SENSE_LIMIT_INIT              LIMIT_SENSE_INIT
#endif

#if AXIS1_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS1_DRIVER_MODEL <= STEP_DIR_DRIVER_LAST
#define AXIS1_STEP_DIR_PRESENT                             // 轴 1 启用 STEP/DIR 驱动

  #ifndef AXIS1_STEP_STATE
#define AXIS1_STEP_STATE                    HIGH           // 步进信号的默认有效电平
  #endif
  #ifndef AXIS1_M2_ON_STATE
#define AXIS1_M2_ON_STATE                   HIGH           // M2 的默认有效电平
  #endif
  #ifndef AXIS1_DRIVER_MICROSTEPS
#define AXIS1_DRIVER_MICROSTEPS             OFF            // 正常运行微步模式
  #endif
  #ifndef AXIS1_DRIVER_MICROSTEPS_GOTO
#define AXIS1_DRIVER_MICROSTEPS_GOTO        OFF            // 快速移动时的微步模式
  #endif
  #ifndef AXIS1_DRIVER_INTPOL
#define AXIS1_DRIVER_INTPOL                 ON             // 支持时启用微步插值
  #endif
  #ifndef AXIS1_DRIVER_DECAY
#define AXIS1_DRIVER_DECAY                  OFF            // OFF 使用默认值 TMC 可选 STEALTHCHOP
  #endif
  #ifndef AXIS1_DRIVER_DECAY_GOTO
#define AXIS1_DRIVER_DECAY_GOTO             OFF            // OFF 使用默认值 TMC 可选 SPREADCYCLE
  #endif
  #ifndef AXIS1_DRIVER_IHOLD
#define AXIS1_DRIVER_IHOLD                  OFF            // 单位毫安
  #endif
  #ifndef AXIS1_DRIVER_IRUN
#define AXIS1_DRIVER_IRUN                   OFF            // 单位毫安
  #endif
  #ifndef AXIS1_DRIVER_IGOTO
#define AXIS1_DRIVER_IGOTO                  OFF            // 单位毫安
  #endif
  #ifndef AXIS1_DRIVER_STATUS
#define AXIS1_DRIVER_STATUS                 OFF            // 驱动状态检测 TMC SPI 用 ON 故障引脚用 HIGH 或 LOW
  #endif
#endif

#if AXIS1_DRIVER_MODEL >= SERVO_DRIVER_FIRST && AXIS1_DRIVER_MODEL <= SERVO_DRIVER_LAST
#define AXIS1_SERVO_PRESENT                                // 轴 1 启用 SERVO 驱动

  #ifndef AXIS1_SERVO_PH1_STATE
#define AXIS1_SERVO_PH1_STATE               LOW            // 电机驱动空闲电平 IN1 用于 SERVO_EE PHASE 用于 SERVO_PE
  #endif
  #ifndef AXIS1_SERVO_PH2_STATE
#define AXIS1_SERVO_PH2_STATE               LOW            // 电机驱动空闲电平 IN2 或 ENABLE PWM 引脚
  #endif

  #ifndef AXIS1_SERVO_ACCELERATION
#define AXIS1_SERVO_ACCELERATION            100            // 加速度限制 单位百分比每秒
  #endif
  #ifndef AXIS1_SERVO_VELOCITY_PWMTHRS
#define AXIS1_SERVO_VELOCITY_PWMTHRS        OFF            // 从 stealthChop 切换到 spreadCycle 的速度 单位步每秒
  #endif  // 应在两倍恒星速度时切换 仅适用于 TMC2209 和 TMC5160
  #ifndef AXIS1_SERVO_DC_PWR_MIN                          
#define AXIS1_SERVO_DC_PWR_MIN              0.0F           // 略低于轴开始运动时的最小功率 单位百分比
  #endif  // 近似值 仅适用于直流电机
  #ifndef AXIS1_SERVO_DC_PWR_MAX
#define AXIS1_SERVO_DC_PWR_MAX              100.0F         // 达到最快移动速度所需的功率 单位百分比
  #endif  // 近似值 仅适用于直流电机

  #ifndef AXIS1_SERVO_FEEDBACK
#define AXIS1_SERVO_FEEDBACK                DUAL_PID       // 反馈类型 DUAL_PID
  #endif
  #ifndef AXIS1_PID_SENSITIVITY
#define AXIS1_PID_SENSITIVITY               0              // 0 按快速移动状态切换 或指定启用第二组 PID 参数的功率百分比
  #endif
  #ifndef AXIS1_PID_P
#define AXIS1_PID_P                         0.0            // P 比例系数
  #endif
  #ifndef AXIS1_PID_I
#define AXIS1_PID_I                         0.0            // I 积分系数
  #endif
  #ifndef AXIS1_PID_D
#define AXIS1_PID_D                         0.0            // D 微分系数
  #endif
  #ifndef AXIS1_PID_P_GOTO
#define AXIS1_PID_P_GOTO                    AXIS1_PID_P    // P 比例系数
  #endif
  #ifndef AXIS1_PID_I_GOTO
#define AXIS1_PID_I_GOTO                    AXIS1_PID_I    // I 积分系数
  #endif
  #ifndef AXIS1_PID_D_GOTO
#define AXIS1_PID_D_GOTO                    AXIS1_PID_D    // D 微分系数
  #endif

  #ifndef AXIS1_SERVO_FLTR
#define AXIS1_SERVO_FLTR                    OFF            // 伺服编码器滤波 OFF 为关闭
  #endif
  #ifndef AXIS1_ENCODER
#define AXIS1_ENCODER                       AB             // 编码器类型 AB CW_CCW PULSE_DIR PULSE_ONLY SERIAL_BRIDGE
  #endif
  #ifndef AXIS1_ENCODER_ORIGIN
#define AXIS1_ENCODER_ORIGIN                0              // 绝对编码器原点偏置 支持正负值 使原点计数为 0
  #endif
  #ifndef AXIS1_ENCODER_REVERSE
#define AXIS1_ENCODER_REVERSE               OFF            // 反转编码器计数方向
  #endif
#endif

#if AXIS1_DRIVER_MODEL == ODRIVE
#define AXIS1_ODRIVE_PRESENT                               // 轴 1 启用 ODRIVE 驱动
  #ifndef AXIS1_ODRIVE_P
#define AXIS1_ODRIVE_P                      2.0            // P 比例系数
  #endif
  #ifndef AXIS1_ODRIVE_I
#define AXIS1_ODRIVE_I                      5.0            // I 积分系数
  #endif
  #ifndef AXIS1_ODRIVE_D
#define AXIS1_ODRIVE_D                      1.0            // D 微分系数
  #endif
#endif

#if AXIS1_DRIVER_MODEL == KTECH
#define AXIS1_KTECH_PRESENT                                // 轴 1 启用 KTECH 驱动
#endif

#if AXIS1_DRIVER_MODEL == MKS42D
#define AXIS1_MKS42D_PRESENT                               // 轴 1 启用 MKS42D 驱动
#endif

#ifndef AXIS2_DRIVER_MODEL
#define AXIS2_DRIVER_MODEL                  OFF            // 指定驱动器以启用
#endif
#ifndef AXIS2_STEPS_PER_DEGREE
#define AXIS2_STEPS_PER_DEGREE              12800.0        // 每度运动对应的步数或微步数
#endif
#ifndef AXIS2_COUNTS_PER_DEGREE
// 每度编码器计数
#define AXIS2_COUNTS_PER_DEGREE             AXIS2_STEPS_PER_DEGREE
#endif
#ifndef AXIS2_REVERSE
#define AXIS2_REVERSE                       OFF            // 反转运动方向
#endif
#ifndef AXIS2_POWER_DOWN
#define AXIS2_POWER_DOWN                    OFF            // 静止时关闭电机电源
#endif
#ifndef AXIS2_POWER_DOWN_TIME
#define AXIS2_POWER_DOWN_TIME               30000          // 断电延时 单位毫秒
#endif
#ifndef AXIS2_ENABLE_STATE
#define AXIS2_ENABLE_STATE                  LOW            // 步进驱动使能电平
#endif
#ifndef AXIS2_LIMIT_MIN
#define AXIS2_LIMIT_MIN                     -90            // 单位度
#endif
#ifndef AXIS2_LIMIT_MAX
#define AXIS2_LIMIT_MAX                     90             // 单位度
#endif
#ifndef AXIS2_LIMIT_SYNC
#define AXIS2_LIMIT_SYNC                    OFF            // 同步距离限制 单位度 OFF 为关闭
#endif
#ifndef AXIS2_SENSE_HOME
#define AXIS2_SENSE_HOME                    OFF            // 从上方看位于原点顺时针方向时的电平 HIGH 或 LOW
#endif
#ifndef AXIS2_SENSE_HOME_INIT
#define AXIS2_SENSE_HOME_INIT               INPUT_PULLUP   // 原点检测引脚模式
#endif
#ifndef AXIS2_SENSE_HOME_OFFSET
#define AXIS2_SENSE_HOME_OFFSET             0              // 传感器位置到原点的默认偏置 单位角秒
#endif
#ifndef AXIS2_SENSE_HOME_DIST_LIMIT
#define AXIS2_SENSE_HOME_DIST_LIMIT         180.0          // 最大距离 单位度
#endif
#ifndef AXIS2_SENSE_LIMIT_MIN
#define AXIS2_SENSE_LIMIT_MIN               LIMIT_SENSE    // 最小限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS2_SENSE_LIMIT_MAX
#define AXIS2_SENSE_LIMIT_MAX               LIMIT_SENSE    // 最大限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS2_SENSE_LIMIT_INIT
// 限位检测引脚模式
#define AXIS2_SENSE_LIMIT_INIT              LIMIT_SENSE_INIT
#endif

#if AXIS2_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS2_DRIVER_MODEL <= STEP_DIR_DRIVER_LAST
#define AXIS2_STEP_DIR_PRESENT                             // 轴 2 启用 STEP/DIR 驱动

  #ifndef AXIS2_STEP_STATE
#define AXIS2_STEP_STATE                    HIGH           // 步进信号的默认有效电平
  #endif
  #ifndef AXIS2_M2_ON_STATE
#define AXIS2_M2_ON_STATE                   HIGH           // M2 的默认有效电平
  #endif
  #ifndef AXIS2_DRIVER_MICROSTEPS
#define AXIS2_DRIVER_MICROSTEPS             OFF            // 正常运行微步模式
  #endif
  #ifndef AXIS2_DRIVER_MICROSTEPS_GOTO
#define AXIS2_DRIVER_MICROSTEPS_GOTO        OFF            // 快速移动时的微步模式
  #endif
  #ifndef AXIS2_DRIVER_INTPOL
#define AXIS2_DRIVER_INTPOL                 ON             // 支持时启用微步插值
  #endif
  #ifndef AXIS2_DRIVER_DECAY
#define AXIS2_DRIVER_DECAY                  OFF            // OFF 使用默认值 TMC 可选 STEALTHCHOP
  #endif
  #ifndef AXIS2_DRIVER_DECAY_GOTO
#define AXIS2_DRIVER_DECAY_GOTO             OFF            // OFF 使用默认值 TMC 可选 SPREADCYCLE
  #endif
  #ifndef AXIS2_DRIVER_IHOLD
#define AXIS2_DRIVER_IHOLD                  OFF            // 单位毫安
  #endif
  #ifndef AXIS2_DRIVER_IRUN
#define AXIS2_DRIVER_IRUN                   OFF            // 单位毫安
  #endif
  #ifndef AXIS2_DRIVER_IGOTO
#define AXIS2_DRIVER_IGOTO                  OFF            // 单位毫安
  #endif
  #ifndef AXIS2_DRIVER_STATUS
#define AXIS2_DRIVER_STATUS                 OFF            // 驱动状态检测 TMC SPI 用 ON 故障引脚用 HIGH 或 LOW
  #endif
#endif

#if AXIS2_DRIVER_MODEL >= SERVO_DRIVER_FIRST && AXIS2_DRIVER_MODEL <= SERVO_DRIVER_LAST
#define AXIS2_SERVO_PRESENT                                // 轴 2 启用 SERVO 驱动

  #ifndef AXIS2_SERVO_PH1_STATE
#define AXIS2_SERVO_PH1_STATE               LOW            // 电机驱动空闲电平 IN1 用于 SERVO_EE PHASE 用于 SERVO_PE
  #endif
  #ifndef AXIS2_SERVO_PH2_STATE
#define AXIS2_SERVO_PH2_STATE               LOW            // 电机驱动空闲电平 IN2 或 ENABLE PWM 引脚
  #endif

  #ifndef AXIS2_SERVO_ACCELERATION
#define AXIS2_SERVO_ACCELERATION            100            // 加速度限制 单位百分比每秒
  #endif
  #ifndef AXIS2_SERVO_VELOCITY_PWMTHRS
#define AXIS2_SERVO_VELOCITY_PWMTHRS        OFF            // 从 stealthChop 切换到 spreadCycle 的速度 单位步每秒
  #endif
  #ifndef AXIS2_SERVO_DC_PWR_MIN
#define AXIS2_SERVO_DC_PWR_MIN              0.0F           // 略低于轴开始运动时的最小功率 单位百分比
  #endif
  #ifndef AXIS2_SERVO_DC_PWR_MAX
#define AXIS2_SERVO_DC_PWR_MAX              100.0F         // 达到最快移动速度所需的功率 单位百分比
  #endif

  #ifndef AXIS2_SERVO_FEEDBACK
#define AXIS2_SERVO_FEEDBACK                DUAL_PID       // 反馈类型 DUAL_PID
  #endif
  #ifndef AXIS2_PID_SENSITIVITY
#define AXIS2_PID_SENSITIVITY               0              // 0 按快速移动状态切换 或指定启用第二组 PID 参数的功率百分比
  #endif
  #ifndef AXIS2_PID_P
#define AXIS2_PID_P                         0.0            // P 比例系数
  #endif
  #ifndef AXIS2_PID_I
#define AXIS2_PID_I                         0.0            // I 积分系数
  #endif
  #ifndef AXIS2_PID_D
#define AXIS2_PID_D                         0.0            // D 微分系数
  #endif
  #ifndef AXIS2_PID_P_GOTO
#define AXIS2_PID_P_GOTO                    AXIS2_PID_P    // P 比例系数
  #endif
  #ifndef AXIS2_PID_I_GOTO
#define AXIS2_PID_I_GOTO                    AXIS2_PID_I    // I 积分系数
  #endif
  #ifndef AXIS2_PID_D_GOTO
#define AXIS2_PID_D_GOTO                    AXIS2_PID_D    // D 微分系数
  #endif

  #ifndef AXIS2_SERVO_FLTR
#define AXIS2_SERVO_FLTR                    OFF            // 伺服编码器滤波 OFF 为关闭
  #endif
  #ifndef AXIS2_ENCODER
#define AXIS2_ENCODER                       AB             // 编码器类型 AB CW_CCW PULSE_DIR PULSE_ONLY SERIAL_BRIDGE
  #endif
  #ifndef AXIS2_ENCODER_ORIGIN
#define AXIS2_ENCODER_ORIGIN                0              // 绝对编码器原点偏置 支持正负值 使原点计数为 0
  #endif
  #ifndef AXIS2_ENCODER_REVERSE
#define AXIS2_ENCODER_REVERSE               OFF            // 反转编码器计数方向
  #endif
#endif

#if AXIS2_DRIVER_MODEL == ODRIVE
#define AXIS2_ODRIVE_PRESENT                               // 轴 2 启用 ODRIVE 驱动
  #ifndef AXIS2_ODRIVE_P
#define AXIS2_ODRIVE_P                      2.0            // P 比例系数
  #endif
  #ifndef AXIS2_ODRIVE_I
#define AXIS2_ODRIVE_I                      5.0            // I 积分系数
  #endif
  #ifndef AXIS2_ODRIVE_D
#define AXIS2_ODRIVE_D                      1.0            // D 微分系数
  #endif
#endif

#if AXIS2_DRIVER_MODEL == KTECH
#define AXIS2_KTECH_PRESENT                                // 轴 2 启用 KTECH 驱动
#endif

#if AXIS2_DRIVER_MODEL == MKS42D
#define AXIS2_MKS42D_PRESENT                               // 轴 2 启用 MKS42D 驱动
#endif

#if defined(AXIS1_SERVO_PRESENT) && \
    (AXIS1_ENCODER == AS37_H39B_B || AXIS1_ENCODER == JTW_24BIT || AXIS1_ENCODER == JTW_26BIT || AXIS1_ENCODER == LIKA_ASC85 || \
     (AXIS1_ENCODER == SERIAL_BRIDGE && SERIAL_ENCODER_ABSOLUTE == ON))
#define HAS_ABSOLUTE_ENCODER_SERVO_AXIS1                   // 轴 1 使用绝对编码器伺服驱动
#endif

#if defined(AXIS2_SERVO_PRESENT) && \
    (AXIS2_ENCODER == AS37_H39B_B || AXIS2_ENCODER == JTW_24BIT || AXIS2_ENCODER == JTW_26BIT || AXIS2_ENCODER == LIKA_ASC85 || \
     (AXIS2_ENCODER == SERIAL_BRIDGE && SERIAL_ENCODER_ABSOLUTE == ON))
#define HAS_ABSOLUTE_ENCODER_SERVO_AXIS2                   // 轴 2 使用绝对编码器伺服驱动
#endif

#if defined(HAS_ABSOLUTE_ENCODER_SERVO_AXIS1) && defined(HAS_ABSOLUTE_ENCODER_SERVO_AXIS2)
#define HAS_ABSOLUTE_ENCODER_SERVO                         // 存在使用绝对编码器的伺服轴
#endif

// 解析内部架台类型 切线臂和方位轴无限旋转设置
#ifndef MOUNT_TYPE
#define MOUNT_TYPE                          GEM            // GEM 德式赤道仪等需要执行中天翻转的架台
#endif
#if MOUNT_TYPE == GEM
#define MOUNT_SUBTYPE                       GEM            // 解析后的基础架台类型
#endif
#if MOUNT_TYPE == GEM_TA
#define MOUNT_SUBTYPE                       GEM            // 解析后的基础架台类型
#define AXIS2_TANGENT_ARM                   ON             // 赤纬轴采用行程受限的切线臂
#endif
#if MOUNT_TYPE == GEM_TAC
#define MOUNT_SUBTYPE                       GEM            // 解析后的基础架台类型
#define AXIS2_TANGENT_ARM                   ON             // 赤纬轴采用行程受限的切线臂
#define AXIS2_TANGENT_ARM_CORRECTION        ON             // 使用以下公式修正切线臂几何误差
#endif
#if MOUNT_TYPE == FORK
#define MOUNT_SUBTYPE                       FORK           // 解析后的基础架台类型
#endif
#if MOUNT_TYPE == FORK_TA
#define MOUNT_SUBTYPE                       FORK           // 解析后的基础架台类型
#define AXIS2_TANGENT_ARM                   ON             // 赤纬轴采用行程受限的切线臂
#endif
#if MOUNT_TYPE == FORK_TAC
#define MOUNT_SUBTYPE                       FORK           // 解析后的基础架台类型
#define AXIS2_TANGENT_ARM                   ON             // 赤纬轴采用行程受限的切线臂
#define AXIS2_TANGENT_ARM_CORRECTION        ON             // 使用以下公式修正切线臂几何误差
#endif
#if MOUNT_TYPE == ALTALT
#define MOUNT_SUBTYPE                       ALTALT         // 解析后的基础架台类型
#endif
#if MOUNT_TYPE == ALTAZM
#define MOUNT_SUBTYPE                       ALTAZM         // 解析后的基础架台类型
#endif
#if MOUNT_TYPE == ALTAZM_UNL
#define MOUNT_SUBTYPE                       ALTAZM         // 解析后的基础架台类型
#define AXIS1_WRAP                          ON             // 允许方位轴无限旋转
#endif

#ifndef MOUNT_SUBTYPE
#define MOUNT_SUBTYPE                       OFF            // 解析后的基础架台类型
#endif
#ifndef MOUNT_ALTERNATE_ORIENTATION
#define MOUNT_ALTERNATE_ORIENTATION         OFF            // 使用 ON 或 OFF 仅适用于 FORK 和 ALTAZM
#endif
#ifndef MOUNT_COORDS
#define MOUNT_COORDS                        TOPOCENTRIC    // 架台坐标系
#endif
#ifndef MOUNT_COORDS_MEMORY
#define MOUNT_COORDS_MEMORY                 OFF            // ON 启用架台位置记忆
#endif
// SA_STRICT 必须由有效来源确认坐标可信
// SA_AUTO 仅在未配置双轴绝对位置授权及坐标记忆时沿用传统启动位置假定
// SA_PERMISSIVE 始终信任开机位置
#ifndef MOUNT_STARTUP_MODE
#define MOUNT_STARTUP_MODE                  SA_AUTO        // 启动位置授权模式 默认 SA_AUTO
#endif
#ifndef NV_INIT_ERROR_REVOKES_AUTHORITY
#define NV_INIT_ERROR_REVOKES_AUTHORITY     ON             // ON 在非易失存储初始化或读取故障时撤销启动位置授权
#endif
#ifndef ST4_HOME_BUTTON
#define ST4_HOME_BUTTON                     OFF            // 赤经双键长按三秒回零
#endif

#ifndef HOME_FAST_RATE
#define HOME_FAST_RATE                      9              // 回零快速档位
#endif
#ifndef HOME_SLOW_RATE
#define HOME_SLOW_RATE                      OFF            // OFF 沿用逐级降速
#endif
#ifndef HOME_OFFSET_RATE
#define HOME_OFFSET_RATE                    9              // 回零偏置档位
#endif

#ifndef HOME_REQUIRED_ON_BOOT
#define HOME_REQUIRED_ON_BOOT               OFF            // 开机要求确认原点
#endif
#ifndef HOME_REQUIRED_AFTER_LIMIT
#define HOME_REQUIRED_AFTER_LIMIT           OFF            // 物理限位后要求重新回零
#endif
#ifndef MOTOR_HOLD_ON_BOOT
#define MOTOR_HOLD_ON_BOOT                  OFF            // 开机使能电机保持
#endif

#ifndef MOUNT_ENABLE_IN_STANDBY
#define MOUNT_ENABLE_IN_STANDBY             OFF            // ON 在待机时使能架台电机驱动
#endif
#ifndef MOUNT_AUTO_HOME_DEFAULT
#define MOUNT_AUTO_HOME_DEFAULT             OFF            // ON 默认在开机时自动寻找原点
#endif
#ifndef MOUNT_HOME_AT_OFFSETS
#define MOUNT_HOME_AT_OFFSETS               OFF            // ON 将运行时偏置计入传感器原点位置
#endif
#ifndef MOUNT_HORIZON_AVOIDANCE
#define MOUNT_HORIZON_AVOIDANCE             ON             // ON 允许赤道模式避开地平线
#endif

#ifndef AXIS1_TARGET_TOLERANCE
#define AXIS1_TARGET_TOLERANCE              0.0F           // GOTO 到位容差 单位角秒
#endif
#ifndef AXIS1_HOME_TOLERANCE  // 原点到位容差 单位角秒
// 轴 1 原点容差 含半个编码器计数对应的角度
#define AXIS1_HOME_TOLERANCE                AXIS1_TARGET_TOLERANCE + (1800.0/AXIS1_COUNTS_PER_DEGREE)
#endif
#ifndef AXIS1_SECTOR_GEAR
#define AXIS1_SECTOR_GEAR                   OFF            // 赤经轴采用行程受限的扇形齿轮
#endif
#ifndef AXIS1_WRAP
#define AXIS1_WRAP                          OFF            // 允许方位轴无限旋转
#endif

#ifndef AXIS2_TARGET_TOLERANCE
#define AXIS2_TARGET_TOLERANCE              0.0F           // 单位角秒
#endif
#ifndef AXIS2_HOME_TOLERANCE  // 单位角秒
// 轴 2 原点容差 含半个编码器计数对应的角度
#define AXIS2_HOME_TOLERANCE                AXIS2_TARGET_TOLERANCE + (1800.0/AXIS2_COUNTS_PER_DEGREE)
#endif
#ifndef AXIS2_TANGENT_ARM
#define AXIS2_TANGENT_ARM                   OFF            // 赤纬轴采用行程受限的切线臂
#endif
#ifndef AXIS2_TANGENT_ARM_CORRECTION
#define AXIS2_TANGENT_ARM_CORRECTION        OFF            // 使用以下公式修正切线臂几何误差
#endif
#ifndef TANGENT_ARM_INSTRUMENT_TO_MOUNT
#define TANGENT_ARM_INSTRUMENT_TO_MOUNT(a) (atan(a))  // 返回角度 单位弧度
#endif
#ifndef TANGENT_ARM_MOUNT_TO_INSTRUMENT
#define TANGENT_ARM_MOUNT_TO_INSTRUMENT(a) (tan(a))  // 角度 a 单位弧度
#endif

// 状态提示
#ifndef STATUS_LED
#define STATUS_LED                          OFF            // 无错误时常亮 否则按错误代码闪烁
#endif
#ifndef STATUS_LED_ON_STATE
#define STATUS_LED_ON_STATE                 LOW            // 状态灯有效电平
#endif
#ifndef STATUS_MOUNT_LED
#define STATUS_MOUNT_LED                    OFF            // ON 按运动速率成比例闪烁 转动时常亮
#endif
#ifndef MOUNT_LED_ON_STATE
#define MOUNT_LED_ON_STATE                  LOW            // 架台状态灯有效电平
#endif
#ifndef STATUS_BUZZER
#define STATUS_BUZZER                       OFF            // ON, n 其中 n=100..6000 为扬声器频率 单位为 Hz ON 用于压电蜂鸣器
#endif
#ifndef STATUS_BUZZER_ON_STATE
#define STATUS_BUZZER_ON_STATE              HIGH           // 蜂鸣器有效电平
#endif
#ifndef STATUS_BUZZER_DEFAULT
#define STATUS_BUZZER_DEFAULT               OFF            // ON 启动时启用蜂鸣器声音
#endif
#ifndef STATUS_BUZZER_MEMORY
#define STATUS_BUZZER_MEMORY                OFF            // ON 断电后保留蜂鸣器声音设置
#endif

// 分划板照明
#ifndef RETICLE_LED_DEFAULT
#define RETICLE_LED_DEFAULT                 OFF            // 其中 n=0..255 对应 0..100% 启用功能并设置默认亮度
#endif
#ifndef RETICLE_LED_MEMORY
#define RETICLE_LED_MEMORY                  OFF            // ON 断电后保留分划板亮度设置
#endif
#ifndef RETICLE_LED_INVERT
#define RETICLE_LED_INVERT                  OFF            // ON 反转控制逻辑 适用于 0V 表示最大亮度的情况
#endif

// 时间与位置
#ifndef TIME_LOCATION_SOURCE
#define TIME_LOCATION_SOURCE                OFF            // 指定日期时间来源 可同时提供位置
#endif
#ifndef TIME_LOCATION_SOURCE_FALLBACK  // 备用时间位置来源 须与主来源不同 且不能使用 GPS 或 NTP
#define TIME_LOCATION_SOURCE_FALLBACK       OFF            // 备用时间位置来源
#endif
#ifndef TIME_LOCATION_PPS_SENSE
#define TIME_LOCATION_PPS_SENSE             OFF            // HIGH 检测 PPS 每秒脉冲信号上升沿 LOW 检测下降沿
#endif
#ifndef TIME_LOCATION_PPS_SYNC
#define TIME_LOCATION_PPS_SYNC              OFF            // 根据 PPS 调整定时器速率以保持时间同步
#endif

// 限位设置
#ifndef LIMIT_SENSE
#define LIMIT_SENSE                         OFF            // 用于轴 1 和轴 2
#endif
#ifndef LIMIT_SENSE_INIT
#define LIMIT_SENSE_INIT                    INPUT_PULLUP   // 用于轴 1 和轴 2
#endif
#ifndef LIMIT_SENSE_STRICT
#define LIMIT_SENSE_STRICT                  OFF            // 适用于所有轴 ON 在最小和最大限位共用引脚时阻止运动
#endif
#ifndef LIMIT_RECOVERY
#define LIMIT_RECOVERY                      OFF            // ON 允许用 1 秒 GOTO 脱离地平线 中天及轴 1 行程限位
#endif
#ifndef LIMIT_RECOVERY_WITH_TRACKING
#define LIMIT_RECOVERY_WITH_TRACKING        OFF            // ON 在限位恢复后自动开启跟踪
#endif

// ST4 接口
#ifndef ST4_INTERFACE
#define ST4_INTERFACE                       OFF            // ON 启用接口 非手控模式下导星速率不超过 1 倍速
#endif
#ifndef ST4_INTERFACE_INIT
#define ST4_INTERFACE_INIT                  INPUT_PULLUP   // ST4 使用板上外部上拉
#endif
#ifndef ST4_HAND_CONTROL
#define ST4_HAND_CONTROL                    OFF            // ON 启用手控器特殊功能和 SHC 支持
#endif
#ifndef ST4_HAND_CONTROL_FOCUSER
#define ST4_HAND_CONTROL_FOCUSER            OFF            // ON 使用上述功能的替代模式 调焦器移动 [E]f1 [W]f2 [N]- [S]+
#endif

// 泊车设置
#ifndef PARK_SENSE
#define PARK_SENSE                          OFF            // HIGH 或 LOW 状态表示架台处于停放方向
#endif
#ifndef PARK_SENSE_INIT
#define PARK_SENSE_INIT                     INPUT_PULLUP   // 泊车检测引脚模式
#endif

#ifndef PARK_SIGNAL
#define PARK_SIGNAL                         OFF            // HIGH 或 LOW 状态的停放输入信号触发停放
#endif
#ifndef PARK_SIGNAL_INIT
#define PARK_SIGNAL_INIT                    INPUT_PULLUP   // 泊车信号引脚模式
#endif

#ifndef PARK_STATUS
#define PARK_STATUS                         OFF            // 成功停放后以 HIGH 或 LOW 状态输出信号
#endif

// 周期误差修正
#ifndef PEC_STEPS_PER_WORM_ROTATION
#define PEC_STEPS_PER_WORM_ROTATION         0              // 蜗杆每转的步数 0 表示禁用 其他值会分配 720 秒缓冲区
#endif
#ifndef PEC_BUFFER_SIZE_LIMIT
#define PEC_BUFFER_SIZE_LIMIT               720            // PEC 缓冲区固定最大容量
#endif
#ifndef PEC_SENSE
#define PEC_SENSE                           OFF            // HIGH 检测 PEC 信号上升沿 LOW 检测下降沿
#endif
#ifndef PEC_SENSE_INIT
#define PEC_SENSE_INIT                      INPUT_PULLUP   // 周期误差检测引脚模式
#endif

// 导星设置
#ifndef GUIDE_TIME_LIMIT
#define GUIDE_TIME_LIMIT                    0              // 单位秒 0 为禁用
#endif
#ifndef GUIDE_TIME_NO_LIMITS
#define GUIDE_TIME_NO_LIMITS                5              // 软件限位关闭时的最长导星时间 单位秒 0 为禁用
#endif
#ifndef GUIDE_DISABLE_BACKLASH
#define GUIDE_DISABLE_BACKLASH              OFF            // 脉冲导星时禁用回差补偿
#endif
#ifndef GUIDE_SEPARATE_PULSE_RATE
#define GUIDE_SEPARATE_PULSE_RATE           ON             // 通常始终启用
#endif

// 跟踪设置
#ifndef TRACK_AUTOSTART
#define TRACK_AUTOSTART                     OFF            // 开机时自动开始跟踪
#endif
#ifndef TRACK_WITHOUT_LIMITS
#define TRACK_WITHOUT_LIMITS                OFF            // 允许在限位关闭时跟踪
#endif
#ifndef TRACK_COMPENSATION_DEFAULT
#define TRACK_COMPENSATION_DEFAULT          OFF            // 可选 OFF REFRACTION REFRACTION_DUAL MODEL MODEL_DUAL
#endif
#ifndef TRACK_COMPENSATION_MEMORY
#define TRACK_COMPENSATION_MEMORY           OFF            // 记忆上次运行时的跟踪补偿设置
#endif
#ifndef TRACK_BACKLASH_RATE
#define TRACK_BACKLASH_RATE                 25             // 回差补偿速度 单位恒星速度倍数
#endif  // 不使用加速 速度须在步进电机扭矩允许范围内
#ifndef TRACKING_RATE_DEFAULT_HZ
// 正常恒星跟踪速率
#define TRACKING_RATE_DEFAULT_HZ            SIDEREAL_RATE_HZ
#endif

// 快速移动设置
#ifndef GOTO_FEATURE
#define GOTO_FEATURE                        ON             // OFF 禁用 GOTO 功能
#endif
#ifndef SLEW_RATE_BASE_DESIRED
#define SLEW_RATE_BASE_DESIRED              1.0            // 期望的最大移动速度 实际速度受多种因素影响
#endif
#ifndef SLEW_RATE_MEMORY
#define SLEW_RATE_MEMORY                    OFF            // ON 断电后保留运行时移动速度设置
#endif
#ifndef AXIS2_SLEW_RATE_PERCENT
#define AXIS2_SLEW_RATE_PERCENT             100.0F         // 允许轴 2 最大移动速度低于轴 1
#endif
#ifndef SLEW_ACCELERATION_DIST
#define SLEW_ACCELERATION_DIST              5.0            // 完成加减速所需距离 单位度
#endif
#ifndef SLEW_RAPID_STOP_DIST
#define SLEW_RAPID_STOP_DIST                2.0            // 紧急停止距离 单位度
#endif
#ifndef GOTO_OFFSET
#define GOTO_OFFSET                         0.25           // GOTO 单向接近目标的距离 单位度 0.0 为禁用
#endif
#ifndef GOTO_OFFSET_ALIGN
#define GOTO_OFFSET_ALIGN                   OFF            // 校准星 GOTO 跳过最终接近阶段
#endif  // 便于用户从正确方向将星点居中
#ifndef GOTO_SETTLE_TIME
#define GOTO_SETTLE_TIME                    1500           // GOTO 偏置最终阶段的稳定时间 单位毫秒
#endif  // 用于等待稳定 并在支持时同步编码器
#ifndef GOTO_REFINE_STAGES
#define GOTO_REFINE_STAGES                  1              // GOTO 精调阶段的执行次数
#endif

// 中天翻转与镜筒侧向
#ifndef MFLIP_HOME_DEFAULT
  #if defined(MFLIP_PAUSE_HOME_DEFAULT) && MFLIP_PAUSE_HOME_DEFAULT == ON
#define MFLIP_HOME_DEFAULT                  PAUSE          // OFF, VISIT, PAUSE 选择中天翻转时的原点路径点行为
  #elif defined(MFLIP_VISIT_HOME_DEFAULT)
    #if MFLIP_VISIT_HOME_DEFAULT == ON
#define MFLIP_HOME_DEFAULT                  VISIT          // OFF, VISIT, PAUSE 选择中天翻转时的原点路径点行为
    #else
#define MFLIP_HOME_DEFAULT                  OFF            // OFF, VISIT, PAUSE 选择中天翻转时的原点路径点行为
    #endif
  #elif defined(MFLIP_SKIP_HOME)
    #if MFLIP_SKIP_HOME == ON
#define MFLIP_HOME_DEFAULT                  OFF            // OFF, VISIT, PAUSE 选择中天翻转时的原点路径点行为
    #else
#define MFLIP_HOME_DEFAULT                  VISIT          // OFF, VISIT, PAUSE 选择中天翻转时的原点路径点行为
    #endif
  #else
#define MFLIP_HOME_DEFAULT                  VISIT          // OFF, VISIT, PAUSE 选择中天翻转时的原点路径点行为
  #endif
#endif
#ifndef MFLIP_HOME_MEMORY
  #if defined(MFLIP_PAUSE_HOME_MEMORY) && MFLIP_PAUSE_HOME_MEMORY == ON
#define MFLIP_HOME_MEMORY                   ON             // ON 断电后保留中天翻转原点模式设置
  #elif defined(MFLIP_VISIT_HOME_MEMORY)
// ON 断电后保留中天翻转原点模式设置
#define MFLIP_HOME_MEMORY                   MFLIP_VISIT_HOME_MEMORY
  #else
#define MFLIP_HOME_MEMORY                   OFF            // ON 断电后保留中天翻转原点模式设置
  #endif
#endif
#ifndef MFLIP_AUTOMATIC_DEFAULT
#define MFLIP_AUTOMATIC_DEFAULT             OFF            // ON 启动时启用自动中天翻转
#endif
#ifndef MFLIP_AUTOMATIC_MEMORY
#define MFLIP_AUTOMATIC_MEMORY              OFF            // ON 断电后保留自动中天翻转设置
#endif
#ifndef PIER_SIDE_SYNC_CHANGE_SIDES
#define PIER_SIDE_SYNC_CHANGE_SIDES         OFF            // ON 允许 GEM 架台在同步时改变基座侧
#endif
#ifndef PIER_SIDE_PREFERRED_DEFAULT
#define PIER_SIDE_PREFERRED_DEFAULT         BEST           // BEST 尽可能保持当前侧 EAST 或 WEST 则尽可能切换到指定侧
#endif
#ifndef PIER_SIDE_PREFERRED_MEMORY
#define PIER_SIDE_PREFERRED_MEMORY          OFF            // ON 断电后保留首选基座侧设置
#endif

// 星点校准
#ifndef ALIGN_MAX_STARS
#define ALIGN_MAX_STARS                     AUTO           // 最大校准星数 AUTO 使用硬件抽象层默认值
#endif

#ifndef ALIGN_AUTO_HOME
#define ALIGN_AUTO_HOME                     OFF            // 开始校准前使用原点开关回零
#endif

#ifndef ALIGN_MODEL_MEMORY
#define ALIGN_MODEL_MEMORY                  OFF            // 开机时恢复非易失存储中的指向模型
#endif

#define HIGH_SPEED_ALIGN                                   // 启用高速指向模型计算

// -----------------------------------------------------------------------------------
// 旋转器设置 ROTATOR
#ifndef AXIS3_DRIVER_MODEL
#define AXIS3_DRIVER_MODEL                  OFF            // 指定驱动器以启用
#endif
#ifndef AXIS3_STEPS_PER_DEGREE
#define AXIS3_STEPS_PER_DEGREE              64.0           // 每度运动对应的步数或微步数
#endif
#ifndef AXIS3_COUNTS_PER_DEGREE
// 每度编码器计数
#define AXIS3_COUNTS_PER_DEGREE             AXIS3_STEPS_PER_DEGREE
#endif
#ifndef AXIS3_REVERSE
#define AXIS3_REVERSE                       OFF            // 反转运动方向
#endif
#ifndef AXIS3_POWER_DOWN
#define AXIS3_POWER_DOWN                    OFF            // 静止时关闭电机电源
#endif
#ifndef AXIS3_POWER_DOWN_TIME
#define AXIS3_POWER_DOWN_TIME               30000          // 断电延时 单位毫秒
#endif
#ifndef AXIS3_ENABLE_STATE
#define AXIS3_ENABLE_STATE                  LOW            // 步进驱动使能电平
#endif
#ifndef AXIS3_SLEW_RATE_BASE_DESIRED
#define AXIS3_SLEW_RATE_BASE_DESIRED        3.0            // 单位度每秒
#endif
#ifndef AXIS3_ACCELERATION_TIME
#define AXIS3_ACCELERATION_TIME             1.0            // 加速至选定速度所需时间 单位秒
#endif
#ifndef AXIS3_RAPID_STOP_TIME
#define AXIS3_RAPID_STOP_TIME               1.0            // 停止所需时间 单位秒
#endif
#ifndef AXIS3_BACKLASH_RATE
// 单位度每秒
#define AXIS3_BACKLASH_RATE                 (AXIS3_SLEW_RATE_BASE_DESIRED/4)
#endif
#ifndef AXIS3_LIMIT_MIN
#define AXIS3_LIMIT_MIN                     -180           // 单位度
#endif
#ifndef AXIS3_LIMIT_MAX
#define AXIS3_LIMIT_MAX                     180            // 单位度
#endif
#ifndef AXIS3_SENSE_HOME
#define AXIS3_SENSE_HOME                    OFF            // 从正面看位于原点顺时针方向时的电平 HIGH 或 LOW
#endif
#ifndef AXIS3_SENSE_HOME_INIT
#define AXIS3_SENSE_HOME_INIT               INPUT_PULLUP   // 原点检测引脚模式
#endif
#ifndef AXIS3_SENSE_HOME_DIST_LIMIT
#define AXIS3_SENSE_HOME_DIST_LIMIT         180.0          // 原点搜索最大距离 单位度
#endif
#ifndef AXIS3_SENSE_LIMIT_MIN
#define AXIS3_SENSE_LIMIT_MIN               OFF            // 最小限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS3_SENSE_LIMIT_MAX
#define AXIS3_SENSE_LIMIT_MAX               OFF            // 最大限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS3_SENSE_LIMIT_INIT
#define AXIS3_SENSE_LIMIT_INIT              INPUT_PULLUP   // 限位检测引脚模式
#endif
#if AXIS3_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS3_DRIVER_MODEL <= STEP_DIR_DRIVER_LAST
#define AXIS3_STEP_DIR_PRESENT                             // 轴 3 启用 STEP/DIR 驱动

  #ifndef AXIS3_STEP_STATE
#define AXIS3_STEP_STATE                    HIGH           // 步进信号的默认有效电平
  #endif
  #ifndef AXIS3_M2_ON_STATE
#define AXIS3_M2_ON_STATE                   HIGH           // M2 的默认有效电平
  #endif
  #ifndef AXIS3_DRIVER_MICROSTEPS
#define AXIS3_DRIVER_MICROSTEPS             OFF            // 正常运行微步模式
  #endif
  #ifndef AXIS3_DRIVER_MICROSTEPS_GOTO
#define AXIS3_DRIVER_MICROSTEPS_GOTO        OFF            // 快速移动时的微步模式
  #endif
  #ifndef AXIS3_DRIVER_INTPOL
#define AXIS3_DRIVER_INTPOL                 ON             // 支持时启用微步插值
  #endif
  #ifndef AXIS3_DRIVER_DECAY
#define AXIS3_DRIVER_DECAY                  OFF            // OFF 使用默认值 TMC 可选 STEALTHCHOP
  #endif
  #ifndef AXIS3_DRIVER_DECAY_GOTO
#define AXIS3_DRIVER_DECAY_GOTO             OFF            // OFF 使用默认值 TMC 可选 SPREADCYCLE
  #endif
  #ifndef AXIS3_DRIVER_IHOLD
#define AXIS3_DRIVER_IHOLD                  OFF            // 单位毫安
  #endif
  #ifndef AXIS3_DRIVER_IRUN
#define AXIS3_DRIVER_IRUN                   OFF            // 单位毫安
  #endif
  #ifndef AXIS3_DRIVER_IGOTO
#define AXIS3_DRIVER_IGOTO                  OFF            // 单位毫安
  #endif
  #ifndef AXIS3_DRIVER_STATUS
#define AXIS3_DRIVER_STATUS                 OFF            // 驱动状态检测 TMC SPI 用 ON 故障引脚用 HIGH 或 LOW
  #endif
#endif

#if AXIS3_DRIVER_MODEL >= SERVO_DRIVER_FIRST && AXIS2_DRIVER_MODEL <= SERVO_DRIVER_LAST
#define AXIS3_SERVO_PRESENT                                // 轴 3 启用 SERVO 驱动

  #ifndef AXIS3_SERVO_PH1_STATE
#define AXIS3_SERVO_PH1_STATE               LOW            // 电机驱动空闲电平 IN1 用于 SERVO_EE PHASE 用于 SERVO_PE
  #endif
  #ifndef AXIS3_SERVO_PH2_STATE
#define AXIS3_SERVO_PH2_STATE               LOW            // 电机驱动空闲电平 IN2 或 ENABLE PWM 引脚
  #endif

  #ifndef AXIS3_SERVO_ACCELERATION
#define AXIS3_SERVO_ACCELERATION            100            // 加速度限制 单位百分比每秒
  #endif
  #ifndef AXIS3_SERVO_VELOCITY_PWMTHRS
#define AXIS3_SERVO_VELOCITY_PWMTHRS        OFF            // 从 stealthChop 切换到 spreadCycle 的速度 单位步每秒
  #endif                            
  #ifndef AXIS3_SERVO_DC_PWR_MIN
#define AXIS3_SERVO_DC_PWR_MIN              0.0F           // 略低于轴开始运动时的最小功率 单位百分比
  #endif
  #ifndef AXIS3_SERVO_DC_PWR_MAX
#define AXIS3_SERVO_DC_PWR_MAX              100.0F         // 达到最快移动速度所需的功率 单位百分比
  #endif

  #ifndef AXIS3_SERVO_FEEDBACK
#define AXIS3_SERVO_FEEDBACK                PID            // 反馈类型 DUAL_PID
  #endif
  #ifndef AXIS3_PID_P
#define AXIS3_PID_P                         0.0            // P 比例系数
  #endif
  #ifndef AXIS3_PID_I
#define AXIS3_PID_I                         0.0            // I 积分系数
  #endif
  #ifndef AXIS3_PID_D
#define AXIS3_PID_D                         0.0            // D 微分系数
  #endif

  #ifndef AXIS3_SERVO_FLTR
#define AXIS3_SERVO_FLTR                    OFF            // 伺服编码器滤波 OFF 为关闭
  #endif
  #ifndef AXIS3_ENCODER
#define AXIS3_ENCODER                       AB             // 编码器类型 AB CW_CCW PULSE_DIR PULSE_ONLY SERIAL_BRIDGE
  #endif
  #ifndef AXIS3_ENCODER_ORIGIN
#define AXIS3_ENCODER_ORIGIN                0              // 绝对编码器原点偏置 支持正负值 使原点计数为 0
  #endif
  #ifndef AXIS3_ENCODER_REVERSE
#define AXIS3_ENCODER_REVERSE               OFF            // 反转编码器计数方向
  #endif
#endif

#if AXIS3_DRIVER_MODEL == KTECH
#define AXIS3_KTECH_PRESENT                                // 轴 3 启用 KTECH 驱动
#endif

#if AXIS3_DRIVER_MODEL == MKS42D
#define AXIS3_MKS42D_PRESENT                               // 轴 3 启用 MKS42D 驱动
#endif

// -----------------------------------------------------------------------------------
// 所有调焦器的共用设置

#ifndef FOCUSER_TEMPERATURE
#define FOCUSER_TEMPERATURE                 OFF            // 启用并设置调焦温度传感器 如 DS18B20 或 THERMISTOR
#endif
#ifndef FOCUSER_BUTTON_SENSE_OUT
#define FOCUSER_BUTTON_SENSE_OUT            OFF            // 调焦器向外移动
#endif
#ifndef FOCUSER_BUTTON_SENSE_IN
#define FOCUSER_BUTTON_SENSE_IN             OFF            // 调焦器向内移动
#endif
#ifndef FOCUSER_BUTTON_SENSE_INIT
#define FOCUSER_BUTTON_SENSE_INIT           INPUT_PULLUP   // 调焦按钮引脚模式
#endif
#ifndef FOCUSER_BUTTON_FOCUSER_INDEX
#define FOCUSER_BUTTON_FOCUSER_INDEX        1              // 按钮控制的调焦器编号 1 至 6
#endif
#ifndef FOCUSER_BUTTON_MOVE_RATE
#define FOCUSER_BUTTON_MOVE_RATE            0              // 按钮移动速度 0 使用上次设置 或指定固定速度 单位微米每秒
#endif

// -----------------------------------------------------------------------------------
// 调焦器 1 设置 FOCUSER1
#ifndef AXIS4_DRIVER_MODEL
#define AXIS4_DRIVER_MODEL                  OFF            // 指定驱动器以启用
#endif
#ifndef AXIS4_STEPS_PER_MICRON
#define AXIS4_STEPS_PER_MICRON              0.5            // 每微米运动对应的步数或微步数
#endif
#ifndef AXIS4_COUNTS_PER_MICRON
// 每微米编码器计数
#define AXIS4_COUNTS_PER_MICRON             AXIS4_STEPS_PER_MICRON
#endif
#ifndef AXIS4_REVERSE
#define AXIS4_REVERSE                       OFF            // 反转运动方向
#endif
#ifndef AXIS4_POWER_DOWN
#define AXIS4_POWER_DOWN                    OFF            // 静止时关闭电机电源
#endif
#ifndef AXIS4_POWER_DOWN_TIME
#define AXIS4_POWER_DOWN_TIME               30000          // 断电延时 单位毫秒
#endif
#ifndef AXIS4_SLAVED_TO_FOCUSER
#define AXIS4_SLAVED_TO_FOCUSER             0              // 跟随的主调焦器编号 0 为禁用
#endif
#ifndef AXIS4_ENABLE_STATE
#define AXIS4_ENABLE_STATE                  LOW            // 驱动器工作时的使能电平
#endif
#ifndef AXIS4_SLEW_RATE_MINIMUM
#define AXIS4_SLEW_RATE_MINIMUM             20             // 单位微米每秒
#endif
#ifndef AXIS4_SLEW_RATE_BASE_DESIRED
#define AXIS4_SLEW_RATE_BASE_DESIRED        500            // 单位微米每秒
#endif
#ifndef AXIS4_ACCELERATION_TIME
#define AXIS4_ACCELERATION_TIME             1.0            // 加速至选定速度所需时间 单位秒
#endif
#ifndef AXIS4_RAPID_STOP_TIME
#define AXIS4_RAPID_STOP_TIME               1.0            // 停止所需时间 单位秒
#endif
#ifndef AXIS4_BACKLASH_RATE
// 单位微米每秒
#define AXIS4_BACKLASH_RATE                 (AXIS4_SLEW_RATE_BASE_DESIRED/4)
#endif
#ifndef AXIS4_LIMIT_MIN
#define AXIS4_LIMIT_MIN                     0              // 单位毫米
#endif
#ifndef AXIS4_LIMIT_MAX
#define AXIS4_LIMIT_MAX                     50             // 单位毫米
#endif
#ifndef AXIS4_HOME_DEFAULT
#define AXIS4_HOME_DEFAULT                  MIDDLE         // 可选 MINIMUM 零位 MIDDLE 半行程 MAXIMUM 全行程 或指定微米位置
#endif
#ifndef AXIS4_SENSE_HOME
#define AXIS4_SENSE_HOME                    OFF            // 从正面看位于原点顺时针方向时的电平 HIGH 或 LOW
#endif
#ifndef AXIS4_SENSE_HOME_INIT
#define AXIS4_SENSE_HOME_INIT               INPUT_PULLUP   // 原点检测引脚模式
#endif
#ifndef AXIS4_SENSE_HOME_DIST_LIMIT
#define AXIS4_SENSE_HOME_DIST_LIMIT         50             // 原点搜索最大距离 单位毫米
#endif
#ifndef AXIS4_SENSE_LIMIT_MIN
#define AXIS4_SENSE_LIMIT_MIN               OFF            // 最小限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS4_SENSE_LIMIT_MAX
#define AXIS4_SENSE_LIMIT_MAX               OFF            // 最大限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS4_SENSE_LIMIT_INIT
#define AXIS4_SENSE_LIMIT_INIT              INPUT_PULLUP   // 限位检测引脚模式
#endif

#if AXIS4_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS4_DRIVER_MODEL <= STEP_DIR_DRIVER_LAST
#define AXIS4_STEP_DIR_PRESENT                             // 轴 4 启用 STEP/DIR 驱动

  #ifndef AXIS4_STEP_STATE
#define AXIS4_STEP_STATE                    HIGH           // 步进信号的默认有效电平
  #endif
  #ifndef AXIS4_M2_ON_STATE
#define AXIS4_M2_ON_STATE                   HIGH           // M2 的默认有效电平
  #endif
  #ifndef AXIS4_DRIVER_MICROSTEPS
#define AXIS4_DRIVER_MICROSTEPS             OFF            // 正常运行微步模式
  #endif
  #ifndef AXIS4_DRIVER_MICROSTEPS_GOTO
#define AXIS4_DRIVER_MICROSTEPS_GOTO        OFF            // 快速移动时的微步模式
  #endif
  #ifndef AXIS4_DRIVER_INTPOL
#define AXIS4_DRIVER_INTPOL                 ON             // 支持时启用微步插值
  #endif
  #ifndef AXIS4_DRIVER_DECAY
#define AXIS4_DRIVER_DECAY                  OFF            // OFF 使用默认值 TMC 可选 STEALTHCHOP
  #endif
  #ifndef AXIS4_DRIVER_DECAY_GOTO
#define AXIS4_DRIVER_DECAY_GOTO             OFF            // OFF 使用默认值 TMC 可选 SPREADCYCLE
  #endif
  #ifndef AXIS4_DRIVER_IHOLD
#define AXIS4_DRIVER_IHOLD                  OFF            // 单位毫安
  #endif
  #ifndef AXIS4_DRIVER_IRUN
#define AXIS4_DRIVER_IRUN                   OFF            // 单位毫安
  #endif
  #ifndef AXIS4_DRIVER_IGOTO
#define AXIS4_DRIVER_IGOTO                  OFF            // 单位毫安
  #endif
  #ifndef AXIS4_DRIVER_STATUS
#define AXIS4_DRIVER_STATUS                 OFF            // 驱动状态检测 TMC SPI 用 ON 故障引脚用 HIGH 或 LOW
  #endif
#endif

#if AXIS4_DRIVER_MODEL >= SERVO_DRIVER_FIRST && AXIS2_DRIVER_MODEL <= SERVO_DRIVER_LAST
#define AXIS4_SERVO_PRESENT                                // 轴 4 启用 SERVO 驱动

  #ifndef AXIS4_SERVO_PH1_STATE
#define AXIS4_SERVO_PH1_STATE               LOW            // 电机驱动空闲电平 IN1 用于 SERVO_EE PHASE 用于 SERVO_PE
  #endif
  #ifndef AXIS4_SERVO_PH2_STATE
#define AXIS4_SERVO_PH2_STATE               LOW            // 电机驱动空闲电平 IN2 或 ENABLE PWM 引脚
  #endif

  #ifndef AXIS4_SERVO_ACCELERATION
#define AXIS4_SERVO_ACCELERATION            100            // 加速度限制 单位百分比每秒
  #endif
  #ifndef AXIS4_SERVO_VELOCITY_PWMTHRS
#define AXIS4_SERVO_VELOCITY_PWMTHRS        OFF            // 从 stealthChop 切换到 spreadCycle 的速度 单位步每秒
  #endif                            
  #ifndef AXIS4_SERVO_DC_PWR_MIN
#define AXIS4_SERVO_DC_PWR_MIN              0.0F           // 略低于轴开始运动时的最小功率 单位百分比
  #endif
  #ifndef AXIS4_SERVO_DC_PWR_MAX
#define AXIS4_SERVO_DC_PWR_MAX              100.0F         // 达到最快移动速度所需的功率 单位百分比
  #endif

  #ifndef AXIS4_SERVO_FEEDBACK
#define AXIS4_SERVO_FEEDBACK                PID            // 反馈类型 DUAL_PID
  #endif
  #ifndef AXIS4_PID_P
#define AXIS4_PID_P                         0.0            // P 比例系数
  #endif
  #ifndef AXIS4_PID_I
#define AXIS4_PID_I                         0.0            // I 积分系数
  #endif
  #ifndef AXIS4_PID_D
#define AXIS4_PID_D                         0.0            // D 微分系数
  #endif

  #ifndef AXIS4_SERVO_FLTR
#define AXIS4_SERVO_FLTR                    OFF            // 伺服编码器滤波 OFF 为关闭
  #endif
  #ifndef AXIS4_ENCODER
#define AXIS4_ENCODER                       AB             // 编码器类型 AB CW_CCW PULSE_DIR PULSE_ONLY SERIAL_BRIDGE
  #endif
  #ifndef AXIS4_ENCODER_ORIGIN
#define AXIS4_ENCODER_ORIGIN                0              // 绝对编码器原点偏置 支持正负值 使原点计数为 0
  #endif
  #ifndef AXIS4_ENCODER_REVERSE
#define AXIS4_ENCODER_REVERSE               OFF            // 反转编码器计数方向
  #endif
#endif

#if AXIS4_DRIVER_MODEL == KTECH
#define AXIS4_KTECH_PRESENT                                // 轴 4 启用 KTECH 驱动
#endif

#if AXIS4_DRIVER_MODEL == MKS42D
#define AXIS4_MKS42D_PRESENT                               // 轴 4 启用 MKS42D 驱动
#endif

// 调焦器 2 设置 FOCUSER2
#ifndef AXIS5_DRIVER_MODEL
#define AXIS5_DRIVER_MODEL                  OFF            // 指定驱动型号以启用
#endif
#ifndef AXIS5_STEPS_PER_MICRON
#define AXIS5_STEPS_PER_MICRON              0.5            // 每微米运动对应的步数或微步数
#endif
#ifndef AXIS5_COUNTS_PER_MICRON
// 每微米编码器计数
#define AXIS5_COUNTS_PER_MICRON             AXIS5_STEPS_PER_MICRON
#endif
#ifndef AXIS5_REVERSE
#define AXIS5_REVERSE                       OFF            // 反转旋转方向
#endif
#ifndef AXIS5_POWER_DOWN
#define AXIS5_POWER_DOWN                    OFF            // 静止时关闭电机电源
#endif
#ifndef AXIS5_POWER_DOWN_TIME
#define AXIS5_POWER_DOWN_TIME               30000          // 断电延时 单位毫秒
#endif
#ifndef AXIS5_SLAVED_TO_FOCUSER
#define AXIS5_SLAVED_TO_FOCUSER             0              // 跟随的主调焦器编号 0 为禁用
#endif
#ifndef AXIS5_ENABLE_STATE
#define AXIS5_ENABLE_STATE                  LOW            // 电机通电时使能引脚的默认电平
#endif
#ifndef AXIS5_SLEW_RATE_MINIMUM
#define AXIS5_SLEW_RATE_MINIMUM             20             // 单位微米每秒
#endif
#ifndef AXIS5_SLEW_RATE_BASE_DESIRED
#define AXIS5_SLEW_RATE_BASE_DESIRED        500            // 单位度每秒
#endif
#ifndef AXIS5_ACCELERATION_TIME
#define AXIS5_ACCELERATION_TIME             1.0            // 加速至选定速度所需时间 单位秒
#endif
#ifndef AXIS5_RAPID_STOP_TIME
#define AXIS5_RAPID_STOP_TIME               1.0            // 停止所需时间 单位秒
#endif
#ifndef AXIS5_BACKLASH_RATE
// 单位度每秒
#define AXIS5_BACKLASH_RATE                 (AXIS5_SLEW_RATE_BASE_DESIRED/4)
#endif
#ifndef AXIS5_LIMIT_MIN
#define AXIS5_LIMIT_MIN                     0              // 单位度
#endif
#ifndef AXIS5_LIMIT_MAX
#define AXIS5_LIMIT_MAX                     50             // 单位度
#endif
#ifndef AXIS5_HOME_DEFAULT
#define AXIS5_HOME_DEFAULT                  MIDDLE         // 可选 MINIMUM 零位 MIDDLE 半行程 MAXIMUM 全行程 或指定微米位置
#endif
#ifndef AXIS5_SENSE_HOME
#define AXIS5_SENSE_HOME                    OFF            // 从正面看位于原点顺时针方向时的电平 HIGH 或 LOW
#endif
#ifndef AXIS5_SENSE_HOME_INIT
#define AXIS5_SENSE_HOME_INIT               INPUT_PULLUP   // 原点检测引脚模式
#endif
#ifndef AXIS5_SENSE_HOME_DIST_LIMIT
#define AXIS5_SENSE_HOME_DIST_LIMIT         50             // 最大距离 单位度
#endif
#ifndef AXIS5_SENSE_LIMIT_MIN
#define AXIS5_SENSE_LIMIT_MIN               OFF            // 最小限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS5_SENSE_LIMIT_MAX
#define AXIS5_SENSE_LIMIT_MAX               OFF            // 最大限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS5_SENSE_LIMIT_INIT
#define AXIS5_SENSE_LIMIT_INIT              INPUT_PULLUP   // 限位检测引脚模式
#endif

#if AXIS5_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS5_DRIVER_MODEL <= STEP_DIR_DRIVER_LAST
#define AXIS5_STEP_DIR_PRESENT                             // 轴 5 启用 STEP/DIR 驱动
  
  #ifndef AXIS5_STEP_STATE
#define AXIS5_STEP_STATE                    HIGH           // 步进信号的默认有效电平
  #endif
  #ifndef AXIS5_M2_ON_STATE
#define AXIS5_M2_ON_STATE                   HIGH           // M2 的默认有效电平
  #endif
  #ifndef AXIS5_DRIVER_MICROSTEPS
#define AXIS5_DRIVER_MICROSTEPS             OFF            // 正常运行微步模式
  #endif
  #ifndef AXIS5_DRIVER_MICROSTEPS_GOTO
#define AXIS5_DRIVER_MICROSTEPS_GOTO        OFF            // 快速移动时的微步模式
  #endif
  #ifndef AXIS5_DRIVER_INTPOL
#define AXIS5_DRIVER_INTPOL                 ON             // 支持时启用微步插值
  #endif
  #ifndef AXIS5_DRIVER_DECAY
#define AXIS5_DRIVER_DECAY                  OFF            // OFF 使用默认值 TMC 可选 STEALTHCHOP
  #endif
  #ifndef AXIS5_DRIVER_DECAY_GOTO
#define AXIS5_DRIVER_DECAY_GOTO             OFF            // OFF 使用默认值 TMC 可选 SPREADCYCLE
  #endif
  #ifndef AXIS5_DRIVER_IHOLD
#define AXIS5_DRIVER_IHOLD                  OFF            // 单位毫安
  #endif
  #ifndef AXIS5_DRIVER_IRUN
#define AXIS5_DRIVER_IRUN                   OFF            // 单位毫安
  #endif
  #ifndef AXIS5_DRIVER_IGOTO
#define AXIS5_DRIVER_IGOTO                  OFF            // 单位毫安
  #endif
  #ifndef AXIS5_DRIVER_STATUS
#define AXIS5_DRIVER_STATUS                 OFF            // 驱动状态检测 TMC SPI 用 ON 故障引脚用 HIGH 或 LOW
  #endif
#endif

#if AXIS5_DRIVER_MODEL >= SERVO_DRIVER_FIRST && AXIS2_DRIVER_MODEL <= SERVO_DRIVER_LAST
#define AXIS5_SERVO_PRESENT                                // 轴 5 启用 SERVO 驱动

  #ifndef AXIS5_SERVO_PH1_STATE
#define AXIS5_SERVO_PH1_STATE               LOW            // 电机驱动空闲电平 IN1 用于 SERVO_EE PHASE 用于 SERVO_PE
  #endif
  #ifndef AXIS5_SERVO_PH2_STATE
#define AXIS5_SERVO_PH2_STATE               LOW            // 电机驱动空闲电平 IN2 或 ENABLE PWM 引脚
  #endif

  #ifndef AXIS5_SERVO_ACCELERATION
#define AXIS5_SERVO_ACCELERATION            100            // 加速度限制 单位百分比每秒
  #endif
  #ifndef AXIS5_SERVO_VELOCITY_PWMTHRS
#define AXIS5_SERVO_VELOCITY_PWMTHRS        OFF            // 从 stealthChop 切换到 spreadCycle 的速度 单位步每秒
  #endif                            
  #ifndef AXIS5_SERVO_DC_PWR_MIN
#define AXIS5_SERVO_DC_PWR_MIN              0.0F           // 略低于轴开始运动时的最小功率 单位百分比
  #endif
  #ifndef AXIS5_SERVO_DC_PWR_MAX
#define AXIS5_SERVO_DC_PWR_MAX              100.0F         // 达到最快移动速度所需的功率 单位百分比
  #endif

  #ifndef AXIS5_SERVO_FEEDBACK
#define AXIS5_SERVO_FEEDBACK                PID            // 反馈类型 DUAL_PID
  #endif
  #ifndef AXIS5_PID_P
#define AXIS5_PID_P                         0.0            // P 比例系数
  #endif
  #ifndef AXIS5_PID_I
#define AXIS5_PID_I                         0.0            // I 积分系数
  #endif
  #ifndef AXIS5_PID_D
#define AXIS5_PID_D                         0.0            // D 微分系数
  #endif
  #ifndef AXIS5_PID_P_GOTO
#define AXIS5_PID_P_GOTO                    AXIS5_PID_P    // P 比例系数
  #endif
  #ifndef AXIS5_PID_I_GOTO
#define AXIS5_PID_I_GOTO                    AXIS5_PID_I    // I 积分系数
  #endif
  #ifndef AXIS5_PID_D_GOTO
#define AXIS5_PID_D_GOTO                    AXIS5_PID_D    // D 微分系数
  #endif

  #ifndef AXIS5_SERVO_FLTR
#define AXIS5_SERVO_FLTR                    OFF            // 伺服编码器滤波 OFF 为关闭
  #endif
  #ifndef AXIS5_ENCODER
#define AXIS5_ENCODER                       AB             // 编码器类型 AB CW_CCW PULSE_DIR PULSE_ONLY SERIAL_BRIDGE
  #endif
  #ifndef AXIS5_ENCODER_ORIGIN
#define AXIS5_ENCODER_ORIGIN                0              // 绝对编码器原点偏置 支持正负值 使原点计数为 0
  #endif
  #ifndef AXIS5_ENCODER_REVERSE
#define AXIS5_ENCODER_REVERSE               OFF            // 反转编码器计数方向
  #endif
#endif

#if AXIS5_DRIVER_MODEL == KTECH
#define AXIS5_KTECH_PRESENT                                // 轴 5 启用 KTECH 驱动
#endif

#if AXIS5_DRIVER_MODEL == MKS42D
#define AXIS5_MKS42D_PRESENT                               // 轴 5 启用 MKS42D 驱动
#endif

// 调焦器 3 设置 FOCUSER3
#ifndef AXIS6_DRIVER_MODEL
#define AXIS6_DRIVER_MODEL                  OFF            // 指定驱动型号以启用
#endif
#ifndef AXIS6_STEPS_PER_MICRON
#define AXIS6_STEPS_PER_MICRON              0.5            // 每微米运动对应的步数或微步数
#endif
#ifndef AXIS6_COUNTS_PER_MICRON
// 每微米编码器计数
#define AXIS6_COUNTS_PER_MICRON             AXIS6_STEPS_PER_MICRON
#endif
#ifndef AXIS6_REVERSE
#define AXIS6_REVERSE                       OFF            // 反转旋转方向
#endif
#ifndef AXIS6_POWER_DOWN
#define AXIS6_POWER_DOWN                    OFF            // 静止时关闭电机电源
#endif
#ifndef AXIS6_POWER_DOWN_TIME
#define AXIS6_POWER_DOWN_TIME               30000          // 断电延时 单位毫秒
#endif
#ifndef AXIS6_SLAVED_TO_FOCUSER
#define AXIS6_SLAVED_TO_FOCUSER             0              // 跟随的主调焦器编号 0 为禁用
#endif
#ifndef AXIS6_ENABLE_STATE
#define AXIS6_ENABLE_STATE                  LOW            // 电机通电时使能引脚的默认电平
#endif
#ifndef AXIS6_SLEW_RATE_MINIMUM
#define AXIS6_SLEW_RATE_MINIMUM             20             // 单位微米每秒
#endif
#ifndef AXIS6_SLEW_RATE_BASE_DESIRED
#define AXIS6_SLEW_RATE_BASE_DESIRED        500            // 单位度每秒
#endif
#ifndef AXIS6_ACCELERATION_TIME
#define AXIS6_ACCELERATION_TIME             1.0            // 加速至选定速度所需时间 单位秒
#endif
#ifndef AXIS6_RAPID_STOP_TIME
#define AXIS6_RAPID_STOP_TIME               1.0            // 停止所需时间 单位秒
#endif
#ifndef AXIS6_BACKLASH_RATE
// 单位度每秒
#define AXIS6_BACKLASH_RATE                 (AXIS6_SLEW_RATE_BASE_DESIRED/4)
#endif
#ifndef AXIS6_LIMIT_MIN
#define AXIS6_LIMIT_MIN                     0              // 单位度
#endif
#ifndef AXIS6_LIMIT_MAX
#define AXIS6_LIMIT_MAX                     50             // 单位度
#endif
#ifndef AXIS6_HOME_DEFAULT
#define AXIS6_HOME_DEFAULT                  MIDDLE         // 可选 MINIMUM 零位 MIDDLE 半行程 MAXIMUM 全行程 或指定微米位置
#endif
#ifndef AXIS6_SENSE_HOME
#define AXIS6_SENSE_HOME                    OFF            // 从正面看位于原点顺时针方向时的电平 HIGH 或 LOW
#endif
#ifndef AXIS6_SENSE_HOME_INIT
#define AXIS6_SENSE_HOME_INIT               INPUT_PULLUP   // 原点检测引脚模式
#endif
#ifndef AXIS6_SENSE_HOME_DIST_LIMIT
#define AXIS6_SENSE_HOME_DIST_LIMIT         50             // 最大距离 单位度
#endif
#ifndef AXIS6_SENSE_LIMIT_MIN
#define AXIS6_SENSE_LIMIT_MIN               OFF            // 最小限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS6_SENSE_LIMIT_MAX
#define AXIS6_SENSE_LIMIT_MAX               OFF            // 最大限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS6_SENSE_LIMIT_INIT
#define AXIS6_SENSE_LIMIT_INIT              INPUT_PULLUP   // 限位检测引脚模式
#endif

#if AXIS6_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS6_DRIVER_MODEL <= STEP_DIR_DRIVER_LAST
#define AXIS6_STEP_DIR_PRESENT                             // 轴 6 启用 STEP/DIR 驱动

  #ifndef AXIS6_STEP_STATE
#define AXIS6_STEP_STATE                    HIGH           // 步进信号的默认有效电平
  #endif
  #ifndef AXIS6_M2_ON_STATE
#define AXIS6_M2_ON_STATE                   HIGH           // M2 的默认有效电平
  #endif
  #ifndef AXIS6_DRIVER_MICROSTEPS
#define AXIS6_DRIVER_MICROSTEPS             OFF            // 正常运行微步模式
  #endif
  #ifndef AXIS6_DRIVER_MICROSTEPS_GOTO
#define AXIS6_DRIVER_MICROSTEPS_GOTO        OFF            // 快速移动时的微步模式
  #endif
  #ifndef AXIS6_DRIVER_INTPOL
#define AXIS6_DRIVER_INTPOL                 ON             // 支持时启用微步插值
  #endif
  #ifndef AXIS6_DRIVER_DECAY
#define AXIS6_DRIVER_DECAY                  OFF            // OFF 使用默认值 TMC 可选 STEALTHCHOP
  #endif
  #ifndef AXIS6_DRIVER_DECAY_GOTO
#define AXIS6_DRIVER_DECAY_GOTO             OFF            // OFF 使用默认值 TMC 可选 SPREADCYCLE
  #endif
  #ifndef AXIS6_DRIVER_IHOLD
#define AXIS6_DRIVER_IHOLD                  OFF            // 单位毫安
  #endif
  #ifndef AXIS6_DRIVER_IRUN
#define AXIS6_DRIVER_IRUN                   OFF            // 单位毫安
  #endif
  #ifndef AXIS6_DRIVER_IGOTO
#define AXIS6_DRIVER_IGOTO                  OFF            // 单位毫安
  #endif
  #ifndef AXIS6_DRIVER_STATUS
#define AXIS6_DRIVER_STATUS                 OFF            // 驱动状态检测 TMC SPI 用 ON 故障引脚用 HIGH 或 LOW
  #endif
#endif

#if AXIS6_DRIVER_MODEL >= SERVO_DRIVER_FIRST && AXIS2_DRIVER_MODEL <= SERVO_DRIVER_LAST
#define AXIS6_SERVO_PRESENT                                // 轴 6 启用 SERVO 驱动

  #ifndef AXIS6_SERVO_PH1_STATE
#define AXIS6_SERVO_PH1_STATE               LOW            // 电机驱动空闲电平 IN1 用于 SERVO_EE PHASE 用于 SERVO_PE
  #endif
  #ifndef AXIS6_SERVO_PH2_STATE
#define AXIS6_SERVO_PH2_STATE               LOW            // 电机驱动空闲电平 IN2 或 ENABLE PWM 引脚
  #endif

  #ifndef AXIS6_SERVO_ACCELERATION
#define AXIS6_SERVO_ACCELERATION            100            // 加速度限制 单位百分比每秒
  #endif
  #ifndef AXIS6_SERVO_VELOCITY_PWMTHRS
#define AXIS6_SERVO_VELOCITY_PWMTHRS        OFF            // 从 stealthChop 切换到 spreadCycle 的速度 单位步每秒
  #endif                            
  #ifndef AXIS6_SERVO_DC_PWR_MIN
#define AXIS6_SERVO_DC_PWR_MIN              0.0F           // 略低于轴开始运动时的最小功率 单位百分比
  #endif
  #ifndef AXIS6_SERVO_DC_PWR_MAX
#define AXIS6_SERVO_DC_PWR_MAX              100.0F         // 达到最快移动速度所需的功率 单位百分比
  #endif

  #ifndef AXIS6_SERVO_FEEDBACK
#define AXIS6_SERVO_FEEDBACK                PID            // 反馈类型 DUAL_PID
  #endif
  #ifndef AXIS6_PID_P
#define AXIS6_PID_P                         0.0            // P 比例系数
  #endif
  #ifndef AXIS6_PID_I
#define AXIS6_PID_I                         0.0            // I 积分系数
  #endif
  #ifndef AXIS6_PID_D
#define AXIS6_PID_D                         0.0            // D 微分系数
  #endif

  #ifndef AXIS6_SERVO_FLTR
#define AXIS6_SERVO_FLTR                    OFF            // 伺服编码器滤波 OFF 为关闭
  #endif
  #ifndef AXIS6_ENCODER
#define AXIS6_ENCODER                       AB             // 编码器类型 AB CW_CCW PULSE_DIR PULSE_ONLY SERIAL_BRIDGE
  #endif
  #ifndef AXIS6_ENCODER_ORIGIN
#define AXIS6_ENCODER_ORIGIN                0              // 绝对编码器原点偏置 支持正负值 使原点计数为 0
  #endif
  #ifndef AXIS6_ENCODER_REVERSE
#define AXIS6_ENCODER_REVERSE               OFF            // 反转编码器计数方向
  #endif
#endif

#if AXIS6_DRIVER_MODEL == KTECH
#define AXIS6_KTECH_PRESENT                                // 轴 6 启用 KTECH 驱动
#endif

#if AXIS6_DRIVER_MODEL == MKS42D
#define AXIS6_MKS42D_PRESENT                               // 轴 6 启用 MKS42D 驱动
#endif

// 调焦器 4 设置 FOCUSER4
#ifndef AXIS7_DRIVER_MODEL
#define AXIS7_DRIVER_MODEL                  OFF            // 指定驱动型号以启用
#endif
#ifndef AXIS7_STEPS_PER_MICRON
#define AXIS7_STEPS_PER_MICRON              0.5            // 每微米运动对应的步数或微步数
#endif
#ifndef AXIS7_COUNTS_PER_MICRON
// 每微米编码器计数
#define AXIS7_COUNTS_PER_MICRON             AXIS7_STEPS_PER_MICRON
#endif
#ifndef AXIS7_REVERSE
#define AXIS7_REVERSE                       OFF            // 反转旋转方向
#endif
#ifndef AXIS7_POWER_DOWN
#define AXIS7_POWER_DOWN                    OFF            // 静止时关闭电机电源
#endif
#ifndef AXIS7_POWER_DOWN_TIME
#define AXIS7_POWER_DOWN_TIME               30000          // 断电延时 单位毫秒
#endif
#ifndef AXIS7_SLAVED_TO_FOCUSER
#define AXIS7_SLAVED_TO_FOCUSER             0              // 跟随的主调焦器编号 0 为禁用
#endif
#ifndef AXIS7_ENABLE_STATE
#define AXIS7_ENABLE_STATE                  LOW            // 电机通电时使能引脚的默认电平
#endif
#ifndef AXIS7_SLEW_RATE_MINIMUM
#define AXIS7_SLEW_RATE_MINIMUM             20             // 单位微米每秒
#endif
#ifndef AXIS7_SLEW_RATE_BASE_DESIRED
#define AXIS7_SLEW_RATE_BASE_DESIRED        500            // 单位度每秒
#endif
#ifndef AXIS7_ACCELERATION_TIME
#define AXIS7_ACCELERATION_TIME             1.0            // 加速至选定速度所需时间 单位秒
#endif
#ifndef AXIS7_RAPID_STOP_TIME
#define AXIS7_RAPID_STOP_TIME               1.0            // 停止所需时间 单位秒
#endif
#ifndef AXIS7_BACKLASH_RATE
// 单位度每秒
#define AXIS7_BACKLASH_RATE                 (AXIS7_SLEW_RATE_BASE_DESIRED/4)
#endif
#ifndef AXIS7_LIMIT_MIN
#define AXIS7_LIMIT_MIN                     0              // 单位度
#endif
#ifndef AXIS7_LIMIT_MAX
#define AXIS7_LIMIT_MAX                     50             // 单位度
#endif
#ifndef AXIS7_HOME_DEFAULT
#define AXIS7_HOME_DEFAULT                  MIDDLE         // 可选 MINIMUM 零位 MIDDLE 半行程 MAXIMUM 全行程 或指定微米位置
#endif
#ifndef AXIS7_SENSE_HOME
#define AXIS7_SENSE_HOME                    OFF            // 从正面看位于原点顺时针方向时的电平 HIGH 或 LOW
#endif
#ifndef AXIS7_SENSE_HOME_INIT
#define AXIS7_SENSE_HOME_INIT               INPUT_PULLUP   // 原点检测引脚模式
#endif
#ifndef AXIS7_SENSE_HOME_DIST_LIMIT
#define AXIS7_SENSE_HOME_DIST_LIMIT         50             // 最大距离 单位度
#endif
#ifndef AXIS7_SENSE_LIMIT_MIN
#define AXIS7_SENSE_LIMIT_MIN               OFF            // 最小限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS7_SENSE_LIMIT_MAX
#define AXIS7_SENSE_LIMIT_MAX               OFF            // 最大限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS7_SENSE_LIMIT_INIT
#define AXIS7_SENSE_LIMIT_INIT              INPUT_PULLUP   // 限位检测引脚模式
#endif
#if AXIS7_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS7_DRIVER_MODEL <= STEP_DIR_DRIVER_LAST
#define AXIS7_STEP_DIR_PRESENT                             // 轴 7 启用 STEP/DIR 驱动

  #ifndef AXIS7_STEP_STATE
#define AXIS7_STEP_STATE                    HIGH           // 步进信号的默认有效电平
  #endif
  #ifndef AXIS7_M2_ON_STATE
#define AXIS7_M2_ON_STATE                   HIGH           // M2 的默认有效电平
  #endif
  #ifndef AXIS7_DRIVER_MICROSTEPS
#define AXIS7_DRIVER_MICROSTEPS             OFF            // 正常运行微步模式
  #endif
  #ifndef AXIS7_DRIVER_MICROSTEPS_GOTO
#define AXIS7_DRIVER_MICROSTEPS_GOTO        OFF            // 快速移动时的微步模式
  #endif
  #ifndef AXIS7_DRIVER_INTPOL
#define AXIS7_DRIVER_INTPOL                 ON             // 支持时启用微步插值
  #endif
  #ifndef AXIS7_DRIVER_DECAY
#define AXIS7_DRIVER_DECAY                  OFF            // OFF 使用默认值 TMC 可选 STEALTHCHOP
  #endif
  #ifndef AXIS7_DRIVER_DECAY_GOTO
#define AXIS7_DRIVER_DECAY_GOTO             OFF            // OFF 使用默认值 TMC 可选 SPREADCYCLE
  #endif
  #ifndef AXIS7_DRIVER_IHOLD
#define AXIS7_DRIVER_IHOLD                  OFF            // 单位毫安
  #endif
  #ifndef AXIS7_DRIVER_IRUN
#define AXIS7_DRIVER_IRUN                   OFF            // 单位毫安
  #endif
  #ifndef AXIS7_DRIVER_IGOTO
#define AXIS7_DRIVER_IGOTO                  OFF            // 单位毫安
  #endif
  #ifndef AXIS7_DRIVER_STATUS
#define AXIS7_DRIVER_STATUS                 OFF            // 驱动状态检测 TMC SPI 用 ON 故障引脚用 HIGH 或 LOW
  #endif
#endif

#if AXIS7_DRIVER_MODEL >= SERVO_DRIVER_FIRST && AXIS2_DRIVER_MODEL <= SERVO_DRIVER_LAST
#define AXIS7_SERVO_PRESENT                                // 轴 7 启用 SERVO 驱动

  #ifndef AXIS7_SERVO_PH1_STATE
#define AXIS7_SERVO_PH1_STATE               LOW            // 电机驱动空闲电平 IN1 用于 SERVO_EE PHASE 用于 SERVO_PE
  #endif
  #ifndef AXIS7_SERVO_PH2_STATE
#define AXIS7_SERVO_PH2_STATE               LOW            // 电机驱动空闲电平 IN2 或 ENABLE PWM 引脚
  #endif

  #ifndef AXIS7_SERVO_ACCELERATION
#define AXIS7_SERVO_ACCELERATION            100            // 加速度限制 单位百分比每秒
  #endif
  #ifndef AXIS7_SERVO_VELOCITY_PWMTHRS
#define AXIS7_SERVO_VELOCITY_PWMTHRS        OFF            // 从 stealthChop 切换到 spreadCycle 的速度 单位步每秒
  #endif                            
  #ifndef AXIS7_SERVO_DC_PWR_MIN
#define AXIS7_SERVO_DC_PWR_MIN              0.0F           // 略低于轴开始运动时的最小功率 单位百分比
  #endif
  #ifndef AXIS7_SERVO_DC_PWR_MAX
#define AXIS7_SERVO_DC_PWR_MAX              100.0F         // 达到最快移动速度所需的功率 单位百分比
  #endif

  #ifndef AXIS7_SERVO_FEEDBACK
#define AXIS7_SERVO_FEEDBACK                PID            // 反馈类型 DUAL_PID
  #endif
  #ifndef AXIS7_PID_P
#define AXIS7_PID_P                         0.0            // P 比例系数
  #endif
  #ifndef AXIS7_PID_I
#define AXIS7_PID_I                         0.0            // I 积分系数
  #endif
  #ifndef AXIS7_PID_D
#define AXIS7_PID_D                         0.0            // D 微分系数
  #endif

  #ifndef AXIS7_SERVO_FLTR
#define AXIS7_SERVO_FLTR                    OFF            // 伺服编码器滤波 OFF 为关闭
  #endif
  #ifndef AXIS7_ENCODER
#define AXIS7_ENCODER                       AB             // 编码器类型 AB CW_CCW PULSE_DIR PULSE_ONLY SERIAL_BRIDGE
  #endif
  #ifndef AXIS7_ENCODER_ORIGIN
#define AXIS7_ENCODER_ORIGIN                0              // 绝对编码器原点偏置 支持正负值 使原点计数为 0
  #endif
  #ifndef AXIS7_ENCODER_REVERSE
#define AXIS7_ENCODER_REVERSE               OFF            // 反转编码器计数方向
  #endif
#endif

#if AXIS7_DRIVER_MODEL == KTECH
#define AXIS7_KTECH_PRESENT                                // 轴 7 启用 KTECH 驱动
#endif

#if AXIS7_DRIVER_MODEL == MKS42D
#define AXIS7_MKS42D_PRESENT                               // 轴 7 启用 MKS42D 驱动
#endif

// 调焦器 5 设置 FOCUSER5
#ifndef AXIS8_DRIVER_MODEL
#define AXIS8_DRIVER_MODEL                  OFF            // 指定驱动型号以启用
#endif
#ifndef AXIS8_STEPS_PER_MICRON
#define AXIS8_STEPS_PER_MICRON              0.5            // 每微米运动对应的步数或微步数
#endif
#ifndef AXIS8_COUNTS_PER_MICRON
// 每微米编码器计数
#define AXIS8_COUNTS_PER_MICRON             AXIS8_STEPS_PER_MICRON
#endif
#ifndef AXIS8_REVERSE
#define AXIS8_REVERSE                       OFF            // 反转旋转方向
#endif
#ifndef AXIS8_POWER_DOWN
#define AXIS8_POWER_DOWN                    OFF            // 静止时关闭电机电源
#endif
#ifndef AXIS8_POWER_DOWN_TIME
#define AXIS8_POWER_DOWN_TIME               30000          // 断电延时 单位毫秒
#endif
#ifndef AXIS8_SLAVED_TO_FOCUSER
#define AXIS8_SLAVED_TO_FOCUSER             0              // 跟随的主调焦器编号 0 为禁用
#endif
#ifndef AXIS8_ENABLE_STATE
#define AXIS8_ENABLE_STATE                  LOW            // 电机通电时使能引脚的默认电平
#endif
#ifndef AXIS8_SLEW_RATE_MINIMUM
#define AXIS8_SLEW_RATE_MINIMUM             20             // 单位微米每秒
#endif
#ifndef AXIS8_SLEW_RATE_BASE_DESIRED
#define AXIS8_SLEW_RATE_BASE_DESIRED        500            // 单位度每秒
#endif
#ifndef AXIS8_ACCELERATION_TIME
#define AXIS8_ACCELERATION_TIME             1.0            // 加速至选定速度所需时间 单位秒
#endif
#ifndef AXIS8_RAPID_STOP_TIME
#define AXIS8_RAPID_STOP_TIME               1.0            // 停止所需时间 单位秒
#endif
#ifndef AXIS8_BACKLASH_RATE
// 单位度每秒
#define AXIS8_BACKLASH_RATE                 (AXIS8_SLEW_RATE_BASE_DESIRED/4)
#endif
#ifndef AXIS8_LIMIT_MIN
#define AXIS8_LIMIT_MIN                     0              // 单位度
#endif
#ifndef AXIS8_LIMIT_MAX
#define AXIS8_LIMIT_MAX                     50             // 单位度
#endif
#ifndef AXIS8_HOME_DEFAULT
#define AXIS8_HOME_DEFAULT                  MIDDLE         // 可选 MINIMUM 零位 MIDDLE 半行程 MAXIMUM 全行程 或指定微米位置
#endif
#ifndef AXIS8_SENSE_HOME
#define AXIS8_SENSE_HOME                    OFF            // 从正面看位于原点顺时针方向时的电平 HIGH 或 LOW
#endif
#ifndef AXIS8_SENSE_HOME_INIT
#define AXIS8_SENSE_HOME_INIT               INPUT_PULLUP   // 原点检测引脚模式
#endif
#ifndef AXIS8_SENSE_HOME_DIST_LIMIT
#define AXIS8_SENSE_HOME_DIST_LIMIT         50             // 最大距离 单位度
#endif
#ifndef AXIS8_SENSE_LIMIT_MIN
#define AXIS8_SENSE_LIMIT_MIN               OFF            // 最小限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS8_SENSE_LIMIT_MAX
#define AXIS8_SENSE_LIMIT_MAX               OFF            // 最大限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS8_SENSE_LIMIT_INIT
#define AXIS8_SENSE_LIMIT_INIT              INPUT_PULLUP   // 限位检测引脚模式
#endif

#if AXIS8_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS8_DRIVER_MODEL <= STEP_DIR_DRIVER_LAST
#define AXIS8_STEP_DIR_PRESENT                             // 轴 8 启用 STEP/DIR 驱动

  #ifndef AXIS8_STEP_STATE
#define AXIS8_STEP_STATE                    HIGH           // 步进信号的默认有效电平
  #endif
  #ifndef AXIS8_M2_ON_STATE
#define AXIS8_M2_ON_STATE                   HIGH           // M2 的默认有效电平
  #endif
  #ifndef AXIS8_DRIVER_MICROSTEPS
#define AXIS8_DRIVER_MICROSTEPS             OFF            // 正常运行微步模式
  #endif
  #ifndef AXIS8_DRIVER_MICROSTEPS_GOTO
#define AXIS8_DRIVER_MICROSTEPS_GOTO        OFF            // 快速移动时的微步模式
  #endif
  #ifndef AXIS8_DRIVER_INTPOL
#define AXIS8_DRIVER_INTPOL                 ON             // 支持时启用微步插值
  #endif
  #ifndef AXIS8_DRIVER_DECAY
#define AXIS8_DRIVER_DECAY                  OFF            // OFF 使用默认值 TMC 可选 STEALTHCHOP
  #endif
  #ifndef AXIS8_DRIVER_DECAY_GOTO
#define AXIS8_DRIVER_DECAY_GOTO             OFF            // OFF 使用默认值 TMC 可选 SPREADCYCLE
  #endif
  #ifndef AXIS8_DRIVER_IHOLD
#define AXIS8_DRIVER_IHOLD                  OFF            // 单位毫安
  #endif
  #ifndef AXIS8_DRIVER_IRUN
#define AXIS8_DRIVER_IRUN                   OFF            // 单位毫安
  #endif
  #ifndef AXIS8_DRIVER_IGOTO
#define AXIS8_DRIVER_IGOTO                  OFF            // 单位毫安
  #endif
  #ifndef AXIS8_DRIVER_STATUS
#define AXIS8_DRIVER_STATUS                 OFF            // 驱动状态检测 TMC SPI 用 ON 故障引脚用 HIGH 或 LOW
  #endif
#endif

#if AXIS8_DRIVER_MODEL >= SERVO_DRIVER_FIRST && AXIS2_DRIVER_MODEL <= SERVO_DRIVER_LAST
#define AXIS8_SERVO_PRESENT                                // 轴 8 启用 SERVO 驱动

  #ifndef AXIS8_SERVO_PH1_STATE
#define AXIS8_SERVO_PH1_STATE               LOW            // 电机驱动空闲电平 IN1 用于 SERVO_EE PHASE 用于 SERVO_PE
  #endif
  #ifndef AXIS8_SERVO_PH2_STATE
#define AXIS8_SERVO_PH2_STATE               LOW            // 电机驱动空闲电平 IN2 或 ENABLE PWM 引脚
  #endif

  #ifndef AXIS8_SERVO_ACCELERATION
#define AXIS8_SERVO_ACCELERATION            100            // 加速度限制 单位百分比每秒
  #endif
  #ifndef AXIS8_SERVO_VELOCITY_PWMTHRS
#define AXIS8_SERVO_VELOCITY_PWMTHRS        OFF            // 从 stealthChop 切换到 spreadCycle 的速度 单位步每秒
  #endif                            
  #ifndef AXIS8_SERVO_DC_PWR_MIN
#define AXIS8_SERVO_DC_PWR_MIN              0.0F           // 略低于轴开始运动时的最小功率 单位百分比
  #endif
  #ifndef AXIS8_SERVO_DC_PWR_MAX
#define AXIS8_SERVO_DC_PWR_MAX              100.0F         // 达到最快移动速度所需的功率 单位百分比
  #endif

  #ifndef AXIS8_SERVO_FEEDBACK
#define AXIS8_SERVO_FEEDBACK                PID            // 反馈类型 DUAL_PID
  #endif
  #ifndef AXIS8_PID_P
#define AXIS8_PID_P                         0.0            // P 比例系数
  #endif
  #ifndef AXIS8_PID_I
#define AXIS8_PID_I                         0.0            // I 积分系数
  #endif
  #ifndef AXIS8_PID_D
#define AXIS8_PID_D                         0.0            // D 微分系数
  #endif

  #ifndef AXIS8_SERVO_FLTR
#define AXIS8_SERVO_FLTR                    OFF            // 伺服编码器滤波 OFF 为关闭
  #endif
  #ifndef AXIS8_ENCODER
#define AXIS8_ENCODER                       AB             // 编码器类型 AB CW_CCW PULSE_DIR PULSE_ONLY SERIAL_BRIDGE
  #endif
  #ifndef AXIS8_ENCODER_ORIGIN
#define AXIS8_ENCODER_ORIGIN                0              // 绝对编码器原点偏置 支持正负值 使原点计数为 0
  #endif
  #ifndef AXIS8_ENCODER_REVERSE
#define AXIS8_ENCODER_REVERSE               OFF            // 反转编码器计数方向
  #endif
#endif

#if AXIS8_DRIVER_MODEL == KTECH
#define AXIS8_KTECH_PRESENT                                // 轴 8 启用 KTECH 驱动
#endif

#if AXIS8_DRIVER_MODEL == MKS42D
#define AXIS8_MKS42D_PRESENT                               // 轴 8 启用 MKS42D 驱动
#endif

// 调焦器 6 设置 FOCUSER6
#ifndef AXIS9_DRIVER_MODEL
#define AXIS9_DRIVER_MODEL                  OFF            // 指定驱动型号以启用
#endif
#ifndef AXIS9_STEPS_PER_MICRON
#define AXIS9_STEPS_PER_MICRON              0.5            // 每微米运动对应的步数或微步数
#endif
#ifndef AXIS9_COUNTS_PER_MICRON
// 每微米编码器计数
#define AXIS9_COUNTS_PER_MICRON             AXIS9_STEPS_PER_MICRON
#endif
#ifndef AXIS9_REVERSE
#define AXIS9_REVERSE                       OFF            // 反转旋转方向
#endif
#ifndef AXIS9_POWER_DOWN
#define AXIS9_POWER_DOWN                    OFF            // 静止时关闭电机电源
#endif
#ifndef AXIS9_POWER_DOWN_TIME
#define AXIS9_POWER_DOWN_TIME               30000          // 断电延时 单位毫秒
#endif
#ifndef AXIS9_SLAVED_TO_FOCUSER
#define AXIS9_SLAVED_TO_FOCUSER             0              // 跟随的主调焦器编号 0 为禁用
#endif
#ifndef AXIS9_ENABLE_STATE
#define AXIS9_ENABLE_STATE                  LOW            // 电机通电时使能引脚的默认电平
#endif
#ifndef AXIS9_SLEW_RATE_MINIMUM
#define AXIS9_SLEW_RATE_MINIMUM             20             // 单位微米每秒
#endif
#ifndef AXIS9_SLEW_RATE_BASE_DESIRED
#define AXIS9_SLEW_RATE_BASE_DESIRED        500            // 单位度每秒
#endif
#ifndef AXIS9_ACCELERATION_TIME
#define AXIS9_ACCELERATION_TIME             1.0            // 加速至选定速度所需时间 单位秒
#endif
#ifndef AXIS9_RAPID_STOP_TIME
#define AXIS9_RAPID_STOP_TIME               1.0            // 停止所需时间 单位秒
#endif
#ifndef AXIS9_BACKLASH_RATE
// 单位度每秒
#define AXIS9_BACKLASH_RATE                 (AXIS9_SLEW_RATE_BASE_DESIRED/4)
#endif
#ifndef AXIS9_LIMIT_MIN
#define AXIS9_LIMIT_MIN                     0              // 单位度
#endif
#ifndef AXIS9_LIMIT_MAX
#define AXIS9_LIMIT_MAX                     50             // 单位度
#endif
#ifndef AXIS9_HOME_DEFAULT
#define AXIS9_HOME_DEFAULT                  MIDDLE         // 可选 MINIMUM 零位 MIDDLE 半行程 MAXIMUM 全行程 或指定微米位置
#endif
#ifndef AXIS9_SENSE_HOME
#define AXIS9_SENSE_HOME                    OFF            // 从正面看位于原点顺时针方向时的电平 HIGH 或 LOW
#endif
#ifndef AXIS9_SENSE_HOME_INIT
#define AXIS9_SENSE_HOME_INIT               INPUT_PULLUP   // 原点检测引脚模式
#endif
#ifndef AXIS9_SENSE_HOME_DIST_LIMIT
#define AXIS9_SENSE_HOME_DIST_LIMIT         50             // 最大距离 单位度
#endif
#ifndef AXIS9_SENSE_LIMIT_MIN
#define AXIS9_SENSE_LIMIT_MIN               OFF            // 最小限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS9_SENSE_LIMIT_MAX
#define AXIS9_SENSE_LIMIT_MAX               OFF            // 最大限位触发电平 HIGH 或 LOW
#endif
#ifndef AXIS9_SENSE_LIMIT_INIT
#define AXIS9_SENSE_LIMIT_INIT              INPUT_PULLUP   // 限位检测引脚模式
#endif

#if AXIS9_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS9_DRIVER_MODEL <= STEP_DIR_DRIVER_LAST
#define AXIS9_STEP_DIR_PRESENT                             // 轴 9 启用 STEP/DIR 驱动

  #ifndef AXIS9_STEP_STATE
#define AXIS9_STEP_STATE                    HIGH           // 步进信号的默认有效电平
  #endif
  #ifndef AXIS9_M2_ON_STATE
#define AXIS9_M2_ON_STATE                   HIGH           // M2 的默认有效电平
  #endif
  #ifndef AXIS9_DRIVER_MICROSTEPS
#define AXIS9_DRIVER_MICROSTEPS             OFF            // 正常运行微步模式
  #endif
  #ifndef AXIS9_DRIVER_MICROSTEPS_GOTO
#define AXIS9_DRIVER_MICROSTEPS_GOTO        OFF            // 快速移动时的微步模式
  #endif
  #ifndef AXIS9_DRIVER_INTPOL
#define AXIS9_DRIVER_INTPOL                 ON             // 支持时启用微步插值
  #endif
  #ifndef AXIS9_DRIVER_DECAY
#define AXIS9_DRIVER_DECAY                  OFF            // OFF 使用默认值 TMC 可选 STEALTHCHOP
  #endif
  #ifndef AXIS9_DRIVER_DECAY_GOTO
#define AXIS9_DRIVER_DECAY_GOTO             OFF            // OFF 使用默认值 TMC 可选 SPREADCYCLE
  #endif
  #ifndef AXIS9_DRIVER_IHOLD
#define AXIS9_DRIVER_IHOLD                  OFF            // 单位毫安
  #endif
  #ifndef AXIS9_DRIVER_IRUN
#define AXIS9_DRIVER_IRUN                   OFF            // 单位毫安
  #endif
  #ifndef AXIS9_DRIVER_IGOTO
#define AXIS9_DRIVER_IGOTO                  OFF            // 单位毫安
  #endif
  #ifndef AXIS9_DRIVER_STATUS
#define AXIS9_DRIVER_STATUS                 OFF            // 驱动状态检测 TMC SPI 用 ON 故障引脚用 HIGH 或 LOW
  #endif
#endif

#if AXIS9_DRIVER_MODEL >= SERVO_DRIVER_FIRST && AXIS2_DRIVER_MODEL <= SERVO_DRIVER_LAST
#define AXIS9_SERVO_PRESENT                                // 轴 9 启用 SERVO 驱动

  #ifndef AXIS9_SERVO_PH1_STATE
#define AXIS9_SERVO_PH1_STATE               LOW            // 电机驱动空闲电平 IN1 用于 SERVO_EE PHASE 用于 SERVO_PE
  #endif
  #ifndef AXIS9_SERVO_PH2_STATE
#define AXIS9_SERVO_PH2_STATE               LOW            // 电机驱动空闲电平 IN2 或 ENABLE PWM 引脚
  #endif

  #ifndef AXIS9_SERVO_ACCELERATION
#define AXIS9_SERVO_ACCELERATION            100            // 加速度限制 单位百分比每秒
  #endif
  #ifndef AXIS9_SERVO_VELOCITY_PWMTHRS
#define AXIS9_SERVO_VELOCITY_PWMTHRS        OFF            // 从 stealthChop 切换到 spreadCycle 的速度 单位步每秒
  #endif                            
  #ifndef AXIS9_SERVO_DC_PWR_MIN
#define AXIS9_SERVO_DC_PWR_MIN              0.0F           // 略低于轴开始运动时的最小功率 单位百分比
  #endif
  #ifndef AXIS9_SERVO_DC_PWR_MAX
#define AXIS9_SERVO_DC_PWR_MAX              100.0F         // 达到最快移动速度所需的功率 单位百分比
  #endif

  #ifndef AXIS9_SERVO_FEEDBACK
#define AXIS9_SERVO_FEEDBACK                PID            // 反馈类型 DUAL_PID
  #endif
  #ifndef AXIS9_PID_P
#define AXIS9_PID_P                         0.0            // P 比例系数
  #endif
  #ifndef AXIS9_PID_I
#define AXIS9_PID_I                         0.0            // I 积分系数
  #endif
  #ifndef AXIS9_PID_D
#define AXIS9_PID_D                         0.0            // D 微分系数
  #endif

  #ifndef AXIS9_SERVO_FLTR
#define AXIS9_SERVO_FLTR                    OFF            // 伺服编码器滤波 OFF 为关闭
  #endif
  #ifndef AXIS9_ENCODER
#define AXIS9_ENCODER                       AB             // 编码器类型 AB CW_CCW PULSE_DIR PULSE_ONLY SERIAL_BRIDGE
  #endif
  #ifndef AXIS9_ENCODER_ORIGIN
#define AXIS9_ENCODER_ORIGIN                0              // 绝对编码器原点偏置 支持正负值 使原点计数为 0
  #endif
  #ifndef AXIS9_ENCODER_REVERSE
#define AXIS9_ENCODER_REVERSE               OFF            // 反转编码器计数方向
  #endif
#endif

#if AXIS9_DRIVER_MODEL == KTECH
#define AXIS9_KTECH_PRESENT                                // 轴 9 启用 KTECH 驱动
#endif

#if AXIS9_DRIVER_MODEL == MKS42D
#define AXIS9_MKS42D_PRESENT                               // 轴 9 启用 MKS42D 驱动
#endif

// -----------------------------------------------------------------------------------
// 辅助判断驱动器是否启用
#define DRIVER_CHECK(model) \
    AXIS1_DRIVER_MODEL == model || \
    AXIS2_DRIVER_MODEL == model || \
    AXIS3_DRIVER_MODEL == model || \
    AXIS4_DRIVER_MODEL == model || \
    AXIS5_DRIVER_MODEL == model || \
    AXIS6_DRIVER_MODEL == model || \
    AXIS7_DRIVER_MODEL == model || \
    AXIS8_DRIVER_MODEL == model || \
    AXIS9_DRIVER_MODEL == model

// 标记传统 STEP/DIR 驱动器已启用
#if AXIS1_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS1_DRIVER_MODEL < TMC_DRIVER_FIRST || \
    AXIS2_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS2_DRIVER_MODEL < TMC_DRIVER_FIRST || \
    AXIS3_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS3_DRIVER_MODEL < TMC_DRIVER_FIRST || \
    AXIS4_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS4_DRIVER_MODEL < TMC_DRIVER_FIRST || \
    AXIS5_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS5_DRIVER_MODEL < TMC_DRIVER_FIRST || \
    AXIS6_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS6_DRIVER_MODEL < TMC_DRIVER_FIRST || \
    AXIS7_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS7_DRIVER_MODEL < TMC_DRIVER_FIRST || \
    AXIS8_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS8_DRIVER_MODEL < TMC_DRIVER_FIRST || \
    AXIS9_DRIVER_MODEL >= STEP_DIR_DRIVER_FIRST && AXIS9_DRIVER_MODEL < TMC_DRIVER_FIRST
#define STEP_DIR_LEGACY_PRESENT                            // 启用 STEP DIR LEGACY 支持
#endif

// 标记 STEP/DIR TMC UART 和 SPI 驱动器已启用
#if DRIVER_CHECK(TMC2130)
#define TMC2130_PRESENT                                    // 启用 TMC2130 支持
#endif
#if DRIVER_CHECK(TMC2160)
#define TMC2160_PRESENT                                    // 启用 TMC2160 支持
#endif
#if DRIVER_CHECK(TMC2208)
#define TMC2208_PRESENT                                    // 启用 TMC2208 支持
#endif
#if DRIVER_CHECK(TMC2209)
#define TMC2209_PRESENT                                    // 启用 TMC2209 支持
#endif
#if DRIVER_CHECK(TMC2226)
#define TMC2209_PRESENT                                    // 启用 TMC2209 支持
#endif
#if DRIVER_CHECK(TMC2660)
#define TMC2660_PRESENT                                    // 启用 TMC2660 支持
#endif
#if DRIVER_CHECK(TMC5160)
#define TMC5160_PRESENT                                    // 启用 TMC5160 支持
#endif
#if DRIVER_CHECK(TMC5161)
#define TMC5161_PRESENT                                    // 启用 TMC5161 支持
#endif

#if defined(TMC2130_PRESENT) || defined(TMC2160_PRESENT) || defined(TMC2660_PRESENT) || \
    defined(TMC5160_PRESENT) || defined(TMC5161_PRESENT)
#define STEP_DIR_TMC_SPI_PRESENT                           // 启用 STEP DIR TMC SPI 支持
#endif

#if defined(TMC2208_PRESENT) || defined(TMC2209_PRESENT)
#define STEP_DIR_TMC_UART_PRESENT                          // 启用 STEP DIR TMC UART 支持
#endif

#if defined(STEP_DIR_LEGACY_PRESENT) || defined(STEP_DIR_TMC_SPI_PRESENT) || defined(STEP_DIR_TMC_UART_PRESENT)
#define STEP_DIR_MOTOR_PRESENT                             // 启用 STEP/DIR 电机支持
#endif

// 标记伺服电机已启用
#if DRIVER_CHECK(SERVO_PE)
#define SERVO_PE_PRESENT                                   // 启用 SERVO PE 支持
#endif
#if DRIVER_CHECK(SERVO_EE)
#define SERVO_EE_PRESENT                                   // 启用 SERVO EE 支持
#endif
#if DRIVER_CHECK(SERVO_TMC2130_DC)
#define SERVO_TMC2130_DC_PRESENT                           // 启用 SERVO TMC2130 DC 支持
#endif
#if DRIVER_CHECK(SERVO_TMC5160_DC)
#define SERVO_TMC5160_DC_PRESENT                           // 启用 SERVO TMC5160 DC 支持
#endif
#if DRIVER_CHECK(SERVO_TMC2209)
#define SERVO_TMC2209_PRESENT                              // 启用 SERVO TMC2209 支持
#endif
#if DRIVER_CHECK(SERVO_TMC5160)
#define SERVO_TMC5160_PRESENT                              // 启用 SERVO TMC5160 支持
#endif
#if DRIVER_CHECK(SERVO_KTECH)
#define SERVO_KTECH_PRESENT                                // 启用 SERVO KTECH 支持
#endif

#if defined(SERVO_PE_PRESENT) || defined(SERVO_EE_PRESENT) || defined(SERVO_TMC2130_DC_PRESENT) || \
    defined(SERVO_TMC5160_DC_PRESENT) || defined(SERVO_TMC2209_PRESENT) || defined(SERVO_TMC5160_PRESENT) || \
    defined(SERVO_KTECH_PRESENT)
#define SERVO_MOTOR_PRESENT                                // 启用伺服电机支持
#endif

// 标记 ODRIVE 电机已启用
#if defined(AXIS1_ODRIVE_PRESENT) || defined(AXIS2_ODRIVE_PRESENT)
#define ODRIVE_MOTOR_PRESENT                               // 启用 ODRIVE 电机支持
#endif

// 标记 KTECH 电机已启用
#if DRIVER_CHECK(KTECH)
#define KTECH_MOTOR_PRESENT                                // 启用 KTECH 电机支持
#endif

// 标记 MKS SERVO42D/57D 电机已启用
#if DRIVER_CHECK(MKS42D)
#define MKS42D_MOTOR_PRESENT                               // 启用 MKS SERVO42D/57D 电机支持
#endif

// 标记任意电机已启用
#if defined(SERVO_MOTOR_PRESENT) || defined(STEP_DIR_MOTOR_PRESENT) || defined(ODRIVE_MOTOR_PRESENT) || defined(KTECH_MOTOR_PRESENT)
#define MOTOR_PRESENT                                      // 存在已启用的电机
#endif

// -----------------------------------------------------------------------------------
// 辅助功能设置

#ifndef FEATURE1_PURPOSE
#define FEATURE1_PURPOSE                    OFF            // OFF 或 SWITCH ANALOG_OUT DEW_HEATER INTERVALOMETER
#endif
#ifndef FEATURE1_NAME
#define FEATURE1_NAME                       "FEATURE1"     // 功能显示名称 最多 10 个字符
#endif
#ifndef FEATURE1_TEMP
#define FEATURE1_TEMP                       OFF            // 温度传感器 热敏电阻或 DS1820
#endif
#ifndef FEATURE1_PIN
#define FEATURE1_PIN                        OFF            // 输出控制引脚
#endif
#ifndef FEATURE1_VALUE_DEFAULT
#define FEATURE1_VALUE_DEFAULT              OFF            // 输出控制引脚默认值或状态 ON OFF 或 0 至 255
#endif
#ifndef FEATURE1_VALUE_MEMORY
#define FEATURE1_VALUE_MEMORY               OFF            // ON 断电后保留功能设置值
#endif
#ifndef FEATURE1_ON_STATE
#define FEATURE1_ON_STATE                   HIGH           // 输出控制引脚的有效电平
#endif

#ifndef FEATURE2_PURPOSE
#define FEATURE2_PURPOSE                    OFF            // OFF 或 SWITCH ANALOG_OUT DEW_HEATER INTERVALOMETER
#endif
#ifndef FEATURE2_NAME
#define FEATURE2_NAME                       "FEATURE2"     // 功能显示名称 最多 10 个字符
#endif
#ifndef FEATURE2_TEMP
#define FEATURE2_TEMP                       OFF            // 温度传感器 热敏电阻或 DS1820
#endif
#ifndef FEATURE2_PIN
#define FEATURE2_PIN                        OFF            // 输出控制引脚
#endif
#ifndef FEATURE2_VALUE_DEFAULT
#define FEATURE2_VALUE_DEFAULT              OFF            // 输出控制引脚默认值或状态 ON OFF 或 0 至 255
#endif
#ifndef FEATURE2_VALUE_MEMORY
#define FEATURE2_VALUE_MEMORY               OFF            // ON 断电后保留功能设置值
#endif
#ifndef FEATURE2_ON_STATE
#define FEATURE2_ON_STATE                   HIGH           // 输出控制引脚的有效电平
#endif

#ifndef FEATURE3_PURPOSE
#define FEATURE3_PURPOSE                    OFF            // OFF 或 SWITCH ANALOG_OUT DEW_HEATER INTERVALOMETER
#endif
#ifndef FEATURE3_NAME
#define FEATURE3_NAME                       "FEATURE3"     // 功能显示名称 最多 10 个字符
#endif
#ifndef FEATURE3_TEMP
#define FEATURE3_TEMP                       OFF            // 温度传感器 热敏电阻或 DS1820
#endif
#ifndef FEATURE3_PIN
#define FEATURE3_PIN                        OFF            // 输出控制引脚
#endif
#ifndef FEATURE3_VALUE_DEFAULT
#define FEATURE3_VALUE_DEFAULT              OFF            // 输出控制引脚默认值或状态 ON OFF 或 0 至 255
#endif
#ifndef FEATURE3_VALUE_MEMORY
#define FEATURE3_VALUE_MEMORY               OFF            // ON 断电后保留功能设置值
#endif
#ifndef FEATURE3_ON_STATE
#define FEATURE3_ON_STATE                   HIGH           // 输出控制引脚的有效电平
#endif

#ifndef FEATURE4_PURPOSE
#define FEATURE4_PURPOSE                    OFF            // OFF 或 SWITCH ANALOG_OUT DEW_HEATER INTERVALOMETER
#endif
#ifndef FEATURE4_NAME
#define FEATURE4_NAME                       "FEATURE4"     // 功能显示名称 最多 10 个字符
#endif
#ifndef FEATURE4_TEMP
#define FEATURE4_TEMP                       OFF            // 温度传感器 热敏电阻或 DS1820
#endif
#ifndef FEATURE4_PIN
#define FEATURE4_PIN                        OFF            // 输出控制引脚
#endif
#ifndef FEATURE4_VALUE_DEFAULT
#define FEATURE4_VALUE_DEFAULT              OFF            // 输出控制引脚默认值或状态 ON OFF 或 0 至 255
#endif
#ifndef FEATURE4_VALUE_MEMORY
#define FEATURE4_VALUE_MEMORY               OFF            // ON 断电后保留功能设置值
#endif
#ifndef FEATURE4_ON_STATE
#define FEATURE4_ON_STATE                   HIGH           // 输出控制引脚的有效电平
#endif

#ifndef FEATURE5_PURPOSE
#define FEATURE5_PURPOSE                    OFF            // OFF 或 SWITCH ANALOG_OUT DEW_HEATER INTERVALOMETER
#endif
#ifndef FEATURE5_NAME
#define FEATURE5_NAME                       "FEATURE5"     // 功能显示名称 最多 10 个字符
#endif
#ifndef FEATURE5_TEMP
#define FEATURE5_TEMP                       OFF            // 温度传感器 热敏电阻或 DS1820
#endif
#ifndef FEATURE5_PIN
#define FEATURE5_PIN                        OFF            // 输出控制引脚
#endif
#ifndef FEATURE5_VALUE_DEFAULT
#define FEATURE5_VALUE_DEFAULT              OFF            // 输出控制引脚默认值或状态 ON OFF 或 0 至 255
#endif
#ifndef FEATURE5_VALUE_MEMORY
#define FEATURE5_VALUE_MEMORY               OFF            // ON 断电后保留功能设置值
#endif
#ifndef FEATURE5_ON_STATE
#define FEATURE5_ON_STATE                   HIGH           // 输出控制引脚的有效电平
#endif

#ifndef FEATURE6_PURPOSE
#define FEATURE6_PURPOSE                    OFF            // OFF 或 SWITCH ANALOG_OUT DEW_HEATER INTERVALOMETER
#endif
#ifndef FEATURE6_NAME
#define FEATURE6_NAME                       "FEATURE6"     // 功能显示名称 最多 10 个字符
#endif
#ifndef FEATURE6_TEMP
#define FEATURE6_TEMP                       OFF            // 温度传感器 热敏电阻或 DS1820
#endif
#ifndef FEATURE6_PIN
#define FEATURE6_PIN                        OFF            // 输出控制引脚
#endif
#ifndef FEATURE6_VALUE_DEFAULT
#define FEATURE6_VALUE_DEFAULT              OFF            // 输出控制引脚默认值或状态 ON OFF 或 0 至 255
#endif
#ifndef FEATURE6_VALUE_MEMORY
#define FEATURE6_VALUE_MEMORY               OFF            // ON 断电后保留功能设置值
#endif
#ifndef FEATURE6_ON_STATE
#define FEATURE6_ON_STATE                   HIGH           // 输出控制引脚的有效电平
#endif

#ifndef FEATURE7_PURPOSE
#define FEATURE7_PURPOSE                    OFF            // OFF 或 SWITCH ANALOG_OUT DEW_HEATER INTERVALOMETER
#endif
#ifndef FEATURE7_NAME
#define FEATURE7_NAME                       "FEATURE7"     // 功能显示名称 最多 10 个字符
#endif
#ifndef FEATURE7_TEMP
#define FEATURE7_TEMP                       OFF            // 温度传感器 热敏电阻或 DS1820
#endif
#ifndef FEATURE7_PIN
#define FEATURE7_PIN                        OFF            // 输出控制引脚
#endif
#ifndef FEATURE7_VALUE_DEFAULT
#define FEATURE7_VALUE_DEFAULT              OFF            // 输出控制引脚默认值或状态 ON OFF 或 0 至 255
#endif
#ifndef FEATURE7_VALUE_MEMORY
#define FEATURE7_VALUE_MEMORY               OFF            // ON 断电后保留功能设置值
#endif
#ifndef FEATURE7_ON_STATE
#define FEATURE7_ON_STATE                   HIGH           // 输出控制引脚的有效电平
#endif

#ifndef FEATURE8_PURPOSE
#define FEATURE8_PURPOSE                    OFF            // OFF 或 SWITCH ANALOG_OUT DEW_HEATER INTERVALOMETER
#endif
#ifndef FEATURE8_NAME
#define FEATURE8_NAME                       "FEATURE8"     // 功能显示名称 最多 10 个字符
#endif
#ifndef FEATURE8_TEMP
#define FEATURE8_TEMP                       OFF            // 温度传感器 热敏电阻或 DS1820
#endif
#ifndef FEATURE8_PIN
#define FEATURE8_PIN                        OFF            // 输出控制引脚
#endif
#ifndef FEATURE8_VALUE_DEFAULT
#define FEATURE8_VALUE_DEFAULT              OFF            // 输出控制引脚默认值或状态 ON OFF 或 0 至 255
#endif
#ifndef FEATURE8_VALUE_MEMORY
#define FEATURE8_VALUE_MEMORY               OFF            // ON 断电后保留功能设置值
#endif
#ifndef FEATURE8_ON_STATE
#define FEATURE8_ON_STATE                   HIGH           // 输出控制引脚的有效电平
#endif

// 辅助功能电源监测设置

// 风扇控制
#ifndef FAN_PIN
#define FAN_PIN                             OFF            // 使用支持 PWM 的引脚驱动小风扇为控制器散热
#endif
#ifndef FAN_THRESHOLD_LOW
#define FAN_THRESHOLD_LOW                   35             // 单位摄氏度
#endif
#ifndef FAN_POWER_LOW
#define FAN_POWER_LOW                       60             // %
#endif
#ifndef FAN_THRESHOLD_MID
#define FAN_THRESHOLD_MID                   45             // 单位摄氏度
#endif
#ifndef FAN_POWER_MID
#define FAN_POWER_MID                       80             // %
#endif
#ifndef FAN_THRESHOLD_HIGH
#define FAN_THRESHOLD_HIGH                  50             // 单位摄氏度
#endif
#ifndef FAN_POWER_HIGH
#define FAN_POWER_HIGH                      100            // %
#endif
#ifndef FAN_THRESHOLD_OT
#define FAN_THRESHOLD_OT                    60             // 主控温度超过此值时关闭全部通道 单位摄氏度
#endif
#if defined(FAN_PIN)
#define POWER_MONITOR_FAN_PRESENT                          // 启用散热风扇控制
#endif

// 电压检测
// #define V_SENSE_PINS {OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF}            // 辅助功能 1 至 8 对应的检测引脚数组
#ifndef V_SENSE_FORMULA
#define V_SENSE_FORMULA                     (v*18.405)     // 将通常为 0 至 3.3V 的读数换算为实际电压 使用 47k/2.7k 分压电阻
#endif
#ifndef V_SENSE_LIMIT_LOW
#define V_SENSE_LIMIT_LOW                   10.5           // 标称电压 12V 低于此下限时关闭全部通道
#endif
#ifndef V_SENSE_LIMIT_HIGH
#define V_SENSE_LIMIT_HIGH                  13.8           // 标称电压 12V 高于此上限时关闭全部通道
#endif
#ifndef V_SENSE_LIMIT_EXCLUDE
#define V_SENSE_LIMIT_EXCLUDE               OFF            // 允许排除指定引脚的检查 用于可变电压输出
#endif
#if defined(V_SENSE_PINS) && defined(V_SENSE_FORMULA)
#define POWER_MONITOR_VOLTAGE_PRESENT                      // 启用电压监测
#endif

// 电流检测
// #define I_SENSE_PINS {OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF}            // 辅助功能 1 至 8 对应的检测引脚数组
#ifndef I_SENSE_FORMULA
// 标称供电 3.3V 零电流输出为供电电压一半 按每安培下降 0.09V 换算
#define I_SENSE_FORMULA                     (-((v-1.65)/0.09))
#endif
#ifndef I_SENSE_CHANNEL_MAX
// 单路电流超过此值时关闭对应通道 1 至 8 单位安培
#define I_SENSE_CHANNEL_MAX                 {OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF}
#endif
#ifndef I_SENSE_COMBINED_MAX
#define I_SENSE_COMBINED_MAX                12             // 总电流超过此值时关闭全部通道 单位安培
#endif
#if defined(I_SENSE_PINS) && defined(I_SENSE_FORMULA)
#define POWER_MONITOR_CURRENT_PRESENT                      // 启用电流监测
#endif

#if defined(POWER_MONITOR_VOLTAGE_PRESENT) && defined(POWER_MONITOR_CURRENT_PRESENT)
#define POWER_MONITOR_PRESENT                              // 启用电源监测
#endif

// -----------------------------------------------------------------------------------
// 热敏电阻配置

// 类型 1
#ifndef THERMISTOR1_TNOM
#define THERMISTOR1_TNOM                    25             // 标称温度 单位摄氏度
#endif
#ifndef THERMISTOR1_RNOM
#define THERMISTOR1_RNOM                    10000          // 标称温度下的电阻 单位欧姆
#endif
#ifndef THERMISTOR1_BETA
#define THERMISTOR1_BETA                    3950           // Beta 系数
#endif
#ifndef THERMISTOR1_RSERIES
#define THERMISTOR1_RSERIES                 4700           // 串联电阻 单位欧姆
#endif

// 类型 2
#ifndef THERMISTOR2_TNOM
#define THERMISTOR2_TNOM                    25             // 标称温度 单位摄氏度
#endif
#ifndef THERMISTOR2_RNOM
#define THERMISTOR2_RNOM                    10000          // 标称温度下的电阻 单位欧姆
#endif
#ifndef THERMISTOR2_BETA
#define THERMISTOR2_BETA                    3950           // Beta 系数
#endif
#ifndef THERMISTOR2_RSERIES
#define THERMISTOR2_RSERIES                 4700           // 串联电阻 单位欧姆
#endif

