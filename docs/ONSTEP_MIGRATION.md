# OnStep 功能迁移

来源为 OnStep release-4.24-dev 的 7777f967
目标基线为 OnStepX 的 5c986c5
以当前源码为准 不复用此前已回退的迁移结果

## 硬件

用户确认使用 TMC5160 SPI
接线依据 SCH_Schematic1_2026-10-13.pdf 第2页和第3页

| 信号 | GPIO |
| --- | --- |
| 两轴使能 | 4 |
| SPI MOSI / SCK / MISO | 13 / 14 / 2 |
| 赤经片选 / STEP / DIR | 23 / 18 / 19 |
| 赤纬片选 / STEP / DIR | 5 / 27 / 26 |
| 赤经原点 / 赤纬原点 | 25 / 15 |
| 赤经限位 / 赤纬限位 | 36 / 39 |
| ST4 西 / 东 / 南 / 北 | 34 / 35 / 32 / 33 |
| 状态灯 低电平点亮 | 12 |
| I2C SDA / SCL | 21 / 22 |

SPI 接线按驱动类型选择 UART 分支保留原 MaxESP4 接线
两种分支对应不同硬件接线 不能只改驱动型号就换用同一块实板
刹车由 ESP07 的 GPIO5 控制 不属于主控直接输出
图中 PPS 未连接主控 不启用 PPS
不迁移 TMC2209 共享模式引脚保护
按用户要求迁入 OnStep 当前 Config.h 的电机电流 传动比和原点偏置
原点传感器方向与电平已确认沿用旧安装 赤经 LOW 赤纬 HIGH

## 逐项评估

| 来源功能 | OnStepX 处理方式 |
| --- | --- |
| 三阶段回零和双轴偏置 | 保留轴层传感器精找 使用步数目标完成偏置 补齐完成与中断状态 |
| 快慢速和偏置速度 | 在现有轴层回零机制上提供独立配置 |
| 位置可信与开机回零 | 扩展 StartupAuthority 避免引入重复坐标状态 |
| 开机电机保持 | 与回零确认解耦 不开启跟踪 |
| 限位反向脱困和回零恢复 | 适配两轴独立输入 不沿用旧板共用输入的猜轴逻辑 |
| ST4 长按回零 | 兼容二极管按钮和原有两秒组合键 |
| GOTO 完成与中断 | 区分普通中止和安全中止 仅成功后启动新增跟踪 |
| 已回零状态上报 | 结合位置可信与回零事务状态 |
| 命令调度饥饿 | OnStepX 已独立调度 不迁入旧主循环 |
| 轴2故障输入初始化 | OnStepX 驱动抽象已独立配置 不复制旧修复 |
| 缩短开机延时 | 保留 OnStepX 初始化时序 不盲目替换延时 |
| WiFi 与库文件 | 不迁入旧附件和捆绑库 使用 OnStepX 现有实现 |

## 验证范围

每个模块提交前执行对应检查
编译与模拟检查不能代替带电台架验证
本次不向硬件上传 不推送远端

## 配置对应

保留 OnStepX 配置注释原文 仅更新数值并对齐排版
旧仓库没有 Extended.config.h 同名配置直接迁入 改名项按语义转换

| OnStep 配置 | OnStepX 配置或处理 |
| --- | --- |
| PINMAP MaxESP3 | 使用当前主板 MaxESP4 |
| SERIAL_C_BAUD_DEFAULT ON 与蓝牙名 OnStep | SERIAL_RADIO BLUETOOTH 与 HOST_NAME OnStep |
| LED_STATUS ON | STATUS_MOUNT_LED ON |
| LED_STATUS2 OFF | STATUS_LED OFF |
| LED_RETICLE BUZZER BUZZER_STATE_DEFAULT | RETICLE_LED_DEFAULT STATUS_BUZZER STATUS_BUZZER_DEFAULT 均为 OFF |
| HOME_SENSE ON_PULLUP | 两轴 SENSE_HOME 分别 LOW HIGH 默认 INPUT_PULLUP |
| HOME_OFFSET_AXIS1 -1.5 度 | 原名原值保留 内部换算 -5400 角秒 |
| HOME_OFFSET_AXIS2 +1 度 | 原名原值保留 内部换算 -3600 角秒 |
| LIMIT_SENSE ON_PULLUP 与 LOW | 两轴 MIN/MAX LOW 使用 GPIO36/39 的外部上拉 |
| PPS_SENSE ON | 新板 PPS 未接主控 保持关闭 |
| PEC_SENSE OFF | 保持关闭 |
| AXIS1_STEPS_PER_WORMROT 51200 | PEC_STEPS_PER_WORM_ROTATION 51200 |
| TMC5160_QUIET | TMC5160 配合 STEALTHCHOP 跟踪和 SPREADCYCLE GOTO |
| DRIVER_STATUS TMC_SPI | DRIVER_STATUS ON |
| DRIVER_REVERSE 与 DRIVER_POWER_DOWN | REVERSE 与 POWER_DOWN |
| TRACK_REFRACTION_RATE_DEFAULT OFF | TRACK_COMPENSATION_DEFAULT OFF |
| MFLIP_SKIP_HOME ON | MFLIP_HOME_DEFAULT OFF |
| MFLIP_PAUSE_HOME_MEMORY OFF | MFLIP_HOME_MEMORY OFF |
| AXIS3/4/5_SLEW_RATE_DESIRED | 对应 SLEW_RATE_BASE_DESIRED |
| AXIS4/5_LIMIT_MIN_RATE 50 | 对应 SLEW_RATE_MINIMUM 50 |
| TELESCOPE_TEMPERATURE OFF | FOCUSER_TEMPERATURE OFF |
| FEATUREn_DEFAULT_VALUE | FEATUREn_VALUE_DEFAULT |
| ROTATOR FOCUSER1 FOCUSER2 OFF | 轴3/4/5 的 DRIVER_MODEL 保持 OFF |
| AXIS4/5_DRIVER_DC_MODE OFF | 保留步进驱动模式 |
| AXIS2_TANGENT_ARM OFF | 保留 GEM 普通赤纬轴 |
| STRICT_PARKING OFF | 不新增旧泊车宏 沿用 OnStepX 原有停放与位置可信策略 |
| FEATURE_LIST_DS OFF | 不启用传感器扫描 |
| FileVersionConfig 4 | 保留 OnStepX 所需版本 6 |

两轴每度步数为 14222.222 跟踪与 GOTO 细分分别为 64 和 32
赤经运行与 GOTO 电流为 1400 和 1500 mA 赤纬为 1200 和 1300 mA
赤纬反向 ON GOTO 基准速度 6 度每秒 速度记忆 ON
保留旧配置的 DS3231 PEC 蓝牙 手控模式与中天翻转选择

旧偏置表示从传感器出发的移动方向 新偏置表示传感器相对原点的位置
旧赤经正值向东 负值向西 与 OnStepX 赤经传感器偏置的减法相符
旧赤纬正值向北 使用绝对轴方向 所以迁入传感器偏置时反号
迁入后北半球实际偏置移动为赤经 +1.5 度 赤纬 +1 度
南半球沿用 OnStepX 的纬度反向规则 未进行实机验证
已有 NV 中的运行时轴参数和原点设置仍可能覆盖编译默认值 本次未清除设备 NV

## 模块提交

| 模块 | 本地提交 |
| --- | --- |
| 驱动切换与图纸 IO | fc0ac81 ed5227e |
| 回零恢复锁与可信状态 | 38b0e60 |
| 开机电机保持 | 47fd910 |
| 传感器回零与偏置完成 | e97d436 |
| 快慢速与偏置速度 | c78fdcf |
| 独立限位恢复 | 5ec5e89 |
| GOTO 完成与安全中断 | df66a57 |
| 纯 SPI 串口依赖修正 | deec5f0 |
| ST4 长按回零 | 60c9c90 |
| 跨模块回零边界修正 | f4c534a 2e3b185 |

## 行为说明

每轴限位单独采样 不移植旧版两轴共用限位的猜轴逻辑
同一轴两端共用输入时 记录该轴触发时方向 只允许反向退出
若上电时已经压住限位且方向未知 禁止自动猜测 需先机械释放开关
限位释放稳定 500 毫秒后才接受新的自动运动
HOME_REQUIRED_AFTER_LIMIT 为 ON 时还需要完成回零或确认机械原点
ST4 赤经双键长按三秒只触发一次 全部松开后重新计次
短于三秒的两秒组合键仍保持原有手控行为
无传感器且位置已经丢失时 需要手动放回原点后使用 hF 确认
