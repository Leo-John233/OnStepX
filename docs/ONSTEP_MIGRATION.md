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
| 完整对应配置 | caa450d |
| 旧版回零参数与注释兼容 | cb0a5a8 |

## 行为说明

每轴限位单独采样 不移植旧版两轴共用限位的猜轴逻辑
同一轴两端共用输入时 记录该轴触发时方向 只允许反向退出
若上电时已经压住限位且方向未知 禁止自动猜测 需先机械释放开关
限位释放稳定 500 毫秒后才接受新的自动运动
HOME_REQUIRED_AFTER_LIMIT 为 ON 时还需要完成回零或确认机械原点
ST4 赤经双键长按三秒只触发一次 全部松开后重新计次
短于三秒的两秒组合键仍保持原有手控行为
无传感器且位置已经丢失时 需要手动放回原点后使用 hF 确认

## 验证记录

源码模拟检查命令

```text
python tests/migration/run.py --cxx D:/MinGW/bin/g++.exe
```

已通过恢复锁 回零速度 独立限位 GOTO 结果 ST4 按钮 回零偏置两种模式
以及旧版偏置配置 原生偏置配置 零偏置配置和回零完成状态 共 11 组检查
引脚编译检查覆盖 TMC5160 SPI 与 TMC2209 UART
配置对照覆盖同名参数与改名参数 旧有注释保持原文

完整编译使用 ESP32 core 2.0.17 与仓库 libraries
开发板为 ESP32 Dev Module Flash 为 16MB 分区为 16M Flash 3MB APP/9.9MB FATFS

```text
arduino-cli compile --jobs 4 --fqbn esp32:esp32:esp32:FlashSize=16M,PartitionScheme=app3M_fat9M_16MB --libraries libraries .
```

TMC5160 SPI 完整编译通过 程序占用 1303557 字节 RAM 占用 52012 字节
TMC2209 UART 完整编译通过 程序占用 1305633 字节 RAM 占用 51676 字节
UART 使用临时副本 仅修改两轴驱动型号 工作区保持 TMC5160 SPI 配置
编译副本中的 427 个源码文件与工作区一致 全部配置宏值一致
板上原点 限位 电机方向和偏置距离仍需实机验证 本次未上传固件

## 再次审计与入口修复

审计覆盖 OnStep 的 342 个 Git 管理代码文件
以 285e196a 为原始 4.24 基线 去除注释和排版影响后逐项比对
237 个文件内容相同 2 个仅注释或排版变化 15 个有代码差异
新增 88 个文件包含 87 个第三方库代码文件和 Config-2209.h
旧 WiFi 附件的代码差异是误插入的 cite_start 标记 不属于新增功能

修复同时对照 OnStepX 5c986c5 的原始实现

| 入口 | 原有实现与最终处理 | 本地提交 |
| --- | --- | --- |
| 普通 GOTO | 原生 firstGoto 会在运动开始时开启跟踪 迁移后改为每次普通 GOTO 成功才开启 停止或失败不新增跟踪 | efd2b8b |
| 泊车恢复 | 保留原生可信状态和严格启动策略 仅允许开机前置授权已通过且状态为已泊车的记录解除开机锁 | 1b32072 |
| 泊车时回零 | 恢复锁期间有传感器或已知坐标时允许退出泊车 再走原生回零流程 | 1b32072 |
| hC 命令 | 原生入口只返回忙 补齐旧版先停止行为 停稳后再次发送才启动回零 | 407003a |

自动恢复泊车坐标不会解除 HOME_REQUIRED_ON_BOOT
显式 Unpark 成功后可解除开机锁并开始跟踪
传感器回零也可退出泊车并在成功后建立位置基准
故障 限位或回零中断会撤销泊车恢复资格 不能用旧记录绕过恢复锁
没有通过原生启动授权的严格模式不会因泊车记录存在而自动获得授权
Unpark 额外检查运动状态和物理限位 包括使用绝对编码器时

回零超时保留 OnStepX 原生实现 不迁入旧版定时移动算法
快速寻找按每轴 SENSE_HOME_DIST_LIMIT 除以实际速度再乘 1.2 计算超时
两轴默认搜索距离为 180 度 精找阶段使用 SLEW_HOME_REFINE_TIME_LIMIT 默认 120 秒
导星层总回零时间上限为 600 秒 偏置阶段另有 600 秒上限
这些限制与限位检测共同生效 不为照搬旧版 360 度范围而扩大机械搜索距离

回归检查增加重复 GOTO 泊车恢复和运动中 hC 共 13 组
覆盖自动恢复保留锁 显式恢复解除锁 位置失效拒绝恢复
以及复位失败 NV 故障 限位未释放 运动未停和无传感器位置丢失等拒绝路径

本轮 13 组源码回归检查全部通过
TMC5160 SPI 完整编译通过 程序 1303853 字节 RAM 52012 字节
TMC2209 UART 完整编译通过 程序 1305905 字节 RAM 51676 字节
UART 使用隔离副本 两份 Config.h 仅两轴 DRIVER_MODEL 不同
当前工作区仍为 TMC5160 SPI 本轮未上传硬件或推送远端

## 解耦兼容实现

以下说明取代前述启动流程和校准入口的处理方式

| 层次 | 职责 |
| --- | --- |
| RecoveryPolicy | 独立保存回零锁和泊车恢复资格 不调用电机 坐标 存储或任务接口 |
| StartupAuthority | 保留原生坐标可信判断 将运动许可与恢复策略组合 |
| Mount 启动协调 | 选择传感器自动回零或泊车坐标恢复 启动结束后落实开机保持 |
| Goto 校准协调 | 检查位置准备结果 等待回零完成后才建立星点校准状态 |
| Axis 与 Guide | 本轴限位决定脱困方向 手动脱困与脉冲导星分别处理 |

原生电机驱动 原点精找 坐标变换 NV 数据布局和引脚文件结构保持不变
不迁入旧版 TMC2209 共享模式引脚保护或两轴共用限位猜轴逻辑
现有回零速度 偏置 开机回零要求和电机保持配置名称及注释保持不变

启动时若已开启传感器自动回零 则直接使用传感器建立坐标 不先恢复泊车记录
回零入口仍检查驱动故障 物理限位和运动状态
没有原点传感器时 保留原生泊车恢复和坐标可信要求
MOUNT_AUTO_HOME_DEFAULT 默认仍为 OFF 本次没有自动开启该选项
MOTOR_HOLD_ON_BOOT 只在启动阶段落实 不改变之后普通复位和泊车的断电规则
回零失败 NV 故障或电机故障时 不通过启动结束处理重新使能电机

星点校准命令使用 requestAlignment 作为统一入口
未开启 ALIGN_AUTO_HOME 时 必须先完成位置恢复 请求失败直接返回错误
开启 ALIGN_AUTO_HOME 时 命令成功表示已接受请求 回零完成后才进入星点校准并开启跟踪
Mount 的已有监控任务调用 pollAlignment 检查完成结果 Home 不直接修改校准状态
回零失败或停止命令取消等待中的校准 手动 Set Home 也取消该等待

物理限位反向脱困独立于 HOME_REQUIRED_AFTER_LIMIT 的 ON 或 OFF
本轴方向未知或指向被阻挡方向时仍拒绝移动
已确认的反向手动脱困不使用可能失真的软件坐标限位
低速脱困走轴层手动运动 脉冲导星在限位未完全释放时拒绝执行
普通手动运动和正常脉冲导星继续使用原生检查

模块提交为 b8edae0 恢复策略 045ad6d 启动协调 5a5d6a1 校准协调 765cdd2 独立脱困
新增组合检查覆盖保持开关 自动回零有无传感器 故障与 NV 错误
以及自动校准开关 请求失败 等待完成 东西侧低速脱困和脉冲导星拒绝

手动设原点取消待完成校准的边界修复提交为 ace39d7
本轮共 23 组源码检查通过 包含真实恢复策略与运动入口的主机模拟
最终 TMC5160 SPI 完整构建通过 程序 1304613 字节 RAM 52020 字节
最终 TMC2209 UART 完整构建通过 程序 1306625 字节 RAM 51684 字节
434 个编译输入文件已核对 SPI 与工作区一致 UART 仅两轴驱动型号不同
检查使用软件模拟和编译 未替代电机 原点 限位 偏置与电流的实机验证

## 回零前置检查与状态变更分离

回零距离检查提前到状态变更之前 检查通过后才退出泊车 清除失败标记 停止跟踪和使能电机
传感器回零仍通过 StartupAuthority 进入恢复锁 不增加第二套坐标可信状态
超限请求保持原有坐标可信状态和软件限位 连续请求不能绕过原生距离保护
坐标已经丢失时仍允许传感器恢复 已接受请求后发生故障仍保留恢复锁

新增检查覆盖两轴 正反方向 正负及零偏置 连续三次超限请求和回到有效范围后的再次请求
同时检查距离边界 坐标丢失恢复和回零复位失败 共 24 组源码回归检查通过
TMC5160 SPI 完整编译通过 程序 1304601 字节 RAM 52020 字节
TMC2209 UART 完整编译通过 程序 1306613 字节 RAM 51684 字节
配置值及配置注释保持不变 本次未上传硬件或推送远端

## MaxESP4 的 ESP07 烧录接口

当前 SPI 主板按原理图补齐 ADDON_GPIO0_PIN 为 GPIO26 和 ADDON_RESET_PIN 为 GPIO2
GPIO26 与赤纬方向共用 GPIO2 经 RST 跳线连接 ESP07 复位 并与 TMC5160 SPI MISO 共用
用户确认仅烧录时短接 RST 跳线 正常运行时断开 烧录前应停止轴运动
不能照搬 MaxESP3 的 GPIO4 复位定义 当前 SPI 主板的 GPIO4 用于电机使能

复位前临时将共用引脚设为输出 复位后恢复上拉输入 避免 SPI 初始化后无法发出复位脉冲
此处理仅对声明共用复位输入的引脚映射生效 原有独立复位流程不变
UART 分支不套用此 SPI 主板的 ESP07 接线 自定义烧录引脚仍可覆盖默认定义
新增 5 组检查覆盖独立复位 共用复位 SPI 引脚 UART 分支及自定义引脚
5 组检查通过 ESP32 core 2.0.17 编译器直接编译主入口和烧录模块通过
验证使用当前 SERIAL_B_ESP_FLASHING 为 ON 的配置 未修改用户配置
完整固件构建因依赖扫描耗时过长已停止 未完成链接及实板烧录验证
用户报错环境为 ESP32 core 2.0.14-cn 本次未在该版本完整构建

## ESP 烧录透传等待修复

原生透传流程将最后接收时间设为未来时间 无符号时间差会使首次等待立即超时
改为记录当前时间 首次等待 90 秒 收到上传数据后采用 30 秒空闲超时
使用 uint32_t 时间差处理毫秒计数回绕
回归检查覆盖无数据等待 持续上传 空闲退出及计数回绕
此修复需要重新编译并上传 OnStepX 到主控 ESP32 才会对 ESP07 透传烧录生效
