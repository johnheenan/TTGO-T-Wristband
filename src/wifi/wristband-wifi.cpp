#include "wristband-wifi.hpp"


#define AUTOCONNECT_APNAME "T-Wristband"

WiFiManager wifiManager;

void configModeCallback(WiFiManager *myWiFiManager)
{
  char apName[50] = "";
  myWiFiManager->getConfigPortalSSID().toCharArray(apName, 50);
  // wifiManagerAdvice(apName);
  msgInfo("Connect to AP then IP", apName,  WiFi.softAPIP().toString().c_str());
}

void setupWiFi()
{
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setBreakAfterConfig(true);
  wifiManager.setTimeout(2 * SECS_PER_MINUTES);
  wifiManager.autoConnect(AUTOCONNECT_APNAME);
}

void activateWifi()
{
  WiFi.mode(WIFI_STA);
  sleep(1);
  if (WiFi.status() != WL_CONNECTED)
    setupWiFi();
}

void deactivateWifi()
{
  WiFi.mode(WIFI_OFF);
}