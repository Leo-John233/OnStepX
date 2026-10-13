// OnStepX 扩展配置文件 用于很少使用的选项
// 修改下方配置后编译上传 请留意编译器警告和错误
// 标记 必填 / 常用 / 可选 / 调整 / 少改
// 默认值与当前值不同时单独注明
// 设置说明 http://www.stellarjourney.com/index.php?r=site/equipment_onstep
// 社区 https://groups.io/g/onstep

// ============================================================================
// 控制器
// ============================================================================

// --- 调试 ---
// 在指定的 SERIAL_DEBUG 端口输出额外的调试信息和状态信息 使用完毕后请务必禁用
// 注意 SERIAL_DEBUG 端口通常与 SERIAL_A 使用同一串口 因此不能用于正常的 LX200 协议通信
//      如果属于这种情况 请将两者设置为下方指定的相同且较高的波特率
#define DEBUG                             VERBOSE       // [少改] 默认 OFF | ON 仅输出后台错误信息 VERBOSE 输出全部错误和状态信息
                                                        //        CONSOLE 启用 VT100 调试控制台
                                                        //        PROFILER 启用 VT100 任务分析器
#define DEBUG_AXIS                        OFF           // [可选] 默认 OFF | 其中 n=1 至 9 且该轴必须已在 Config.h 中启用
#define DEBUG_UART                        OFF           // [可选] 默认 OFF | ON 每秒回读 TMC UART 通信模式 阻塞状态 IOIN 版本和驱动寄存器
                                                        //        使用 DEBUG_AXIS 选择轴 首次输出一次 后续仅在状态变化时输出 使用完毕后请关闭
#define DEBUG_SPI                         OFF           // [可选] ON 每秒回读 DEBUG_AXIS 对应 TMC SPI 轴的状态字节 DRV_STATUS 和 CHOPCONF
                                                        //        首次回读输出一次 后续仅在状态变化时输出 使用完毕后请关闭
#define DEBUG_CAN                         OFF           // [可选] ON 记录 CAN 接口统计信息
#define DEBUG_ECHO_COMMANDS               OFF           // [可选] ON 或 ERRORS_ONLY 将命令记录到调试串口
#define SERIAL_DEBUG                      Serial        // [可选] 使用任意可用的硬件串口 例如 Serial1 或 Serial2
#define SERIAL_DEBUG_BAUD                 9600          // [可选] 默认 230400 | 其中 n=9600,19200,57600,115200,230400,460800 为常用波特率

// --- 非易失性存储器 ---
// 警告 启用 NV_WIPE 后上传并等待一到两分钟 再改回 OFF 并重新上传
// 持续启用可能导致 NV 过度磨损并损坏微控制器的 NV 子系统
#define NV_WIPE                           OFF           // [少改] 将默认值重新写入 NV FLASH EEPROM 等

// ----------------------------------------------------------------------------
