// Additions by John Heenan 2020 are as is and with no warranty.

#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <pcf8563.h>
#include <NTP.h>
#include "hal.hpp"
#include "translations.hpp"
#include "eeprom.hpp" // for settings_t
#include <WiFiUdp.h>

void initClock();
void rtcSleep();
RTC_Date getClockTime();
RTC_Date getUTCTime(time_t *utc=nullptr);
const char* getTZCode();
void setTime(RTC_Date datetime);
void 
setNtpUtcDst(time_t utcNow);
bool isNothernHemispere();

void initNTP();
RTC_Date syncTime();

enum isDstNS_t {DST_FALSE, DST_TRUE, DST_NA, DST_OPP};  // for north or south DST. NA for not applicable as no DST offset difference, OPP for opposite in case asked for south or north, is not NA and is in other hemisphere 

class NTP2
{ // Any class named NTP2 has been declared a friend of the NTP class to get access to private declarations of NTP.
public:    // this minimises changes to the NTP class and also avoids unnecessary duplication here from the NTP class
  NTP2(NTP &ntp) : ntp(ntp) {}
  NTP &ntp;
  bool isDST(time_t utc);
  isDstNS_t isDSTSouth();
  isDstNS_t isDSTNorth();
  void setUtcDst(time_t utcNow);
};
