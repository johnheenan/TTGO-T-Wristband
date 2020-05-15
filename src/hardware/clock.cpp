// Edits and additions by John Heenan 2020 are as is and with no warranty. Please retain line

#include "clock.hpp"
#include "timezones.hpp"

/*
doco
doco the DST, STD convention of DST earlier than STD is retained for the southern hemisphere because the maths works
doco so please do not call this a bug. If there is a bug it is that the names do not mean what they imply.
doco it is a pointless exercise to change variable names when the intention is clear for those in the northern hemisphere
doco and users are not exposed to the issue
doco ntp.isDST() ntp2.isDST(utc) answer the boolean queston is it DST in the Northern Hemisphere
doco ntp2.isDstSouth() and ntp2.isDstNorth() give a quad state type answer instead of a boolean answer, see enum isDstNS_t
doco the answer is correct even if a Southern Hemisphere DST is in use 
doco to find if it is summertime in the Southern Hemisphere call function ntp2.isDSTSouth()
doco
doco there is currently no automatic test of rollover at DST changeover times. You can force a changeover
doco by updating time with NTP (orange local time long press)
doco or by changing to another DST Region and changing back again (green UTC time long press).
doco
*/

PCF8563_Class rtc;
WiFiUDP wifiUdp;
NTP ntp(wifiUdp);
NTP2 ntp2(ntp);

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

RTC_Date getUTCTime(time_t *utc)
{
  RTC_Date now = rtc.getDateTime();
  tm timeStructure;
  time_t utcNowIfDST;
  time_t utcNowIfSTD;
  bool summerIfDST;
  bool summerIfSTD;
  time_t utcNow;
  tm *utcStructure;
  dst_t *dst = &dst_array[dst_index];

  timeStructure.tm_sec = now.second;
  timeStructure.tm_mday = now.day;
  timeStructure.tm_mon = now.month - 1;
  timeStructure.tm_year = now.year - 1900;
  timeStructure.tm_isdst = 0;
  int min;

  if (!settings.tz_uses_dst)
  {
    timeStructure.tm_hour = now.hour - settings.tz_offset / 60;
    min = settings.tz_offset % 60;
    // if (min<0)
    //   min=-min;
    timeStructure.tm_min = now.minute - min;
    utcNow = mktime(&timeStructure);
    utcStructure = localtime(&utcNow);
  }
  else
  {
    timeStructure.tm_hour = now.hour - dst->dst_offset / 60;
    min = dst->dst_offset % 60;
    // if (min<0)
    //   min=-min;
    timeStructure.tm_min = now.minute - min;
    utcNowIfDST = mktime(&timeStructure);
    summerIfDST = ntp2.isDST(utcNowIfDST);

    timeStructure.tm_hour = now.hour - dst->std_offset / 60;
    min = dst->std_offset % 60;
    // if (min<0)
    //   min=-min;
    timeStructure.tm_min = now.minute - min;
    utcNowIfSTD = mktime(&timeStructure);
    summerIfSTD = ntp2.isDST(utcNowIfSTD);

    if (summerIfDST && summerIfSTD)
      utcNow = utcNowIfDST;
    else if (!summerIfDST && !summerIfSTD)
      utcNow = utcNowIfSTD; // todo jh review and test this
    else if (summerIfDST && !summerIfSTD)
      utcNow = utcNowIfDST; // todo jh review and test this
    else                    // !summerIfDST && summerIfSTD
      utcNow = utcNowIfSTD;
    utcStructure = localtime(&utcNow);
  }
  if (utc != nullptr)
    *utc = utcNow;
  return RTC_Date(utcStructure->tm_year + 1900, utcStructure->tm_mon + 1, utcStructure->tm_mday, utcStructure->tm_hour, utcStructure->tm_min, utcStructure->tm_sec);
}

char buf1[10];
const char *getTZCode()
{
  if (settings.tz_uses_dst)
  {
    if (ntp.isDST())
      return dst_array[dst_index].dst_code;
    else
      return dst_array[dst_index].std_code;
  }
  else
  {
    int hr = settings.tz_offset / 60;
    int min = settings.tz_offset % 60;
    char sign = ' ';
    if (settings.tz_offset > 0)
      sign = '+';
    else if (settings.tz_offset < 0)
      sign = '-';
    if(hr<0)
      hr=-hr;
    if (min < 0)
      min = -min;
    snprintf(buf1, sizeof(buf1), "%c%02d:%02d", sign, hr, min);
    return buf1;
  }
}

void setTime(RTC_Date datetime)
{
  rtc.setDateTime(datetime);
}

bool isNothernHemispere() // doco if offsets are equal (which you can test for) then this function fails
{                         // todo jh this function will not work unless there is a real diiference in offsets. For example will not work for Brisbane which has no DST but is included as if it does
  return dst_array[dst_index].dst_offset > dst_array[dst_index].std_offset;
}

void setNtpUtcDst(time_t utcNow)
{
  ntp2.setUtcDst(utcNow);
}

void initNTP()
{
  if (settings.tz_uses_dst)
  {
    ntp.isDST(true);
    dst_t *dst = &dst_array[dst_index];
    ntp.ruleDST(dst->dst_code, dst->week, dst->wday, dst->dst_month, dst->dst_hour, dst->dst_offset);
    ntp.ruleSTD(dst->std_code, dst->week, dst->wday, dst->std_month, dst->std_hour, dst->std_offset);
  }
  else
  {
    ntp.isDST(false);
    ntp.timeZone(settings.tz_offset / 60, settings.tz_offset % 60);
  }
}

RTC_Date syncTime()
{
  RTC_Date datetime = RTC_Date();
  initNTP();
  if (ntp.begin(800, true))
    datetime = RTC_Date(ntp.year(), ntp.month(), ntp.day(), ntp.hours(), ntp.minutes(), ntp.seconds());
  ntp.stop();
  return datetime;
}

bool NTP2::isDST(time_t utc) //answers is it DST in the northern hemisphere, see next two functions for an alternative
{
  if ((utc > ntp.utcDST) && (utc <= ntp.utcSTD))
    return true;
  else
    return false;
}

isDstNS_t NTP2::isDSTSouth()
{
  if (dst_array[dst_index].dst_offset == dst_array[dst_index].std_offset)
    return DST_NA;
  bool dst = ntp.isDST();
  if (isNothernHemispere())
    return DST_OPP;
  if (dst)
    return DST_FALSE;
  else
    return DST_TRUE;
}

isDstNS_t NTP2::isDSTNorth()
{
  if (dst_array[dst_index].dst_offset == dst_array[dst_index].std_offset)
    return DST_NA;
  bool dst = ntp.isDST();
  if (!isNothernHemispere())
    return DST_OPP;
  if (dst)
    return DST_TRUE;
  else
    return DST_FALSE;
}

void NTP2::setUtcDst(time_t utcNow)
{
  ntp.utcTime = utcNow;
  ntp.lastUpdate = millis();
  ntp.diffTime = 0;
  initNTP();
  if (settings.tz_uses_dst)
  {
    ntp.timezoneOffset = ntp.dstEnd.tzOffset * SECS_PER_MINUTES;
    ntp.dstOffset = (ntp.dstStart.tzOffset - ntp.dstEnd.tzOffset) * SECS_PER_MINUTES;
    ntp.currentTime(); // in case year has changed
    ntp.beginDST();
  }
  setTime(RTC_Date(ntp.year(), ntp.month(), ntp.day(), ntp.hours(), ntp.minutes(), ntp.seconds()));
}