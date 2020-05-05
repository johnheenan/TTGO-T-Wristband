
// Edits and additions by John Heenan 2020. Converted to use Preferences
#include "eeprom.hpp"
#include "timezone.hpp"

Preferences prefs;
settings_t settings;

void storeMagBiasEEPROM(float *magbias)
{
  settings.magbias[0] = magbias[0];
  settings.magbias[1] = magbias[1];
  settings.magbias[2] = magbias[2];
  storeSettings();
}

void getMagBiasEEPROM(float *magbias)
{
  magbias[0] = settings.magbias[0];
  magbias[1] = settings.magbias[1];
  magbias[2] = settings.magbias[2];
}


size_t storeSettings()
{
  return prefs.putBytes("settings", &settings, sizeof(settings));
}

char *strcpy_settings(char *to, const char *from)
{
  size_t len = strlen(from);
  if (len > SETTINGS_STRLEN_MAX)
    len = SETTINGS_STRLEN_MAX;
  len++;
  memcpy(&settings.tz_dst_name, to, len);
  settings.tz_dst_name[len + 1] = 0;
  return to + len;
}

size_t loadSettings() //must be called first before any other functions on this page
{
  prefs.begin("eeprom");
  size_t settingsLen = 0;
#ifndef EEPROM_REINIT
  settingsLen = prefs.getBytesLength("settings");
#endif
  if (settingsLen){
    size_t len= prefs.getBytes("settings", &settings, settingsLen);
    return len;
  }
  else
  {
    settings.magbias[0] = 0.0;
    settings.magbias[1] = 0.0;
    settings.magbias[2] = 0.0;
#ifdef IMU_SKIP
    settings.imu_skip = true;
#else
    settings.imu_skip = false;
#endif
    settings.tz_uses_dst = TZ_USES_DST;
    settings.tz_offset = TZ_OFFSET;
    settings.tz_week = TZ_WEEK;
    settings.tz_wday = TZ_WDAY;

    strcpy_settings(settings.tz_dst_name, TZ_DST_NAME);
    settings.tz_dst_month = TZ_DST_MONTH;
    settings.tz_dst_hour = TZ_DST_HOUR;
    settings.tz_dst_offset = TZ_DST_OFFSET;

    strcpy_settings(settings.tz_std_name, TZ_STD_NAME);
    settings.tz_std_month = TZ_STD_MONTH;
    settings.tz_std_hour = TZ_STD_HOUR;
    settings.tz_std_offset = TZ_STD_OFFSET;

    return storeSettings();
  }
}
