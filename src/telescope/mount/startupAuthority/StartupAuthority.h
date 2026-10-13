//--------------------------------------------------------------------------------------------------
// telescope mount startup authority
#pragma once

#include "../../../Common.h"

#ifdef MOUNT_PRESENT

#include "../../../libApp/commands/ProcessCmds.h"

class StartupAuthority {
  public:
    void init();
    void begin();

    inline bool trusted() const { return trustedValue && !homeRequired; }
    // 恢复锁期间仅允许已知坐标返回原点
    inline bool needsHome() const { return homeRequired; }
    inline bool canReturnHome() const { return trustedValue && homeRequired; }
    void requireHome(bool coordinatesKnown = false, bool allowParkRestore = false);
    // 仅开机时已验证的泊车记录可用于恢复
    inline bool canRestorePark() const { return parkRestoreAllowed; }
    void completeHome();
    bool required() const;
    CommandError validateManualMotion() const;
    void setTrusted(bool state);

    inline bool trustEstablished() const { return trustEstablishedValue; }
    inline void setCommissioningAllowed(bool state) { commissioningAllowed = state; }

    inline bool absoluteCoordinateOriginsEstablished() const { return absoluteCoordinateOriginsEstablishedValue; }
    inline void clearAbsoluteCoordinateOriginsEstablished() { absoluteCoordinateOriginsEstablishedValue = false; }

  private:
    bool absoluteCoordinateOriginsEstablishedValue = true;
    bool commissioningAllowed = false;
    bool trustEstablishedValue = false;
    bool trustedValue = false;
    bool homeRequired = false;
    bool parkRestoreAllowed = false;
};

extern StartupAuthority startupAuthority;

#endif
