#include "sleep.hpp"

void handleSleep(bool showMsg)
{
  //tftSleep(showMsg);
  if (!settings.imu_skip)
    mpuSleep();
  tftSleep(false);
  deactivateWifi();
  rtcSleep();
  pinMode(39, GPIO_MODE_INPUT);
  if (!settings.imu_skip)
    esp_sleep_enable_ext1_wakeup(GPIO_SEL_33 | GPIO_SEL_39, ESP_EXT1_WAKEUP_ANY_HIGH);
  else
    esp_sleep_enable_ext1_wakeup(GPIO_SEL_33, ESP_EXT1_WAKEUP_ANY_HIGH);
  esp_deep_sleep_disable_rom_logging();
  esp_deep_sleep_start();
}
