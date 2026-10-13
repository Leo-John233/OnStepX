// 验证驱动型号选择和图纸引脚
#define ESP32
#include "../../src/Constants.h"
#include "../../src/lib/Constants.h"
#ifndef TEST_UART
  #define AXIS1_DRIVER_MODEL TMC5160
  #define AXIS2_DRIVER_MODEL TMC5160
#else
  #define AXIS1_DRIVER_MODEL TMC2209
  #define AXIS2_DRIVER_MODEL TMC2209
#endif
#include "../../src/Config.defaults.h"
#include "../../src/pinmaps/Pins.MaxESP4.h"

#ifndef TEST_UART
static_assert(AXIS1_M0_PIN == 13 && AXIS2_M0_PIN == 13, "MOSI");
static_assert(AXIS1_M1_PIN == 14 && AXIS2_M1_PIN == 14, "SCK");
static_assert(AXIS1_M3_PIN == 2 && AXIS2_M3_PIN == 2, "MISO");
static_assert(AXIS1_M2_PIN == 23 && AXIS2_M2_PIN == 5, "CS");
static_assert(SHARED_ENABLE_PIN == 4, "Enable");
static_assert(AXIS1_STEP_PIN == 18 && AXIS1_DIR_PIN == 19, "RA");
static_assert(AXIS2_STEP_PIN == 27 && AXIS2_DIR_PIN == 26, "DEC");
static_assert(AXIS1_SENSE_HOME_PIN == 25 && AXIS2_SENSE_HOME_PIN == 15, "Home");
static_assert(AXIS1_SENSE_LIMIT_MIN_PIN == 36 && AXIS2_SENSE_LIMIT_MIN_PIN == 39, "Limits");
static_assert(STATUS_LED_ON_STATE == LOW, "LED");
#else
static_assert(SERIAL_TMC_TX == 4 && SERIAL_TMC_RX == 39, "Original UART wiring");
static_assert(AXIS1_M0_PIN == OFF && AXIS2_M0_PIN == OFF, "UART address pins");
static_assert(SERIAL_TMC_ADDRESS_MAP(0) == 0 && SERIAL_TMC_ADDRESS_MAP(1) == 1, "UART addresses");
#endif

int main() { return 0; }
