#include "pages/page-imu.hpp"

void actionIMU()
{
  settings.imu_skip = settings.imu_skip ? false : true;
  storeSettings();
  if (settings.imu_skip)
  {
    mpuDeepSleep();
    msgInfo("IMU (Acceler) is OFF", "Watch using less power");
  }
  else
  {
    initMPU();
    msgInfo("IMU (Acceler) is ON", "Watch using more power");
  }
  sleep(3);
}