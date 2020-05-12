#include <Arduino.h>
#include <Wire.h>
#include <rom/rtc.h>
#include "wristband-tft.hpp"
#include "wristband-ota.hpp"
#include "clock.hpp"
#include "pages.hpp"
#include "mpu.hpp"


//todo jh keeping a centralized copy of how some pins are initialised and for tests
void initInputPins()
{ // todo jh what about setting for unused pins?
  pinMode(RTC_INT_PIN, INPUT); // PCF8563 external pull up,  open drain active low so unusable for interrupt, use internal esp32 RTC for wakeup. The PCF8563 is set to sleep anyway with seep sleep
//pinMode(CHARGE_PIN, INPUT_PULLUP); // TP4054, esp32 pulls high as input in setupBattery(). no external pull up or down. pulled low when charging by tp4054, pulled low weakly when charged so esp32 wins and pulls up, undervoltage forces high.
  pinMode(TP_PIN_PIN, INPUT); // TTP223-BA6 goes high when pressed. no external pull up or down, see initButton
  pinMode(IMU_INT_PIN, INPUT); // MPU-9250 has an external 10K pull up and so wastes 330uA, for LSM9DS1 no pull or down resistors for interrupts
  adcAttachPin(BATT_ADC_PIN);
  adcAttachPin(VBUS_ADC_PIN);
}
void initOutputPins(){
  // pinMode(TP_PWR_PIN, PULLUP);  //see initButton() keeps TP powered on when enters deep sleep
  // digitalWrite(TP_PWR_PIN, HIGH);
  // pinMode(LED_PIN, OUTPUT);
}


void setup()
{
  initInputPins();
  initOutputPins();
  Serial.begin(115200);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(400000);
  loadSettings();
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
