// 回零恢复策略 不依赖电机 坐标或存储实现
#pragma once

class RecoveryPolicy {
  public:
    void reset() { homeRequired = false; parkRestoreAllowed = false; }
    void requireHome(bool allowParkRestore) {
      homeRequired = true;
      parkRestoreAllowed = allowParkRestore;
    }
    void revokeParkRestore() { parkRestoreAllowed = false; }
    void complete(bool coordinatesTrusted) {
      homeRequired = !coordinatesTrusted;
      parkRestoreAllowed = false;
    }
    bool needsHome() const { return homeRequired; }
    bool canRestorePark() const { return parkRestoreAllowed; }
    bool allowsMotion(bool coordinatesTrusted) const { return coordinatesTrusted && !homeRequired; }
    bool canReturnHome(bool coordinatesTrusted) const { return coordinatesTrusted && homeRequired; }

  private:
    bool homeRequired = false;
    bool parkRestoreAllowed = false;
};
