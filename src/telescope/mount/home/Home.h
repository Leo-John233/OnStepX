//--------------------------------------------------------------------------------------------------
// telescope mount home control
#pragma once

#include "../../../Common.h"

#ifdef MOUNT_PRESENT

#include "../../../libApp/commands/ProcessCmds.h"
#include "../coordinates/Transform.h"

// 兼容旧配置的度数与方向 内部仍使用角秒保存
#ifdef HOME_OFFSET_AXIS1
  static_assert(HOME_OFFSET_AXIS1 >= -180.0 && HOME_OFFSET_AXIS1 <= 180.0, "HOME_OFFSET_AXIS1 must be within +/-180 degrees");
  constexpr long homeOffsetAxis1 = (long)(HOME_OFFSET_AXIS1*3600.0);
#else
  constexpr long homeOffsetAxis1 = AXIS1_SENSE_HOME_OFFSET;
#endif
#ifdef HOME_OFFSET_AXIS2
  static_assert(HOME_OFFSET_AXIS2 >= -180.0 && HOME_OFFSET_AXIS2 <= 180.0, "HOME_OFFSET_AXIS2 must be within +/-180 degrees");
  constexpr long homeOffsetAxis2 = (long)(-HOME_OFFSET_AXIS2*3600.0);
#else
  constexpr long homeOffsetAxis2 = AXIS2_SENSE_HOME_OFFSET;
#endif
#if AXIS1_SECTOR_GEAR == ON
  static_assert(homeOffsetAxis1 == 0, "Sector gear homing requires zero offset");
#endif
#if AXIS2_TANGENT_ARM == ON
  static_assert(homeOffsetAxis2 == 0, "Tangent arm homing requires zero offset");
#endif

enum HomeState: uint8_t {HS_NONE, HS_HOMING};

#pragma pack(1)
#define SettingsSize 11
typedef struct HomeSense {
  long senseOffset;
  bool senseReverse;
} HomeSense;
typedef struct Settings {
  bool automaticAtBoot;
  HomeSense axis1;
  HomeSense axis2;
} Settings;
#pragma pack()

class Home {
  public:
    // init the home position (according to settings and mount type)
    void init();

    bool command(char *reply, char *command, char *parameter, bool *suppressFrame, bool *numericReply, CommandError *commandError);

    // move mount to the home position
    CommandError request();

    // reset mount, moves to the home position first if home switches are present
    CommandError requestWithReset();

    // clear home state on abort
    void requestAborted();
    // 中断所有回零阶段 停稳后仍保留恢复锁
    void abort();
    void poll();

    // after finding home switches displace the mount axes as specified
    void guideDone(bool success);

    // once homed mark as done
    void requestDone();

    // reset mount at home
    CommandError reset(bool fullReset = true, bool authoritative = false);

    // get the home position (Mount coordinate system)
    Coordinate getPosition(CoordReturn coordReturn = CR_MOUNT_EQU);

    // home sensing
    bool useOffset();

    // allow axis1 by latitude reversal and home switch reversal
    void setReversal();

    bool hasSense = (((AXIS1_SENSE_HOME) != OFF) && ((AXIS2_SENSE_HOME) != OFF)) || \
                    (((AXIS1_SECTOR_GEAR) == ON) && ((AXIS1_SENSE_HOME) != OFF)) || \
                    (((AXIS2_TANGENT_ARM) == ON) && ((AXIS2_SENSE_HOME) != OFF));

    Settings settings = {MOUNT_AUTO_HOME_DEFAULT == ON, {homeOffsetAxis1, false}, {homeOffsetAxis2, false}};

    bool isRequestWithReset = false;
    bool failed = false;

    HomeState state = HS_NONE;

  private:
    bool offsetActive = false;
    bool abortPending = false;
    unsigned long offsetDeadline = 0;
    bool wasTracking = false;
    Coordinate position;

    uint32_t nvKey;
};

extern Home home;

#endif
