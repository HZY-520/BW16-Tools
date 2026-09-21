#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

//#define DEBUG  // 调试模式开关

// ⚠ 商家量产板硬件限制（2026-09-03 实测实锤）：
// PA_7（D0/BACK 键）是芯片日志串口 LOG_TX 的复用脚。
// Serial.begin() 内部 serial_init() 会调用 pinmap_pinout() 把 PA_7
// 从 GPIO 切成 UART 功能，之后 digitalRead(PA_7) 永远读 0，
// BACK 键将永久失效（按键测试程序已实测复现并验证）。
//
// ⚠ 2026-09-03 二次实锤（WebUI 卡死根因）：
// Serial.begin() 绝对禁止，且 **Serial.print/println 同样致命**——
// 本项目从不调用 Serial.begin()，LOGUARTClass 的 serial_t log_uart_obj
// 停留在 BSS 零值（uart_idx=0），write→serial_putc→serial_writable()
// 会去解引用 uart_adapter[0].UARTx == NULL（全局数组也未初始化），
// 表现为空指针访问或永久忙等挂死。bootloader 只初始化了 UART 硬件
// 寄存器，不会填 Arduino 层的 uart_adapter 数组。
// （旧注释"print 无害"的判断错误，已在 WebUI 启动卡死中实测复现：
//   DNSServer.begin() 里的 Serial.println 直接挂死，AP 起不来。）
// 因此策略：一切 Arduino 层 Serial 输出一律走下面的空操作宏。

// 设置调试串口波特率（保留宏定义以兼容旧代码；不再调用 begin）
#define DEBUG_BAUD 115200

#ifdef DEBUG
  // 调试输出同样禁止（串口未初始化，输出必挂死）；如需调试走 OLED
  #define DEBUG_SER_PRINT(...)
  #define DEBUG_SER_INIT()
#else
  // 如果没有定义DEBUG，这些宏将不执行任何操作
  #define DEBUG_SER_PRINT(...)
  #define DEBUG_SER_INIT()
#endif

#endif
