// Additions by John Heenan 2020

#include <Arduino.h>
#include <Wire.h>
#include <pcf8563.h>
#include <NTP.h> // for enums
#include "hal.hpp"
#include "translations.hpp"

extern time_t utcDST;

void initClock();
void rtcSleep();
RTC_Date getClockTime();
RTC_Date getUTCTime();
void setTime(RTC_Date datetime);
bool isUsingDST();
bool isDST(time_t utcNow);
bool isNothernHemispere();
void beginDST(int year);

