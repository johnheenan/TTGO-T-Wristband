#include <Arduino.h>
#include <EasyButton.h>
#include "wristband-tft.hpp"
#include "pages/page-ota.hpp"
#include "pages/page-clock.hpp"
#include "pages/page-battery.hpp"
#include "pages/page-bearing.hpp"
#include "pages/page-temperature.hpp"
#include "pages/page-imu.hpp"
#include "eeprom.h"

void handleUi();
void increasePage();
void showPage();
void handleSleep(bool showMsg = true);
void initButton();
void handleAction();