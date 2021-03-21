#ifndef Utils_H
#define Utils_H
#include <Arduino.h>

String StringPad(String Str, int Length, char Filler);
void ListDir(const char *Path);
void CatFile(const char *FileName);

// Configuration
//
// This structure is written/read from/to the config file
//
const unsigned int NoOfDNS = 4;
const unsigned int NoOfParameters = 20;
typedef struct {
  String WiFiSSID;
  String WiFiPassword;
  String WiFiIP;
  String WiFiNetMask;
  String WiFiGateway;
  bool WiFiDHCP;
  String WiFiDNS[NoOfDNS];
  //
  String AccessPointSSID;
  String AccessPointPassword;
  String AccessPointIP;
  String AccessPointNetMask;
  String AccessPointGateway;
  //
  unsigned int HttpPort;
  //
  String ParameterName[NoOfParameters];
  String ParameterValue[NoOfParameters];
} Config;
void InitConfig(Config &Conf, const char *DevName);
String ReadConfig(Config &Conf, const char *ConfigFileName, const char *DevName);
void PrintConfig(Config &Conf);
String WriteConfig(Config &Conf, const char *ConfigFileName);

#endif