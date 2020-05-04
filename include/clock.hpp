// Additions by John Heenan 2020

#include <Arduino.h>
#include <Wire.h>
#include <pcf8563.h>
#include <NTP.h> // for enums
#include "hal.hpp"
#include "translations.hpp"

extern const char *tz_dst_name;
extern month_t tz_dst_month;
extern int8_t tz_dst_hour;
extern int tz_dst_offset;

extern week_t tz_week;
extern dow_t tz_wday;
extern bool tz_uses_dst;
extern int tz_offset;

extern const char *tz_std_name;
extern month_t tz_std_month;
extern int8_t tz_std_hour;
extern int tz_std_offset;

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

