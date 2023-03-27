/**
 * @file ConfigParser.h
 * @brief 从文件系统中加载配置
 *
 * 该文件由 weuscloud@gmail.com 编写
 * 最后修改时间：Mon Mar 27 2023
 */
#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H
#include <ESP8266WiFi.h>
#include <FS.h>
#include <map>
#define DEFAULT_CONFIG_FILENAME "/app_config"
class ConfigParser
{
public:
  ConfigParser(const String &filePath)
  {

    _filePath = filePath;
    begin();
  }

  ConfigParser()
  {
    _filePath = DEFAULT_CONFIG_FILENAME;
    begin();
  }
  ~ConfigParser()
  {
    SPIFFS.end();
  }
  bool setConfig(const String &name, const String &value);
  String getConfig(const String &name);
  bool saveConfig();
  bool loadConfig();
  String getValueByNameFromFile(const String &attr_name);

private:
  String _filePath;
  std::map<String, String> _configMap;
  bool _isDataChanged = false;
  void begin();
  bool isValidName(const String &name);

  bool isValidValue(const String &value);
};

#endif