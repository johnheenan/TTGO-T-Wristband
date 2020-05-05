#include <Arduino.h>
#include <Wire.h>
#include <rom/rtc.h>
#include "wristband-tft.hpp"
#include "wristband-ota.hpp"
#include "clock.hpp"
#include "pages.hpp"
#include "mpu.hpp"

void setup()
{
  loadSettings();
  Serial.begin(115200);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(400000);
  initClock();
  tftInit();
  deactivateWifi();
  btStop();
  setupADC();
  if (!settings.imu_skip)
    initMPU();
  else
    mpuDeepSleep();
  initButton();
  setupBattery();
}

void loop()
{
  handleUi();
  updateBatteryChargeStatus();
}
