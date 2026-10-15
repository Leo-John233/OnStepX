// Flashes an esp8266 based SWS on the SERIAL_B interface by passing data to/from SERIAL_A
#pragma once
#include "../../Common.h"

#if defined(SERIAL_PASSTHROUGH)
  class AddonFlasher {
    public:
      // startup the addon flasher, monitors the ADDON_TRIGR_PIN if present
      void init(bool (*canStart)() = nullptr);

      // 由使用方判断共用引脚是否空闲
      bool canFlash() { return !active && (!canStart || canStart()); }

      // flashes the addon
      bool go(bool timeout = true);

      // set the addon for run mode
      void run(bool setSerial = false);

      // set the addon for flash mode
      void flash();

      // reset the addon
      void reset(bool releaseShared = true);

      // monitors for flash button press
      void poll();
    private:
      // 烧录会话统一接管和恢复共用引脚
      bool beginSession();
      void endSession();
      bool (*canStart)() = nullptr;
      bool active = false;
      int savedBootLevel = HIGH;
  };

  extern AddonFlasher addonFlasher;
#endif
