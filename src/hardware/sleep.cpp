#include "sleep.hpp"
#include "battery.hpp"

void handleSleep(bool showMsg)
{ // todo jh store extra state information with storeSettings() here such as page was on
  //tftSleep(showMsg);
  if (!settings.imu_skip)
    mpuSleep();
#ifdef false
  if (!isPluggedIn())
    tftSleep(false);
  else
  { //todo jh is plugged in
    //does not work tft stays on, an alternative is a non deep sleep low power mode
    ; // if is plugged in then add in code to wake up to update time and show clock before going into deep sleep again
  }
#else
  tftSleep(false);
#endif
  deactivateWifi();
  rtcSleep();
  uint64_t gpio_sel = TP_PIN_SEL;
  if (!settings.imu_skip)
    gpio_sel |= IMU_INT_SEL;
  // if (isPluggedIn() && isCharging())   // todo jh also put into a low power state that is not deep sleep
  //   gpio_sel |= CHARGE_SEL;                                         //wakeup when charged to switch off charging LED
  esp_sleep_enable_ext1_wakeup(gpio_sel, ESP_EXT1_WAKEUP_ANY_HIGH); // any high pin wakes up board
  esp_deep_sleep_disable_rom_logging();
  esp_deep_sleep_start();
}
