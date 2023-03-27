#include "ConfigParser.h"
void setup()
{
    ConfigParser cfg;
    if (!cfg.loadConfig())
        return;
    Serial.print(cfg.getConfig("ssid"));
    cfg.setConfig("ssid", "MYWIFI");
    if (cfg.saveConfig())
        ;
}
void loop()
{
}