//--------------------------------------------------------------------------------------------------
// telescope mount startup authority

#include "StartupAuthority.h"

#ifdef MOUNT_PRESENT

#include "../../Telescope.h"
#include "../Mount.h"
#include "../goto/Goto.h"
#include "../limits/Limits.h"
#include "../site/Site.h"

void StartupAuthority::init() {
  absoluteCoordinateOriginsEstablishedValue = true;
}

void StartupAuthority::begin() {
  homeRequired = false;
  parkRestoreAllowed = false;
  trustedValue = false;
  trustEstablishedValue = false;
  commissioningAllowed = false;
}

void StartupAuthority::setTrusted(bool state) {
  // 普通授权不能在恢复锁期间重建已丢失的坐标
  if (state && homeRequired) return;
  #if NV_INIT_ERROR_REVOKES_AUTHORITY == ON
    if (state && initError.nv) {
      DLF("WRN: Mount, startup authority trust rejected due to NV fault");
      return;
    }
  #endif

  trustedValue = state;
  if (!state) parkRestoreAllowed = false;
  if (state) {
    trustEstablishedValue = true;
    commissioningAllowed = false;
  } else limits.enabled(false);

  // If we have a trusted coordinate basis and date/time is ready, limits can
  // be enforced immediately without waiting for a later goto/reset/unpark path.
  if (trusted() && site.isDateTimeReady()) limits.enabled(true);
}

// 停止跟踪并锁定自动运动 手动退出仍由限位检查约束
void StartupAuthority::requireHome(bool coordinatesKnown, bool allowParkRestore) {
  parkRestoreAllowed = allowParkRestore && trusted();
  if (!coordinatesKnown) trustedValue = false;
  homeRequired = true;
  limits.enabled(false);
  mount.tracking(false);
}

// 只有确认原点后才解除恢复锁 NV 故障仍可拒绝授权
void StartupAuthority::completeHome() {
  parkRestoreAllowed = false;
  homeRequired = false;
  trustedValue = false;
  setTrusted(true);
  if (!trustedValue) homeRequired = true;
}

bool StartupAuthority::required() const {
  #if MOUNT_COORDS_MEMORY == ON
    return true;
  #else
    return goTo.absoluteEncodersPresent || axis1.motor->hasAbsoluteEncoder() || axis2.motor->hasAbsoluteEncoder();
  #endif
}

CommandError StartupAuthority::validateManualMotion() const {
  if (homeRequired) return CE_NONE;
  if (!required()) return CE_NONE;

  // Commissioning mode: on first boot/fresh NV there may be no trusted
  // coordinate memory or encoder origin yet.  Allow manual motion so the user
  // can place the mount and establish the one-time origin.
  if (!trustedValue) {
    #if MOUNT_COORDS_MEMORY == ON
      if (commissioningAllowed) return CE_NONE;
    #else
      if (!trustEstablishedValue) return CE_NONE;
    #endif
    return CE_SLEW_ERR_UNSPECIFIED;
  }

  if (!site.isDateTimeReady()) return CE_SLEW_ERR_IN_STANDBY;
  if (!limits.isEnabled()) return CE_SLEW_ERR_OUTSIDE_LIMITS;

  return CE_NONE;
}

StartupAuthority startupAuthority;

#endif
