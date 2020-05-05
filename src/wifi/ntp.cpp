// Edits and additions by John Heenan 2020
#include "ntp.hpp"
#include <WiFiUdp.h>
#include <NTP.h>
#include "clock.hpp"
#include "eeprom.hpp"

WiFiUDP wifiUdp;
NTP ntp(wifiUdp);

void initNTP()
{
  if (settings.tz_uses_dst)
  {
    ntp.ruleDST(settings.tz_dst_name, settings.tz_week, settings.tz_wday, settings.tz_dst_month, settings.tz_dst_hour, settings.tz_dst_offset);
    ntp.ruleSTD(settings.tz_std_name, settings.tz_week, settings.tz_wday, settings.tz_std_month, settings.tz_std_hour, settings.tz_std_offset);
  }
  else
  {
    ntp.isDST(false);
    ntp.timeZone(settings.tz_offset / 60, settings.tz_offset % 60);
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
