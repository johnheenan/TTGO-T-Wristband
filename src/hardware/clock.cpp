// Edits and additions by John Heenan 2020

#include "clock.hpp"
#include "timezone.hpp"

PCF8563_Class rtc;

uint8_t current_dayweek = 8;
uint8_t current_minute = 0;

bool tz_uses_dst = TZ_USES_DST;
int tz_offset = TZ_OFFSET;
week_t tz_week = TZ_WEEK;
dow_t tz_wday = TZ_WDAY;

const char *tz_dst_name = TZ_DST_NAME;
month_t tz_dst_month = TZ_DST_MONTH;
int8_t tz_dst_hour = TZ_DST_HOUR;
int tz_dst_offset = TZ_DST_OFFSET;

const char *tz_std_name = TZ_STD_NAME;
month_t tz_std_month = TZ_STD_MONTH;
int8_t tz_std_hour = TZ_STD_HOUR;
int tz_std_offset = TZ_STD_OFFSET;

void initClock()
{
  rtc.begin(Wire);
  rtc.check();
}

void rtcSleep()
{
  rtc.clearTimer();
  rtc.resetAlarm();
  rtc.disableAlarm();
  rtc.disableCLK();
  rtc.disableTimer();
}

RTC_Date getClockTime()
{
  RTC_Date now = rtc.getDateTime();
  return now;
}

RTC_Date getUTCTime()
{
  RTC_Date now = rtc.getDateTime();
  tm timeStructure;
  time_t utcNowIfDST;
  time_t utcNowIfSTD;
  bool summerIfDST;
  bool summerIfSTD;
  time_t utcNow;
  tm *utcStructure;

  timeStructure.tm_sec = now.second;
  timeStructure.tm_mday = now.day;
  timeStructure.tm_mon = now.month - 1;
  timeStructure.tm_year = now.year - 1900;
  timeStructure.tm_isdst = 0;

  if (!isUsingDST())
  {
    timeStructure.tm_hour = now.hour - tz_offset / 60;
    timeStructure.tm_min = now.minute - tz_offset % 60;
    utcNow = mktime(&timeStructure);
    utcStructure = localtime(&utcNow);
  }
  else
  {
    if (!utcDST) //todo jh also call beginDST if year changes
      beginDST(now.year);

    timeStructure.tm_hour = now.hour - tz_dst_offset / 60;
    timeStructure.tm_min = now.minute - tz_dst_offset % 60;
    utcNowIfDST = mktime(&timeStructure);
    summerIfDST = isDST(utcNowIfDST);

    timeStructure.tm_hour = now.hour - tz_std_offset / 60;
    timeStructure.tm_min = now.minute - tz_std_offset % 60;
    utcNowIfSTD = mktime(&timeStructure);
    summerIfSTD = isDST(utcNowIfSTD);

    // DEBUG_MSG("utcNowIfDST, summerIfDST, utcNowIfSTD, summerIfSTD: %ld, %s, %ld, %s\n",
    //    utcNowIfDST, summerIfDST?"true":"false", utcNowIfSTD, summerIfSTD?"true":"false");

    if (summerIfDST && summerIfSTD)
      utcStructure = localtime(&utcNowIfDST);
    else if (!summerIfDST && !summerIfSTD)
      utcStructure = localtime(&utcNowIfSTD);  // todo jh review and test this 
    else if (summerIfDST && !summerIfSTD)
      utcStructure = localtime(&utcNowIfDST);  // todo jh review and test this 
    else // !summerIfDST && summerIfSTD
      utcStructure = localtime(&utcNowIfSTD);
  }
  return RTC_Date(utcStructure->tm_year + 1900, utcStructure->tm_mon + 1, utcStructure->tm_mday, utcStructure->tm_hour, utcStructure->tm_min, utcStructure->tm_sec);
}

void setTime(RTC_Date datetime)
{
  rtc.setDateTime(datetime);
}

bool isUsingDST()
{
  return tz_uses_dst;
}

bool isNothernHemispere()
{
  return tz_dst_month < tz_std_month;
}

// Next three functions contain code adapted by John Heenan from Arduino NTP code
/**
 * NTP library for Arduino framewoek
 * The MIT License (MIT)
 * (c) 2018 sstaub
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

time_t utcSTD, utcDST;
time_t calcDateDST(int year, int8_t week, int8_t wday, int8_t month, int8_t hour)
{
  if (week == 0)
  {
    if (month++ > 11)
    {
      month = 0;
      year++;
    }
    week = 1;
  }

  struct tm tm;
  tm.tm_hour = hour;
  tm.tm_min = 0;
  tm.tm_sec = 0;
  tm.tm_mday = 1;
  tm.tm_mon = month;
  tm.tm_year = year - 1900;
  time_t t = mktime(&tm);

  t += ((wday - tm.tm_wday + 7) % 7 + (week - 1) * 7) * SECS_PER_DAY;
  if (week == 0)
    t -= 7 * SECS_PER_DAY;
  return t;
}

void beginDST(int year)
{
  time_t dstTime = calcDateDST(year, tz_week, tz_wday, tz_dst_month, tz_dst_hour);
  utcDST = dstTime - (tz_dst_offset * SECS_PER_MINUTES);
  time_t stdTime = calcDateDST(year, tz_week, tz_wday, tz_std_month, tz_std_hour);
  utcSTD = stdTime - (tz_std_offset * SECS_PER_MINUTES);
}

bool isDST(time_t utcNow)
{
  if (!isUsingDST())
    return false;
  // DEBUG_MSG("isDST(): utcSTD, utcNow, utcDST: %ld, %ld, %ld\n", utcSTD, utcNow, utcDST);
  if (isNothernHemispere())
  {
    // DEBUG_MSG("is northern hemisphere\n");
    if ((utcNow > utcDST) && (utcNow <= utcSTD))
      return true;
    else
      return false;
  }
  else
  {
    // DEBUG_MSG("is southern hemisphere\n");
    if ((utcNow >= utcSTD) && (utcNow < utcDST))
      return false;
    else
      return true;
  }
}
