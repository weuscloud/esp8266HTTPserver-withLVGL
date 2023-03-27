#include "ConfigParser.h"
bool ConfigParser::setConfig(const String &name, const String &value)
{
  String upperName = name;
  upperName.toUpperCase();
  if (!isValidName(upperName) || !isValidValue(value))
  {
    return false;
  }
  _isDataChanged = true;
  _configMap[upperName] = value;
  return true;
}

String ConfigParser::getConfig(const String &name)
{
  String upperName = name;
  upperName.toUpperCase();
  if (_configMap.count(upperName) > 0)
  {
    return _configMap[upperName];
  }
  else
  {
    return "";
  }
}

bool ConfigParser::saveConfig()
{
  if (!_isDataChanged)
  {
    Serial.println("Data not changed.Writing finished.");
    return false;
  }
  File configFile = SPIFFS.open(_filePath, "w");
  if (!configFile)
  {
    Serial.println("Failed to open config file for writing");
    return false;
  }
  for (auto &item : _configMap)
  {
    configFile.print(item.first);
    configFile.print("=");
    configFile.println(item.second);
  }
  configFile.close();
  return true;
}

bool ConfigParser::loadConfig()
{
  _configMap.clear();
  bool _isFileOK = false;
  _isDataChanged = false;
  File configFile = SPIFFS.open(_filePath, "r");
  if (!configFile)
  {
    Serial.println("Failed to open config file for reading");
    return false;
  }
  String line;
  while (configFile.available())
  {
    char c = configFile.read();
    if (c == '\r' || c == '\n')
    {
      // ignore leading/trailing whitespace
      line.trim();

      // skip empty lines
      if (line.length() == 0)
      {
        continue;
      }

      // process non-empty lines
      int separatorIndex = line.indexOf('=');
      if (separatorIndex > 0)
      {
        String name = line.substring(0, separatorIndex);
        String value = line.substring(separatorIndex + 1);
        name.toUpperCase();
        if (isValidName(name) && isValidValue(value))
        {
          _isFileOK = true;
          _configMap[name] = value;
        }
      }

      // reset line buffer for next line
      line = "";
    }
    else
    {
      line += c;
    }
  }
  configFile.close();
  return _isFileOK;
}

String ConfigParser::getValueByNameFromFile(const String &attr_name)
{
  String value = "";
  String name = attr_name;
  name.toUpperCase();
  File file = SPIFFS.open(_filePath, "r");
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return value;
  }

  while (file.available())
  {
    String line = file.readStringUntil('\n');
    line.trim(); // 去掉行首和行尾的空格

    int separatorIndex = line.indexOf('=');
    if (separatorIndex == -1)
    {
      // 无效行，跳过
      continue;
    }

    String propertyName = line.substring(0, separatorIndex);
    propertyName.trim();
    propertyName.toUpperCase();
    String propertyValue = line.substring(separatorIndex + 1);
    propertyValue.trim();
    if (propertyName == name && isValidValue(propertyValue))
    {
      value = propertyValue;
      break;
    }
  }

  file.close();
  return value;
}

void ConfigParser::begin()
{
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
  }
}

bool ConfigParser::isValidName(const String &name)
{
  return name.length() > 0 && name.length() <= 31;
}

bool ConfigParser::isValidValue(const String &value)
{
  return value.length() <= 100;
}
