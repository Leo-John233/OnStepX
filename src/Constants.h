// -----------------------------------------------------------------------------------
// 常量定义
#pragma once

// 配置选项

// 引脚映射型号
#define PINMAP_FIRST                1       // 引脚映射型号编号下限

#define FYSETC_E4                   1       // FYSETC E4 1.0 版 实验支持 可能随时移除

#define FYSETC_S6                   2       // FYSETC S6 1.2 版 六轴 3D 打印机控制板
#define FYSETC_S6_1                 3       // FYSETC S6 1.2 版
#define FYSETC_S6_2                 4       // FYSETC S6 2.0 版 六轴 3D 打印机控制板

#define BTT_SKR_PRO                 5       // BigTreeTech SKR PRO 1.2 版
#define BTT_OCTOPUS_PRO             6       // BigTreeTech Octopus Pro 1.1 版

#define MiniPCB                     7       // 小型双轴板 可嵌入或安装于面板背面 使用 Teensy3.2
#define MiniPCB13                   8       // 1.3 改进版 加强 ESP-01 烧录支持 并提供可选 I2C

#define MiniPCB2                    9       // 适配小型铝壳的双轴板 使用 Teensy3.2

#define MaxPCB                      10      // 第一代定制四轴板 使用 Teensy3.5 或 Teensy3.6
#define MaxPCB2                     11      // 第二代增加铝壳支持 使用 Teensy3.5 或 Teensy3.6
#define MaxPCB3                     12      // 第三代增加四轴 SPI 支持 使用 Teensy4.1
#define MaxPCB4                     13      // 第四代支持 SPI 和 TMC2209 使用 Teensy4.1
#define RESERVED                    14      // 保留

#define MaxSTM3                     15      // 基于 MaxPCB3 改为 Blackpill F411CE 替代 Teensy3.5 或 Teensy3.6
#define MaxSTM3I                    16      // 同上 使用板载 STM32F411CE 默认搭配 M24C64 EEPROM

#define MaxESP3                     17      // 增加第四轴 可通过 OnStep 烧录 WeMos D1 Mini WiFi
#define MaxESP4                     18      // 适用于 ESP32S 支持 TMC2209
#define CNC3                        19      // WeMos D1 R32 ESP32 搭配 Arduino CNC 扩展板
#define MicroScope                  20      // MicroScope ESP32 控制板 实验支持 可能随时移除 使用风险自负

#define STM32Blue                   21      // Khalid 和 Dave 设计的 STM32 Blue pill 控制板 支持 STM32F103CB 和 STM32F303CC

#define JTWSTM                      22      // JTW Astronomy JTWSTM 望远镜控制器 2.1 版
#define MANTICORE                   23      // JTW Astronomy MANTICORE 望远镜控制器 1.0 版

#define SAL_XB1                     24      // SAL-XB1 望远镜控制器

#define PINMAP_LAST                 24      // 引脚映射型号编号上限

// 气象传感器 温度 气压和湿度
#define WEATHER_FIRST               1       // 气象传感器编号下限
#define BME280                      1       // BME280 使用 I2C 默认地址 0x77
#define BME280_0x77                 1       // BME280 使用 I2C 地址 0x77
#define BME280_0x76                 2       // BME280 使用 I2C 地址 0x76
#define BME280_SPI                  3       // BME280 使用 SPI 默认片选
#define BMP280                      4       // BMP280 使用 I2C 默认地址 0x77
#define BMP280_0x77                 4       // BMP280 使用 I2C 地址 0x77
#define BMP280_0x76                 5       // BMP280 使用 I2C 地址 0x76
#define BMP280_SPI                  6       // BMP280 使用 SPI 默认片选
#define WEATHER_LAST                6       // 气象传感器编号上限

// 步进信号波形
#define STEP_WAVE_FORM_FIRST        1       // 步进波形编号下限
#define SQUARE                      1       // 方波步进波形
#define PULSE                       2       // 脉冲步进波形
#define STEP_WAVE_FORM_LAST         2       // 步进波形编号上限

// 架台类型
#define MOUNT_TYPE_FIRST            1       // 架台类型编号下限
#define MOUNT_SUBTYPE_FIRST         1       // 基础架台类型编号下限
#define GEM                         1       // 德式赤道仪 启用中天翻转
#define FORK                        2       // 叉式赤道仪 禁用中天翻转
#define ALTAZM                      3       // 地平式架台 如 Dobson 架台
#define ALTALT                      4       // 双高度轴架台
#define MOUNT_SUBTYPE_LAST          4       // 基础架台类型编号上限
#define GEM_TA                      5       // 德式赤道仪 赤纬使用切线臂
#define GEM_TAC                     6       // 德式赤道仪 赤纬使用切线臂并启用几何修正
#define FORK_TA                     7       // 叉式赤道仪 赤纬使用切线臂
#define FORK_TAC                    8       // 叉式赤道仪 赤纬使用切线臂并启用几何修正
#define ALTAZM_UNL                  9       // 地平式架台 方位轴允许无限旋转
#define MOUNT_TYPE_LAST             9       // 架台类型编号上限

// 架台坐标系
#define MOUNT_COORDS_FIRST          1       // 架台坐标系编号下限
#define OBSERVED_PLACE              1       // 视位置坐标
#define TOPOCENTRIC                 2       // 站心坐标
#define TOPO_STRICT                 3       // 严格站心坐标
#define ASTROMETRIC_J2000           4       // J2000 历元天体测量坐标
#define MOUNT_COORDS_LAST           4       // 架台坐标系编号上限

// 启动位置授权模式
#define SA_STRICT                   1       // 必须由有效来源确认坐标可信
#define SA_AUTO                     2       // 根据绝对位置授权和坐标记忆配置自动决定启动信任
#define SA_PERMISSIVE               3       // 始终信任开机位置

// 支持的时间位置来源设备
#define TLS_FIRST                   1       // 时间位置来源编号下限
#define DS3231                      1       // DS3231 实时时钟 使用 I2C
#define DS3234                      2       // DS3234 实时时钟 使用 SPI 和 DS3234_CS_PIN 基于 Makuna 库
#define SD3031                      3       // SD3031 实时时钟 使用 I2C
#define TEENSY                      4       // TEENSY3.2 内置实时时钟
#define GPS                         5       // GPS 设备
#define NTP                         6       // 网络时间协议 NTP
#define TLS_LAST                    6       // 时间位置来源编号上限

// 镜筒侧向
#define PIER_SIDE_FIRST             1       // 镜筒侧向编号下限
#define EAST                        1       // 等同于 PSS_EAST
#define WEST                        2       // 等同于 PSS_WEST
#define BEST                        3       // 等同于 PSS_BEST
#define AUTOMATIC                   4       // 等同于 PSS_AUTO
#define PIER_SIDE_LAST              4       // 镜筒侧向编号上限

// 中天翻转回原点模式
#define VISIT                       1       // 经过原点后继续
#define PAUSE                       2       // 到达原点后暂停 等待继续指令

// 跟踪补偿
#define COMPENSATED_TRACKING_FIRST  1       // 跟踪补偿编号下限
#define REFRACTION                  1       // 仅赤经轴进行大气折射补偿
#define REFRACTION_DUAL             2       // 双轴进行大气折射补偿
#define MODEL                       3       // 仅赤经轴进行指向模型补偿
#define MODEL_DUAL                  4       // 双轴进行指向模型补偿
#define COMPENSATED_TRACKING_LAST   4       // 跟踪补偿编号上限

// 温度检测设备
#define TEMPERATURE_FIRST           1       // 温度传感器编号下限
#define DS1820                      0x2800000000000000  // DS18B20 单总线温度传感器 用于调焦和加热除露
#define DS18B20                     0x2800000000000000  // 同上
#define DS18S20                     0x1000000000000000  // 同上 但使用 DS18S20
#define DS_MASK                     0x3F00000000000000  // 不可用于 Config.h 配置
#define THERMISTOR                  1       // 通用热敏电阻类型 1 详见 Config.defaults.h
#define THERMISTOR1                 1       // 同上
#define THERMISTOR2                 2       // 通用热敏电阻类型 2 详见 Config.defaults.h
#define TEMPERATURE_LAST            2       // 温度传感器编号上限

// 辅助功能用途
#define AUX_FEATURE_PURPOSE_FIRST   1       // 辅助功能用途编号下限
#define SWITCH                      1       // 控制普通开关
#define ANALOG_OUTPUT               2       // 控制 PWM 模拟输出 取决于主控支持
#define ANALOG_OUT                  2       // 同上
#define DEW_HEATER                  3       // 控制除露加热器
#define INTERVALOMETER              4       // 控制相机快门
#define MOMENTARY_SWITCH            5       // 控制瞬时开关
#define HIDDEN_SWITCH               6       // 控制隐藏开关 用于设置开机引脚状态
#define COVER_SWITCH                7       // 控制舵机镜筒盖 ON 为关闭 OFF 为打开
#define AUX_FEATURE_PURPOSE_LAST    7       // 辅助功能用途编号上限

// --------------------------------------------------------------------------------------------------------------------------

// 任务管理
#define TASKS_MAX                   60      // 最多 60 个任务
#define TASKS_SKIP_MISSED                   // 执行过迟时跳过错过的任务
#ifdef ESP32
  #define TASKS_HWTIMERS            4       // 最多 4 个硬件定时器
#else
  #define TASKS_HWTIMERS            3       // 可用硬件定时器数量
#endif

// 轴类默认起始硬件定时器编号
#define AXIS_HARDWARE_TIMER_BASE    2       // OnStepX 的定时器 1 用于恒星时钟

// 启用库功能
#define SERIAL_LOCAL_PRESENT                // 启用本地串口库支持
#define SERIAL_ST4_SERVER_PRESENT           // 启用 ST4 串行服务端支持

// 非易失存储

// 当前存储卷和分区布局的唯一标识
#define NV_VOLUME_SIGNATURE         0x0001u  // 非易失存储卷布局标识
