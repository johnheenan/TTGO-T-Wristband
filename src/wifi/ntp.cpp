// Edits and additions by John Heenan 2020
#include "ntp.hpp"
#include <WiFiUdp.h>
#include <NTP.h>
#include "clock.hpp"

WiFiUDP wifiUdp;
NTP ntp(wifiUdp);

void initNTP()
{
  if (tz_uses_dst)
  {
    ntp.ruleDST(tz_dst_name, tz_week, tz_wday, tz_dst_month, tz_dst_hour, tz_dst_offset);
    ntp.ruleSTD(tz_std_name, tz_week, tz_wday, tz_std_month, tz_std_hour, tz_std_offset);
  }
  else
  {
    ntp.isDST(false);
    ntp.timeZone(tz_offset / 60, tz_offset % 60);
  }
  ntp.begin();
  ntp.update();
}

RTC_Date syncTime()
{
  initNTP();
  RTC_Date datetime = RTC_Date(ntp.year(), ntp.month(), ntp.day(), ntp.hours(), ntp.minutes(), ntp.seconds());
  // DEBUG_MSG("NTP returned: year %d, month %d, day %d, hours %d, minutes %d, seconds %d\n", ntp.year(), ntp.month(), ntp.day(), ntp.hours(), ntp.minutes(), ntp.seconds());
  ntp.stop();
  return datetime;
}
