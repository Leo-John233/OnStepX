/* ---------------------------------------------------------------------------------------------------------------------------------
 * OnStepX 配置文件
 *
 *          有关 OnStep 设置的更多信息请查看 http://www.stellarjourney.com/index.php?r=site/equipment_onstep
 *                      并加入 OnStep Groups.io 社区 https://groups.io/g/onstep
 * 
 *           *** 请阅读编译器警告和错误信息 这些信息有助于防止无效配置 ***
 *
 * ---------------------------------------------------------------------------------------------------------------------------------
 * 调整以下内容以配置控制器功能 -----------------------------------------------------------------------------------------------
 * <-必填 = 始终必须设置, <-常用 = 通常必须设置, 可选 = 可选设置, 调整 = 按需调整, 少改 = 很少更改
*/
//      参数名称                    值      默认值   说明                                                                        提示

// =================================================================================================================================
// 控制器 ==========================================================================================================================
#define HOST_NAME                "OnStepX" // nStep", 本设备的主机名 最多 16 个字符                                              调整

// 引脚映射 ----------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Controller#PINMAP
#define PINMAP                    MaxESP4 //    OFF, 可选值 MiniPCB, MiniPCB2, MaxPCB4, MaxESP4, MaxSTM3, FYSETC_E4,         <-必填
                                          //         BTT_SKR_PRO 等 其他主板及更多信息请查看 ~/src/Constants.h

// 串口命令通道 -------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Controller#SERIAL_PORTS
#define SERIAL_A_BAUD_DEFAULT        9600 //   9600, n 其中 n=9600,19200,57600,115200,230400,460800 为常用波特率                    少改
#define SERIAL_B_BAUD_DEFAULT        9600 //   9600, n 波特率同上 串口分配请查看 src/pinmaps/                                     少改
#define SERIAL_B_ESP_FLASHING         OFF //    OFF, ON 通过 SERIAL_B 使用 :ESPFLASH# 命令上传 ESP8266 WiFi 固件                    可选
#define SERIAL_C_BAUD_DEFAULT         OFF //    OFF, n 波特率同上 串口分配请查看 src/pinmaps/                                     少改
#define SERIAL_D_BAUD_DEFAULT         OFF //    OFF, n 波特率同上 串口分配请查看 src/pinmaps/                                     少改
#define SERIAL_E_BAUD_DEFAULT         OFF //    OFF, n 波特率同上 串口分配请查看 src/pinmaps/                                     少改
#define SERIAL_RADIO            BLUETOOTH //    OFF, 使用 BLUETOOTH 或 WIFI_ACCESS_POINT 或 WIFI_STATION 仅支持 ESP32              可选

// 状态指示 ----------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Controller#STATUS_LED
#define STATUS_LED                    OFF //    OFF, 无错误时常亮 否则按错误代码闪烁                                             可选

// 分划板控制 --------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Controller#RETICLE_CONTROL
#define RETICLE_LED_DEFAULT           OFF //    OFF, n 其中 n=0..255 对应 0..100% 启用功能并设置默认亮度                           可选
#define RETICLE_LED_MEMORY            OFF //    OFF, ON 断电后保留分划板亮度设置                                                 可选
#define RETICLE_LED_INVERT            OFF //    OFF, ON 反转控制逻辑 适用于 0V 表示最大亮度的情况                                  可选

// 气象传感器 --------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Controller#WEATHER_SENSOR
#define WEATHER                       OFF //    OFF, BME280 I2C 0x77, BME280_0x76, BME280_SPI 片选引脚请查看引脚映射                可选
                                          //         BMP280 I2C 0x77, BMP280_0x76, BMP280_SPI 片选引脚请查看引脚映射
                                          //         BME280 或 BMP280 用于测量温度和气压 BME280 还可测量湿度

// 信号输出 ----------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Controller#SIGNALING
#define STEP_WAVE_FORM             SQUARE // SQUARE, PULSE 高速运行时的步进信号波形 SQUARE 的信号完整性最佳                           调整
                                          //         适用于所有轴

// 非易失性存储器 ---------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Controller#NV
#define NV_DRIVER              NV_DEFAULT // NV_DEF, 使用平台默认的非易失性存储设备保存运行时设置                                 可选

// =================================================================================================================================
// 赤道仪架台 =======================================================================================================================

// 许多驱动器型号 步进方向和伺服 有特定要求 请务必查看下方链接了解详情
// 常见型号 A4988, DRV8825, LV8729, S109, TMC2130, TMC5160, TMC2209 等

// 轴1 赤经或方位 ----------------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Axes
#define AXIS1_DRIVER_MODEL        TMC2209 //    OFF, 在两个轴中填写上方列出的电机驱动器型号以启用架台                                <-常用

// 如果已启用轴运行时设置 则会忽略下方配置 可在 SWS 中禁用或清除 NV/EEPROM
// \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ 
#define AXIS1_STEPS_PER_DEGREE      12800 //  12800, n 每度步数                                                              <-必填
                                          //         n = (stepper_steps * micro_steps * overall_gear_reduction)/360.0
#define AXIS1_REVERSE                 OFF //    OFF, ON 反转运动方向 也可通过反接电机线修正方向                                   <-常用
#define AXIS1_LIMIT_MIN              -180 //   -180, n 其中 n=-90..-360 单位为度 表示最小赤经时角或方位角                          调整
#define AXIS1_LIMIT_MAX               180 //    180, n 其中 n=90..360 单位为度 表示最大赤经时角或方位角                            调整
#define AXIS1_LIMIT_SYNC              OFF //    OFF, n 其中 n=0..90 单位为度 仅允许在此正负范围内同步或复位                          可选

#define AXIS1_DRIVER_MICROSTEPS       OFF //    OFF, n 默认微步模式                                                          <-必填
#define AXIS1_DRIVER_MICROSTEPS_GOTO  OFF //    OFF, n 覆盖自动寻星微步模式 OFF 使用 _DRIVER_MICROSTEPS                           可选

// 适用于 TMC2130, TMC5160, TMC2209, TMC2226 步进方向驱动器型号
#define AXIS1_DRIVER_IHOLD            OFF //    OFF, n 单位为 mA 静止电流 OFF 使用 IRUN/2.0                                      可选
#define AXIS1_DRIVER_IRUN             OFF //    OFF, n 单位为 mA 默认运行电流 应与步进电机和驱动器等匹配                              可选
#define AXIS1_DRIVER_IGOTO            OFF //    OFF, n 单位为 mA 覆盖转动时的运行电流 OFF 使用 IRUN                                可选
// /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /

#define AXIS1_DRIVER_STATUS            ON //    OFF, ON, HIGH 或 LOW 用于驱动器状态信息和故障检测                                  可选

#define AXIS1_DRIVER_DECAY            OFF //    OFF, 覆盖跟踪时的默认衰减模式 TMC 默认为 STEALTHCHOP                               少改
#define AXIS1_DRIVER_DECAY_GOTO       OFF //    OFF, 覆盖自动寻星时的默认衰减模式 TMC 默认为 SPREADCYCLE                            少改

#define AXIS1_POWER_DOWN              OFF //    OFF, ON 运动停止 30 秒后关闭电源 或最后一次不超过 1 倍速导星 10 分钟后关闭电源              少改

#define AXIS1_SENSE_HOME              OFF //    OFF, HIGH 或 LOW 用于启用并指定从正面观察时顺时针方向的原点状态                         可选
#define AXIS1_SENSE_LIMIT_MIN LIMIT_SENSE // ...NSE, 限位感应开关为 HIGH 或 LOW 状态时停止运动                                     可选
#define AXIS1_SENSE_LIMIT_MAX LIMIT_SENSE // ...NSE, 限位感应开关为 HIGH 或 LOW 状态时停止运动                                     可选
                                          //         数字输入可选添加 |HYST(n) 其中 n=0..1023 表示稳定时间 单位为 ms
                                          //         支持模拟量的感应输入还可添加
                                          //         |THLD(n) 其中 n=1..1023 ADU 表示模拟阈值
                                          //         |HYST(n) 其中 n=0..1023 ADU 表示正负迟滞范围

// 轴2 赤纬或高度 ----------------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Axes
#define AXIS2_DRIVER_MODEL        TMC2209 //    OFF, 在两个轴中填写上方列出的电机驱动器型号以启用架台                                <-常用

// 如果已启用轴运行时设置 则会忽略下方配置 可在 SWS 中禁用或清除 NV/EEPROM
// \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/
#define AXIS2_STEPS_PER_DEGREE      12800 //  12800, n 每度步数                                                              <-必填
                                          //         n = (stepper_steps * micro_steps * overall_gear_reduction)/360.0
#define AXIS2_REVERSE                 OFF //    OFF, ON 反转运动方向 也可通过反接电机线修正方向                                   <-常用
#define AXIS2_LIMIT_MIN               -90 //    -90, n 其中 n=-90..0 单位为度 表示允许的最小赤纬或高度角                          少改
#define AXIS2_LIMIT_MAX                90 //     90, n 其中 n=0..90 单位为度 表示允许的最大赤纬或高度角                            少改
#define AXIS2_LIMIT_SYNC              OFF //    OFF, n 其中 n=0..90 单位为度 仅允许在此正负范围内同步或复位                          可选

#define AXIS2_DRIVER_MICROSTEPS       OFF //    OFF, n 默认微步模式                                                          <-必填
#define AXIS2_DRIVER_MICROSTEPS_GOTO  OFF //    OFF, n 覆盖自动寻星微步模式 OFF 使用 _DRIVER_MICROSTEPS                           可选

// 适用于 TMC2130, TMC5160, TMC2209, TMC2226 步进方向驱动器型号
#define AXIS2_DRIVER_IHOLD            OFF //    OFF, n 单位为 mA 静止电流 OFF 使用 IRUN/2.0                                      可选
#define AXIS2_DRIVER_IRUN             OFF //    OFF, n 单位为 mA 默认运行电流 应与步进电机和驱动器等匹配                              可选
#define AXIS2_DRIVER_IGOTO            OFF //    OFF, n 单位为 mA 覆盖转动时的运行电流 OFF 使用 IRUN                                可选
// /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /

#define AXIS2_DRIVER_STATUS            ON //    OFF, ON, HIGH 或 LOW 轮询驱动器状态信息和故障检测                                  可选

#define AXIS2_DRIVER_DECAY            OFF //    OFF, 覆盖跟踪时的默认衰减模式 TMC 默认为 STEALTHCHOP                               少改
#define AXIS2_DRIVER_DECAY_GOTO       OFF //    OFF, 覆盖自动寻星时的默认衰减模式 TMC 默认为 SPREADCYCLE                            少改

#define AXIS2_POWER_DOWN              OFF //    OFF, ON 运动停止 30 秒后关闭电源 或最后一次不超过 1 倍速导星 10 分钟后关闭电源              可选

#define AXIS2_SENSE_HOME              OFF //    OFF, HIGH 或 LOW 用于启用并指定从上方观察时顺时针方向的原点状态                         可选
#define AXIS2_SENSE_LIMIT_MIN LIMIT_SENSE // ...NSE, 限位感应开关为 HIGH 或 LOW 状态时停止运动                                     可选
#define AXIS2_SENSE_LIMIT_MAX LIMIT_SENSE // ...NSE, 限位感应开关为 HIGH 或 LOW 状态时停止运动                                     可选

// 架台类型 ------------------------------------------------------ 查看 https://onstep.groups.io/g/main/wiki/Configuration_Mount#MOUNT
#define MOUNT_TYPE                    GEM //    GEM, GEM         德式赤道仪等需要执行中天翻转的架台                                 <-必填
                                          //         GEM_TA      带赤纬切线臂的 GEM
                                          //         GEM_TAC     带赤纬切线臂和几何校正的 GEM
                                          //         FORK        叉式架台
                                          //         FORK_TA     带赤纬切线臂的 FORK
                                          //         FORK_TAC    带赤纬切线臂和几何校正的 FORK
                                          //         ALTAZM      地平式架台和多布森式等
                                          //         ALTAZM_UNL  方位轴可无限旋转的 ALTAZM

#define MOUNT_ALTERNATE_ORIENTATION   OFF //    OFF, ON 允许 FORK 架台执行中天翻转 并允许 ALTAZM 架台越过天顶                         可选
                                          //         GEM 模式忽略此设置

#define MOUNT_STARTUP_MODE        SA_AUTO // ..AUTO, SA_STRICT 或 SA_PERMISSIVE 控制启动时何时授予位置可信状态                       可选
                                          //         仅当没有绝对位置源且坐标记忆为 OFF 时
                                          //         SA_AUTO 才保留旧版立即自动寻星行为

#define MOUNT_COORDS          TOPOCENTRIC // ...RIC, 对 OnStep 输入输出的坐标应用大气折射修正 但精确位于两极时除外                     少改
                                          //              如需在两极也应用大气折射修正 请使用 TOPO_STRICT
                                          //              如不使用大气折射修正 请使用 OBSERVED_PLACE

#define MOUNT_COORDS_MEMORY           OFF //    OFF, ON 断电后保留架台的近似坐标                                                 可选
                                          //         需要 FRAM 非易失性存储器 且断电期间架台不得移动

#define MOUNT_ENABLE_IN_STANDBY       OFF //    OFF, ON 在待机状态下启用架台电机驱动器                                           少改

// 时间和位置 ---------------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Mount#TLS
#define TIME_LOCATION_SOURCE          OFF //    OFF, DS3231 I2C, SD3031 I2C, TEENSY T3.2 等, GPS 或 NTP 源                       可选
                                          //         提供日期和时间 可用时还提供 PPS 和经纬度
#define TIME_LOCATION_PPS_SENSE       OFF //    OFF, HIGH 检测 PPS 每秒脉冲信号上升沿 LOW 检测下降沿                              可选
                                          //         BOTH 同时检测上升沿和下降沿

// 状态指示 ------------------------------------------------------ 查看 https://onstep.groups.io/g/main/wiki/Configuration_Mount#STATUS
#define STATUS_MOUNT_LED              OFF //    OFF, ON 按运动速率成比例闪烁 转动时常亮                                           可选
#define STATUS_BUZZER                 OFF //    OFF, ON, n 其中 n=100..6000 为扬声器频率 单位为 Hz ON 用于压电蜂鸣器                    可选
#define STATUS_BUZZER_DEFAULT         OFF //    OFF, ON 启动时启用蜂鸣器声音                                                   可选
#define STATUS_BUZZER_MEMORY          OFF //    OFF, ON 断电后保留蜂鸣器声音设置                                                可选

// ST4 接口 ------------------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Mount#ST4
// *** 请自行确认接口符合所有已连接设备的电气规范 使用风险由用户自行承担 ***
#define ST4_INTERFACE                 OFF //    OFF, ON 启用接口 非手控模式下导星速率不超过 1 倍速                                  可选
                                          //         自动寻星期间按下按钮可中止转动 或继续执行中天翻转在原点的暂停
#define ST4_HAND_CONTROL               ON //     ON, ON 启用手控器特殊功能和 SHC 支持                                           可选
                                          //         同时按住 [E]+[W] 超过 2 秒 导星速率 [E]- [W]+ [N] 跟踪开关 [S] 同步
                                          //         同时按住 [N]+[S] 超过 2 秒 用户目录项 [E]- [W]+ [N] 自动寻星 [S] 声音开关
#define ST4_HAND_CONTROL_FOCUSER       ON //     ON, ON 使用上述功能的替代模式 调焦器移动 [E]f1 [W]f2 [N]- [S]+                     可选

// 导星行为 ------------------------------------------------------ 查看 https://onstep.groups.io/g/main/wiki/Configuration_Mount#GUIDING
#define GUIDE_TIME_LIMIT               10 //     10, n 时间限制 n=0..120 秒 使用 0 禁用                                          调整
#define GUIDE_DISABLE_BACKLASH        OFF //    OFF, 在不超过 1 倍速导星时禁用回差补偿                                           可选

// 限位 ---------------------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Mount#LIMITS
#define LIMIT_SENSE                   OFF //    OFF, 限位感应开关为 HIGH 或 LOW 状态时停止运动                                     可选
#define LIMIT_RECOVERY                OFF //    OFF, 暂时禁用限位 1 秒 以允许移动到限位外 从而脱离天顶 地平线 中天 GEM                    可选
                                          //         以及轴1最小或最大限位
#define LIMIT_RECOVERY_WITH_TRACKING  OFF //    OFF, 从限位状态恢复时自动启用跟踪                                                可选

// 停放 ---------------------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Mount#PARKING
#define PARK_SENSE                    OFF //    OFF, HIGH 或 LOW 状态表示架台处于停放方向                                         可选
#define PARK_SIGNAL                   OFF //    OFF, HIGH 或 LOW 状态的停放输入信号触发停放                                       可选
#define PARK_STATUS                   OFF //    OFF, 成功停放后以 HIGH 或 LOW 状态输出信号                                        可选

// 周期误差修正 -------------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Mount#PEC
#define PEC_STEPS_PER_WORM_ROTATION     0 //      0, n 蜗杆每转的步数 0 表示禁用 其他值会分配 720 秒缓冲区                            <-必填
                                          //         n = (AXIS1_STEPS_PER_DEGREE*360)/reduction_final_stage

#define PEC_SENSE                     OFF //    OFF, HIGH 检测 PEC 信号上升沿 LOW 检测下降沿                                    可选
                                          //         在 ALTAZM 模式下忽略
#define PEC_BUFFER_SIZE_LIMIT         720 //    720, 允许的 PEC 缓冲区秒数                                                     少改

// 跟踪行为 ------------------------------------------------------ 查看 https://onstep.groups.io/g/main/wiki/Configuration_Mount#TRACKING
#define TRACK_BACKLASH_RATE            20 //     20, n 其中 n=2..50 表示回差补偿期间的恒星时速率倍数                                 可选
                                          //         过快会导致电机失步或齿轮撞击 过慢会使回差补偿迟缓
#define TRACK_AUTOSTART               OFF //    OFF, ON 启动时自动启用跟踪                                                    可选
#define TRACK_COMPENSATION_DEFAULT    OFF //    OFF, 不补偿 或 REFRACTION, REFRACTION_DUAL, MODEL, MODEL_DUAL                    可选
#define TRACK_COMPENSATION_MEMORY     OFF //    OFF, ON 保留大气折射或指向模型补偿跟踪设置                                       可选

// 转动行为 ------------------------------------------------------ 查看 https://onstep.groups.io/g/main/wiki/Configuration_Mount#SLEWING
#define SLEW_RATE_BASE_DESIRED        1.0 //    1.0, n 期望转动速率 单位为度每秒 运行时可在此速率的                                <-必填
                                          //         1/2 至 2 倍范围内调整 并应考虑性能要求
#define SLEW_RATE_MEMORY              OFF //    OFF, ON 断电后保留速率设置                                                    可选
#define SLEW_ACCELERATION_DIST        5.0 //    5.0, n 单位为度 加速和减速所需的大致距离                                         调整
#define SLEW_RAPID_STOP_DIST          2.0 //    2.0, n 单位为度 中止转动或超过限位时停止所需的大致距离                              调整
#define GOTO_FEATURE                   ON //     ON, 使用 OFF 禁用架台自动寻星功能                                             少改
#define GOTO_OFFSET                  0.25 //   0.25, 自动寻星目标单向接近的偏移量 单位为度 0.0 表示禁用                               调整
#define GOTO_OFFSET_ALIGN             OFF //    OFF, ON 跳过校准星自动寻星的最后阶段 以便用户在居中时通常从正确方向接近                    可选

// 基座侧行为 ---------------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Mount#PIERSIDE
#define MFLIP_HOME_DEFAULT          VISIT //  VISIT, OFF, VISIT, PAUSE 选择中天翻转时的原点路径点行为                               可选
                                          //         OFF 直接前往 VISIT 经过原点 PAUSE 在原点等待
#define MFLIP_HOME_MEMORY             OFF //    OFF, ON 断电后保留中天翻转原点模式设置                                         可选
#define MFLIP_AUTOMATIC_DEFAULT       OFF //    OFF, ON 启动时启用自动中天翻转                                                可选
#define MFLIP_AUTOMATIC_MEMORY        OFF //    OFF, ON 断电后保留自动中天翻转设置                                             可选

#define PIER_SIDE_SYNC_CHANGE_SIDES   OFF //    OFF, ON 允许 GEM 架台在同步时改变基座侧                                         可选
#define PIER_SIDE_PREFERRED_DEFAULT  BEST //   BEST, BEST 尽可能保持当前侧 EAST 或 WEST 则尽可能切换到指定侧                       可选
                                          //         AUTOMATIC 在接近下极子午线时优先西侧 否则优先东侧                                可选
#define PIER_SIDE_PREFERRED_MEMORY    OFF //    OFF, ON 断电后保留首选基座侧设置                                               可选

// 校准 ---------------------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Mount#ALIGN
#define ALIGN_AUTO_HOME               OFF //    OFF, ON 开始校准时先使用原点开关寻找原点                                         可选
#define ALIGN_MODEL_MEMORY            OFF //    OFF, ON 启动时恢复保存在 NV 中的指向模型                                        可选
#define ALIGN_MAX_STARS              AUTO //   AUTO, 使用 HAL 指定的默认值 6 颗或 9 颗星                                        少改
                                          //         也可使用 n 其中 n=1 仅用于同步 或 3 到 9 用于自动寻星辅助

// =================================================================================================================================
// 旋转器 ==========================================================================================================================

// 许多驱动器型号 步进方向和伺服 有特定要求 请务必查看下方链接了解详情
// 常见型号 A4988, DRV8825, LV8729, S109, TMC2130S 等

// 轴3 旋转器 ----------------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Rotator
#define AXIS3_DRIVER_MODEL            OFF //    OFF, 填写上方列出的电机驱动器型号以启用旋转器                                     可选
#define AXIS3_SLEW_RATE_BASE_DESIRED  1.0 //    1.0, n 期望转动速率 单位为度每秒 运行时可在此速率的                                <-必填
                                          //         1/2 至 2 倍范围内调整 并应考虑性能要求

// 如果已启用轴运行时设置 则会忽略下方配置 可在 SWS 中禁用或清除 NV/EEPROM
// \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/
#define AXIS3_STEPS_PER_DEGREE       64.0 //   64.0, n 旋转器或消旋器的每度步数                                                 调整
                                          //         高度方位消旋 n = (圆周像素数 * 2)/360 为最小值
#define AXIS3_REVERSE                 OFF //    OFF, ON 反转运动方向 也可通过反接电机线修正方向                                    可选
#define AXIS3_LIMIT_MIN                 0 //      0, n 其中 n=-360..0 单位为度 表示允许的最小旋转器角度                          少改
#define AXIS3_LIMIT_MAX               360 //    360, n 其中 n=0..360 单位为度 表示允许的最大旋转器角度                            少改

#define AXIS3_DRIVER_MICROSTEPS       OFF //    OFF, n 默认微步模式                                                           可选
#define AXIS3_DRIVER_MICROSTEPS_GOTO  OFF //    OFF, n 覆盖自动寻星微步模式 OFF 使用 _DRIVER_MICROSTEPS                           可选

// 适用于 TMC2130, TMC5160, TMC2209, TMC2226 步进方向驱动器型号
#define AXIS3_DRIVER_IHOLD            OFF //    OFF, n 单位为 mA 静止电流 OFF 使用 IRUN/2.0                                      可选
#define AXIS3_DRIVER_IRUN             OFF //    OFF, n 单位为 mA 默认运行电流 应与步进电机和驱动器等匹配                              可选
#define AXIS3_DRIVER_IGOTO            OFF //    OFF, n 单位为 mA 覆盖转动时的运行电流 OFF 使用 IRUN                                可选
// /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /

#define AXIS3_DRIVER_STATUS           OFF //    OFF, ON, HIGH 或 LOW 用于驱动器状态信息和故障检测                                  可选

#define AXIS3_DRIVER_DECAY            OFF //    OFF, 覆盖跟踪时的默认衰减模式 TMC 默认为 STEALTHCHOP                               少改
#define AXIS3_DRIVER_DECAY_GOTO       OFF //    OFF, 覆盖自动寻星时的默认衰减模式 TMC 默认为 SPREADCYCLE                            少改

#define AXIS3_POWER_DOWN              OFF //    OFF, ON 运动停止 30 秒后关闭电源                                               可选

#define AXIS3_SENSE_HOME              OFF //    OFF, HIGH 或 LOW 用于启用并指定从上方观察时顺时针方向的原点状态                         可选
#define AXIS3_SENSE_LIMIT_MIN         OFF //    OFF, 限位感应开关为 HIGH 或 LOW 状态时停止运动                                     可选
#define AXIS3_SENSE_LIMIT_MAX         OFF //    OFF, 限位感应开关为 HIGH 或 LOW 状态时停止运动                                     可选

// =================================================================================================================================
// 调焦器 ==========================================================================================================================

// 许多驱动器型号 步进方向和伺服 有特定要求 请务必查看下方链接了解详情
// 常见型号 A4988, DRV8825, LV8729, S109, TMC2130S 等

// 轴4 调焦器1 ---------------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Focuser
#define AXIS4_DRIVER_MODEL            OFF //    OFF, 填写上方列出的电机驱动器型号以启用调焦器                                     可选
#define AXIS4_SLEW_RATE_BASE_DESIRED  500 //    500, n 其中 n=200..5000 单位为 um/s 运行时可在此速率的                          <-必填
                                          //         1/2 至 2 倍范围内调整 并应考虑性能要求
#define AXIS4_SLEW_RATE_MINIMUM        20 //     20, n 其中 n=5..200 单位为 um/s 表示最小微米每秒速率                            调整

// 如果已启用轴运行时设置 则会忽略下方配置 可在 SWS 中禁用或清除 NV/EEPROM
// \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/ \/
#define AXIS4_STEPS_PER_MICRON        0.5 //    0.5, n 每微米步数 可通过测试或其他方法确定                                       调整
#define AXIS4_REVERSE                 OFF //    OFF, ON 反转运动方向 也可通过反接电机线修正方向                                    可选
#define AXIS4_LIMIT_MIN                 0 //      0, n 其中 n=0..500 单位为毫米 表示允许的最小位置                              调整
#define AXIS4_LIMIT_MAX                50 //     50, n 其中 n=0..500 单位为毫米 表示允许的最大位置                              调整

#define AXIS4_DRIVER_MICROSTEPS       OFF //    OFF, n 默认微步模式                                                           可选
#define AXIS4_DRIVER_MICROSTEPS_GOTO  OFF //    OFF, n 覆盖自动寻星微步模式 OFF 使用 _DRIVER_MICROSTEPS                           可选

// 适用于 TMC2130, TMC5160, TMC2209, TMC2226 步进方向驱动器型号
#define AXIS4_DRIVER_IHOLD            OFF //    OFF, n 单位为 mA 静止电流 OFF 使用 IRUN/2.0                                      可选
#define AXIS4_DRIVER_IRUN             OFF //    OFF, n 单位为 mA 默认运行电流 应与步进电机和驱动器等匹配                              可选
#define AXIS4_DRIVER_IGOTO            OFF //    OFF, n 单位为 mA 覆盖转动时的运行电流 OFF 使用 IRUN                                可选
// /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /\ /

#define AXIS4_DRIVER_STATUS           OFF //    OFF, ON, HIGH 或 LOW 用于驱动器状态信息和故障检测                                  可选

#define AXIS4_DRIVER_DECAY            OFF //    OFF, 覆盖跟踪时的默认衰减模式 TMC 默认为 STEALTHCHOP                               少改
#define AXIS4_DRIVER_DECAY_GOTO       OFF //    OFF, 覆盖自动寻星时的默认衰减模式 TMC 默认为 SPREADCYCLE                            少改

#define AXIS4_POWER_DOWN              OFF //    OFF, ON 运动停止 30 秒后关闭电源                                               可选

#define AXIS4_SENSE_HOME              OFF //    OFF, HIGH 或 LOW 用于启用并指定从上方观察时顺时针方向的原点状态                         可选
#define AXIS4_SENSE_LIMIT_MIN         OFF //    OFF, 限位感应开关为 HIGH 或 LOW 状态时停止运动                                     可选
#define AXIS4_SENSE_LIMIT_MAX         OFF //    OFF, 限位感应开关为 HIGH 或 LOW 状态时停止运动                                     可选

// 轴5 调焦器2 ----------------------------------------------------------------------------------------------------------------------
// 最多可配置 6 个调焦器 AXIS4 至 AXIS9 只需复制上方调焦器1的内容 并将调焦器2的名称改为 AXIS5_ 其余依此类推

// 调焦器温度 ---------------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Focuser
#define FOCUSER_TEMPERATURE           OFF //    OFF, THERMISTOR 或 n 其中 n 为调焦器温度传感器 ds18b20 的序列号                         调整

// =================================================================================================================================
// 辅助功能 =========================================================================================================================

// 功能 --------------------------------------------------------------- 查看 https://onstep.groups.io/g/main/wiki/Configuration_Aux
// 注意 临时将 DEBUG 模式设为 VERBOSE 并使用 "FEATURE1_TEMP DS1820" 列出 DS18B20 设备序列号

#define FEATURE1_PURPOSE              OFF //    OFF, SWITCH, MOMENTARY_SWITCH, ANALOG_OUT, DEW_HEATER, INTERVALOMETER         可选
#define FEATURE1_NAME          "FEATURE1" // "FE..", 受控功能的名称                                                              调整
#define FEATURE1_TEMP                 OFF //    OFF, THERMISTOR 或 n 其中 n 为 ds18b20 序列号 用于 DEW_HEATER 温度                   调整
#define FEATURE1_PIN                  OFF //    OFF, AUX 表示辅助引脚 n 表示引脚编号                                             调整
#define FEATURE1_VALUE_DEFAULT        OFF //    OFF, ON, n 其中 n=0..255 用于 ANALOG_OUT 功能                                   调整
#define FEATURE1_VALUE_MEMORY         OFF //    OFF, ON 断电后保留 SWITCH, ANALOG_OUT, DEW_HEATER 状态                          调整
#define FEATURE1_ON_STATE            HIGH //   HIGH, LOW 表示反转逻辑 使 ON 为 0V 而 OFF 为 Vcc 通常为 3.3V                         调整

#define FEATURE2_PURPOSE              OFF //    OFF, SWITCH, MOMENTARY_SWITCH, ANALOG_OUT, DEW_HEATER, INTERVALOMETER         可选
#define FEATURE2_NAME          "FEATURE2" // "FE..", 受控功能的名称                                                              调整
#define FEATURE2_TEMP                 OFF //    OFF, THERMISTOR 或 n 其中 n 为 ds18b20 序列号 用于 DEW_HEATER 温度                   调整
#define FEATURE2_PIN                  OFF //    OFF, AUX 表示辅助引脚 n 表示引脚编号                                             调整
#define FEATURE2_VALUE_DEFAULT        OFF //    OFF, ON, n 其中 n=0..255 用于 ANALOG_OUT 功能                                   调整
#define FEATURE2_VALUE_MEMORY         OFF //    OFF, ON 断电后保留 SWITCH, ANALOG_OUT, DEW_HEATER 状态                          调整
#define FEATURE2_ON_STATE            HIGH //   HIGH, LOW 表示反转逻辑 使 ON 为 0V 而 OFF 为 Vcc 通常为 3.3V                         调整

#define FEATURE3_PURPOSE              OFF //    OFF, SWITCH, MOMENTARY_SWITCH, ANALOG_OUT, DEW_HEATER, INTERVALOMETER         可选
#define FEATURE3_NAME          "FEATURE3" // "FE..", 受控功能的名称                                                              调整
#define FEATURE3_TEMP                 OFF //    OFF, THERMISTOR 或 n 其中 n 为 ds18b20 序列号 用于 DEW_HEATER 温度                   调整
#define FEATURE3_PIN                  OFF //    OFF, AUX 表示辅助引脚 n 表示引脚编号                                             调整
#define FEATURE3_VALUE_DEFAULT        OFF //    OFF, ON, n 其中 n=0..255 用于 ANALOG_OUT 功能                                   调整
#define FEATURE3_VALUE_MEMORY         OFF //    OFF, ON 断电后保留 SWITCH, ANALOG_OUT, DEW_HEATER 状态                          调整
#define FEATURE3_ON_STATE            HIGH //   HIGH, LOW 表示反转逻辑 使 ON 为 0V 而 OFF 为 Vcc 通常为 3.3V                         调整

#define FEATURE4_PURPOSE              OFF //    OFF, SWITCH, MOMENTARY_SWITCH, ANALOG_OUT, DEW_HEATER, INTERVALOMETER         可选
#define FEATURE4_NAME          "FEATURE4" // "FE..", 受控功能的名称                                                              调整
#define FEATURE4_TEMP                 OFF //    OFF, THERMISTOR 或 n 其中 n 为 ds18b20 序列号 用于 DEW_HEATER 温度                   调整
#define FEATURE4_PIN                  OFF //    OFF, AUX 表示辅助引脚 n 表示引脚编号                                             调整
#define FEATURE4_VALUE_DEFAULT        OFF //    OFF, ON, n 其中 n=0..255 用于 ANALOG_OUT 功能                                   调整
#define FEATURE4_VALUE_MEMORY         OFF //    OFF, ON 断电后保留 SWITCH, ANALOG_OUT, DEW_HEATER 状态                          调整
#define FEATURE4_ON_STATE            HIGH //   HIGH, LOW 表示反转逻辑 使 ON 为 0V 而 OFF 为 Vcc 通常为 3.3V                         调整

#define FEATURE5_PURPOSE              OFF //    OFF, SWITCH, MOMENTARY_SWITCH, ANALOG_OUT, DEW_HEATER, INTERVALOMETER         可选
#define FEATURE5_NAME          "FEATURE5" // "FE..", 受控功能的名称                                                              调整
#define FEATURE5_TEMP                 OFF //    OFF, THERMISTOR 或 n 其中 n 为 ds18b20 序列号 用于 DEW_HEATER 温度                   调整
#define FEATURE5_PIN                  OFF //    OFF, AUX 表示辅助引脚 n 表示引脚编号                                             调整
#define FEATURE5_VALUE_DEFAULT        OFF //    OFF, ON, n 其中 n=0..255 用于 ANALOG_OUT 功能                                   调整
#define FEATURE5_VALUE_MEMORY         OFF //    OFF, ON 断电后保留 SWITCH, ANALOG_OUT, DEW_HEATER 状态                          调整
#define FEATURE5_ON_STATE            HIGH //   HIGH, LOW 表示反转逻辑 使 ON 为 0V 而 OFF 为 Vcc 通常为 3.3V                         调整

#define FEATURE6_PURPOSE              OFF //    OFF, SWITCH, MOMENTARY_SWITCH, ANALOG_OUT, DEW_HEATER, INTERVALOMETER         可选
#define FEATURE6_NAME          "FEATURE6" // "FE..", 受控功能的名称                                                              调整
#define FEATURE6_TEMP                 OFF //    OFF, THERMISTOR 或 n 其中 n 为 ds18b20 序列号 用于 DEW_HEATER 温度                   调整
#define FEATURE6_PIN                  OFF //    OFF, AUX 表示辅助引脚 n 表示引脚编号                                             调整
#define FEATURE6_VALUE_DEFAULT        OFF //    OFF, ON, n 其中 n=0..255 用于 ANALOG_OUT 功能                                   调整
#define FEATURE6_VALUE_MEMORY         OFF //    OFF, ON 断电后保留 SWITCH, ANALOG_OUT, DEW_HEATER 状态                          调整
#define FEATURE6_ON_STATE            HIGH //   HIGH, LOW 表示反转逻辑 使 ON 为 0V 而 OFF 为 Vcc 通常为 3.3V                         调整

#define FEATURE7_PURPOSE              OFF //    OFF, SWITCH, MOMENTARY_SWITCH, ANALOG_OUT, DEW_HEATER, INTERVALOMETER         可选
#define FEATURE7_NAME          "FEATURE7" // "FE..", 受控功能的名称                                                              调整
#define FEATURE7_TEMP                 OFF //    OFF, THERMISTOR 或 n 其中 n 为 ds18b20 序列号 用于 DEW_HEATER 温度                   调整
#define FEATURE7_PIN                  OFF //    OFF, AUX 表示辅助引脚 n 表示引脚编号                                             调整
#define FEATURE7_VALUE_DEFAULT        OFF //    OFF, ON, n 其中 n=0..255 用于 ANALOG_OUT 功能                                   调整
#define FEATURE7_VALUE_MEMORY         OFF //    OFF, ON 断电后保留 SWITCH, ANALOG_OUT, DEW_HEATER 状态                          调整
#define FEATURE7_ON_STATE            HIGH //   HIGH, LOW 表示反转逻辑 使 ON 为 0V 而 OFF 为 Vcc 通常为 3.3V                         调整

#define FEATURE8_PURPOSE              OFF //    OFF, SWITCH, MOMENTARY_SWITCH, ANALOG_OUT, DEW_HEATER, INTERVALOMETER         可选
#define FEATURE8_NAME          "FEATURE8" // "FE..", 受控功能的名称                                                              调整
#define FEATURE8_TEMP                 OFF //    OFF, THERMISTOR 或 n 其中 n 为 ds18b20 序列号 用于 DEW_HEATER 温度                   调整
#define FEATURE8_PIN                  OFF //    OFF, AUX 表示辅助引脚 n 表示引脚编号                                             调整
#define FEATURE8_VALUE_DEFAULT        OFF //    OFF, ON, n 其中 n=0..255 用于 ANALOG_OUT 功能                                   调整
#define FEATURE8_VALUE_MEMORY         OFF //    OFF, ON 断电后保留 SWITCH, ANALOG_OUT, DEW_HEATER 状态                          调整
#define FEATURE8_ON_STATE            HIGH //   HIGH, LOW 表示反转逻辑 使 ON 为 0V 而 OFF 为 Vcc 通常为 3.3V                         调整

// ---------------------------------------------------------------------------------------------------------------------------------
#define FileVersionConfig 6
#include "Extended.config.h"
