// -----------------------------------------------------------------------------------
// axis step/dir motor driver
#pragma once

#include <Arduino.h>
#include "../../../../../../../Common.h"

#if !defined(DRIVER_TMC_STEPPER) && defined(TMC2130_PRESENT)

#include "../TmcSPI.h"
#include "../../TmcStepDirDriverNSG.h"

class StepDirTmc2130 : public TmcStepDirDriverNSG {
  public:
    // constructor
    StepDirTmc2130(uint8_t axisNumber, const StepDirDriverPins *Pins, const StepDirDriverSettings *Settings, int16_t currentHold, int16_t currentRun, int16_t currentSlewing, int8_t  intpol);

    // setup driver
    bool init();

    // set microstep mode for tracking
    void modeMicrostepTracking();

    // set microstep mode for slewing
    int modeMicrostepSlewing();

    // set decay mode for tracking
    void modeDecayTracking();

    // set decay mode for slewing
    void modeDecaySlewing();

    // secondary way to power down not using the enable pin
    bool enable(bool state);

    // calibrate the motor driver if required
    void calibrateDriver();

  private:
    // read status info. from driver
    void readStatus();

    #if DEBUG != OFF && defined(DEBUG_AXIS) && DEBUG_AXIS != OFF && defined(DEBUG_SPI) && DEBUG_SPI == ON
      // output complete SPI/register status for the selected debug axis
      void debugStatus() override;
      bool debugSnapshotValid = false;
      uint8_t debugLastStatusByte = 0;
      uint32_t debugLastDriverStatus = 0;
      uint32_t debugLastChopconf = 0;
    #endif

    TmcSPI driver;
};

#endif
