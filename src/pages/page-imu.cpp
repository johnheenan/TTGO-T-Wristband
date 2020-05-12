#include "pages/page-imu.hpp"

void actionIMU()
{
  settings.imu_skip = settings.imu_skip ? false : true;
  storeSettings();
  if (settings.imu_skip)
  {
    mpuDeepSleep();
    msgInfo("IMU (Acceler) is OFF", "Watch using less power", FIRMWARE_VER);
  }
  else
  {
    initMPU();
    msgInfo("IMU (Acceler) is ON", "Watch using more power", FIRMWARE_VER);
  }
  sleep(3);
}