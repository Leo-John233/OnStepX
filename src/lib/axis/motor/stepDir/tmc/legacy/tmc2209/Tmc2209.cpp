// -----------------------------------------------------------------------------------
// axis step/dir motor driver, TMC2209Stepper

// note: LegacyUART requires UART RX but TX is optional

#include "Tmc2209.h"

#if !defined(DRIVER_TMC_STEPPER) && defined(TMC2209_PRESENT)

#include "../../../../../../gpioEx/GpioEx.h"

// help with pin names
#define rx m3
#define tx m2

#define CurrentToPercent(current) ((current/1000.0)/(0.325/(rSense + 0.02)))*100.0

// constructor
StepDirTmc2209::StepDirTmc2209(uint8_t axisNumber, const StepDirDriverPins *Pins, const StepDirDriverSettings *Settings,
                               int16_t currentHold, int16_t currentRun, int16_t currentSlewing, int8_t intpol)
                               :TmcStepDirDriverNSG(axisNumber, Pins, Settings, currentHold, currentRun, currentSlewing, intpol) {
  strcpy(axisPrefix, " Axis_Tmc2209StepDir legacy, ");
  axisPrefix[5] = '0' + axisNumber;
}

// setup driver
bool StepDirTmc2209::init() {
  if (!TmcStepDirDriver::init()) return false;

  // get TMC UART driver ready
  pinModeEx(Pins->m0, OUTPUT);
  pinModeEx(Pins->m1, OUTPUT);

  driver = new TMC2209Stepper();
  if (driver == NULL) return false; 

  int16_t rxPin = Pins->rx;

  delay(1);
  #if defined(SERIAL_TMC_HARDWARE_UART)
    #if defined(DEDICATED_MODE_PINS)
      // program the device address 0,1,2,3 since M0 and M1 are all unique
      int deviceAddress = SERIAL_TMC_ADDRESS_MAP(axisNumber - 1);
      digitalWriteEx(Pins->m0, bitRead(deviceAddress, 0));
      digitalWriteEx(Pins->m1, bitRead(deviceAddress, 1));
    #else
      // help user hard code the device address 0,1,2,3 by cutting pins
      digitalWriteEx(Pins->m0, HIGH);
      digitalWriteEx(Pins->m1, HIGH);
    #endif

    VF("MSG:"); V(axisPrefix);
    VF("HW UART driver pins rx="); V(SERIAL_TMC_RX); VF(", tx="); V(SERIAL_TMC_TX); VF(", baud="); V(SERIAL_TMC_BAUD); VLF(" bps");
    pinModeEx(SERIAL_TMC_RX, INPUT_PULLUP);
    #if SERIAL_TMC_INVERT == ON
      driver->setup(SERIAL_TMC, SERIAL_TMC_BAUD, SERIAL_TMC_ADDRESS_MAP(axisNumber - 1), SERIAL_TMC_RX, SERIAL_TMC_TX, true);
    #else
      driver->setup(SERIAL_TMC, SERIAL_TMC_BAUD, SERIAL_TMC_ADDRESS_MAP(axisNumber - 1), SERIAL_TMC_RX, SERIAL_TMC_TX);
    #endif
    rxPin = SERIAL_TMC_RX;
  #else
    // pull MS1 and MS2 low for device address 0
    digitalWriteEx(Pins->m0, LOW);
    digitalWriteEx(Pins->m1, LOW);

    #if SERIAL_TMC_RX_DISABLE == true
      rxPin = OFF;
    #endif
    VF("MSG:"); V(axisPrefix);
    VF("SW UART driver pins rx="); V(rxPin); VF(", tx="); V(Pins->tx); VF(", baud="); V(SERIAL_TMC_BAUD); VLF(" bps");
    pinModeEx(rxPin, INPUT_PULLUP);
    driver->setup(SERIAL_TMC_BAUD, SERIAL_TMC_ADDRESS_MAP(axisNumber - 1), rxPin, Pins->tx);
  #endif

  // this driver automatically switches to one-way communications, even if a RX pin is set
  // so the following only returns false if communications are "half working"
  #ifdef MOTOR_DRIVER_DETECT
    if (rxPin != OFF) {
      if (!driver->isSetupAndCommunicating()) {
        DF("ERR:"); D(axisPrefix); DLF("no motor driver device detected!");
        return false;
      } else { VF("MSG:"); V(axisPrefix); VLF("motor driver device detected"); }
    }
  #endif

  driver->useExternalSenseResistors();
  driver->enableAnalogCurrentScaling();

  driver->enable();
  driver->moveUsingStepDirInterface();
  driver->setPwmOffset(pc_pwm_ofs);
  driver->setPwmGradient(pc_pwm_grad);
  if (pc_pwm_auto) driver->enableAutomaticCurrentScaling();
  if (!intpol.value) {
    VF("MSG:"); V(axisPrefix); VLF("driver interpolation control not supported");
  }

  modeMicrostepTracking();
  driver->setRunCurrent(CurrentToPercent(iRun));
  driver->setHoldCurrent(CurrentToPercent(iHold));
  driver->disableStealthChop();

  return true;
}

void StepDirTmc2209::modeMicrostepTracking() {
  driver->setMicrostepsPerStep(normalizedMicrosteps);
}

int StepDirTmc2209::modeMicrostepSlewing() {
  if (microstepRatio > 1) {
    driver->setMicrostepsPerStep(normalizedMicrostepsSlewing);
  }
  return microstepRatio;
}

void StepDirTmc2209::modeDecayTracking() {
  if (normalizedDecay == SPREADCYCLE) driver->disableStealthChop(); else driver->enableStealthChop();
  driver->setRunCurrent(CurrentToPercent(iRun));
  driver->setHoldCurrent(CurrentToPercent(iHold));
}  

void StepDirTmc2209::modeDecaySlewing() {
  if (decaySlewing.value == SPREADCYCLE) driver->disableStealthChop(); else driver->enableStealthChop();
  driver->setRunCurrent(CurrentToPercent(iGoto));
  driver->setHoldCurrent(CurrentToPercent(iHold));
}

void StepDirTmc2209::readStatus() {
  driverStatus = driver->getStatus();
  status.outputA.shortToGround  = (bool)driverStatus.short_to_ground_a || (bool)driverStatus.low_side_short_a;
  status.outputA.openLoad       = (bool)driverStatus.open_load_a;
  status.outputB.shortToGround  = (bool)driverStatus.short_to_ground_b || (bool)driverStatus.low_side_short_b;
  status.outputB.openLoad       = (bool)driverStatus.open_load_b;
  status.overTemperatureWarning = (bool)driverStatus.over_temperature_warning;
  status.overTemperature        = (bool)driverStatus.over_temperature_shutdown;
  status.standstill             = (bool)driverStatus.standstill;
}

#if DEBUG != OFF && defined(DEBUG_AXIS) && DEBUG_AXIS != OFF && defined(DEBUG_UART) && DEBUG_UART == ON
void StepDirTmc2209::debugStatus() {
  uint32_t raw = 0;
  raw |= (uint32_t)driverStatus.over_temperature_warning;
  raw |= (uint32_t)driverStatus.over_temperature_shutdown << 1;
  raw |= (uint32_t)driverStatus.short_to_ground_a << 2;
  raw |= (uint32_t)driverStatus.short_to_ground_b << 3;
  raw |= (uint32_t)driverStatus.low_side_short_a << 4;
  raw |= (uint32_t)driverStatus.low_side_short_b << 5;
  raw |= (uint32_t)driverStatus.open_load_a << 6;
  raw |= (uint32_t)driverStatus.open_load_b << 7;
  raw |= (uint32_t)driverStatus.over_temperature_120c << 8;
  raw |= (uint32_t)driverStatus.over_temperature_143c << 9;
  raw |= (uint32_t)driverStatus.over_temperature_150c << 10;
  raw |= (uint32_t)driverStatus.over_temperature_157c << 11;
  raw |= (uint32_t)driverStatus.current_scaling << 16;
  raw |= (uint32_t)driverStatus.stealth_chop_mode << 30;
  raw |= (uint32_t)driverStatus.standstill << 31;

  const bool communicating = driver->isCommunicating();
  const bool setup = driver->isSetupAndCommunicating();
  const uint8_t ifcnt = driver->getInterfaceTransmissionCounter();
  const uint32_t tstep = driver->getInterstepDuration();
  const uint16_t sgResult = driver->getStallGuardResult();
  const uint16_t mscnt = driver->getMicrostepCounter();
  const uint8_t pwmScaleSum = driver->getPwmScaleSum();
  const int16_t pwmScaleAuto = driver->getPwmScaleAuto();
  const uint8_t pwmOffsetAuto = driver->getPwmOffsetAuto();
  const uint8_t pwmGradientAuto = driver->getPwmGradientAuto();
  const bool transmitOnly = driver->debugTransmitOnly();
  const uint8_t version = driver->debugVersion();
  const bool blocking = driver->debugBlocking();

  const bool changed = !debugSnapshotValid ||
                       communicating != debugLastSnapshot.communicating ||
                       setup != debugLastSnapshot.setup ||
                       transmitOnly != debugLastSnapshot.transmitOnly ||
                       blocking != debugLastSnapshot.blocking ||
                       version != debugLastSnapshot.version ||
                       ifcnt != debugLastSnapshot.ifcnt ||
                       raw != debugLastSnapshot.driverStatus ||
                       tstep != debugLastSnapshot.tstep ||
                       sgResult != debugLastSnapshot.sgResult ||
                       mscnt != debugLastSnapshot.mscnt ||
                       pwmScaleSum != debugLastSnapshot.pwmScaleSum ||
                       pwmScaleAuto != debugLastSnapshot.pwmScaleAuto ||
                       pwmOffsetAuto != debugLastSnapshot.pwmOffsetAuto ||
                       pwmGradientAuto != debugLastSnapshot.pwmGradientAuto;
  if (!changed) return;

  debugLastSnapshot.communicating = communicating;
  debugLastSnapshot.setup = setup;
  debugLastSnapshot.transmitOnly = transmitOnly;
  debugLastSnapshot.blocking = blocking;
  debugLastSnapshot.version = version;
  debugLastSnapshot.ifcnt = ifcnt;
  debugLastSnapshot.driverStatus = raw;
  debugLastSnapshot.tstep = tstep;
  debugLastSnapshot.sgResult = sgResult;
  debugLastSnapshot.mscnt = mscnt;
  debugLastSnapshot.pwmScaleSum = pwmScaleSum;
  debugLastSnapshot.pwmScaleAuto = pwmScaleAuto;
  debugLastSnapshot.pwmOffsetAuto = pwmOffsetAuto;
  debugLastSnapshot.pwmGradientAuto = pwmGradientAuto;
  debugSnapshotValid = true;

  char s[256];
  snprintf(s, sizeof(s),
           "Axis%u TMC2209 UART comm=%u setup=%u tx_only_=%u blocking_=%u IOIN.VERSION=0x%02X IFCNT=%u DRV_STATUS=0x%08lX TSTEP=%lu SG=%u MSCNT=%u",
           (unsigned int)axisNumber, (unsigned int)communicating, (unsigned int)setup,
           (unsigned int)transmitOnly, (unsigned int)blocking, (unsigned int)version,
           (unsigned int)ifcnt, (unsigned long)raw, (unsigned long)tstep,
           (unsigned int)sgResult, (unsigned int)mscnt);
  DL(s);

  snprintf(s, sizeof(s),
           "Axis%u TMC2209 bits otpw=%u ot=%u s2ga=%u s2gb=%u lsa=%u lsb=%u ola=%u olb=%u t120=%u t143=%u t150=%u t157=%u cs=%u stealth=%u stst=%u pwm=%u/%d/%u/%u",
           (unsigned int)axisNumber, (unsigned int)driverStatus.over_temperature_warning,
           (unsigned int)driverStatus.over_temperature_shutdown, (unsigned int)driverStatus.short_to_ground_a,
           (unsigned int)driverStatus.short_to_ground_b, (unsigned int)driverStatus.low_side_short_a,
           (unsigned int)driverStatus.low_side_short_b, (unsigned int)driverStatus.open_load_a,
           (unsigned int)driverStatus.open_load_b, (unsigned int)driverStatus.over_temperature_120c,
           (unsigned int)driverStatus.over_temperature_143c, (unsigned int)driverStatus.over_temperature_150c,
           (unsigned int)driverStatus.over_temperature_157c, (unsigned int)driverStatus.current_scaling,
           (unsigned int)driverStatus.stealth_chop_mode, (unsigned int)driverStatus.standstill,
           (unsigned int)pwmScaleSum, (int)pwmScaleAuto, (unsigned int)pwmOffsetAuto,
           (unsigned int)pwmGradientAuto);
  DL(s);
}
#endif

// secondary way to power down not using the enable pin
bool StepDirTmc2209::enable(bool state) {
  if (state) {
    modeDecayTracking();
  } else {
    driver->enableStealthChop();
    driver->setHoldCurrent(0);
  }

  return true;
}

// calibrate the motor driver if required
void StepDirTmc2209::calibrateDriver() {
  if (decay.value == STEALTHCHOP || decaySlewing.value == STEALTHCHOP) {
    VF("MSG: StepDirDriver"); V(axisNumber); VL(", TMC standstill automatic current calibration");
    driver->setRunCurrent(CurrentToPercent(iRun));
    driver->setHoldCurrent(CurrentToPercent(iRun));
    driver->enableStealthChop();
    delay(1000);
    driver->setRunCurrent(CurrentToPercent(iRun));
    driver->setHoldCurrent(CurrentToPercent(iHold));
    driver->disableStealthChop();
  }
}

#endif
