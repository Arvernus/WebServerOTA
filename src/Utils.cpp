#include "Utils.h"

#include <ArduinoJson.h>
#include <LittleFS.h>

// Print a String with a fixed length
//
String StringPad(String Str, int Length, char Filler) {
  int StrLen = Str.length();
  String Result = "";
  int AdditionalCharacters = 0;
  if (Length < 0) {
    AdditionalCharacters = -Length - StrLen;
    while (--AdditionalCharacters >= 0) {
      Result += Filler;
    }
  }
  Result += Str;
  if (Length > 0) {
    AdditionalCharacters = Length - StrLen;
    while (--AdditionalCharacters >= 0) {
      Result += Filler;
    }
  }
  return (Result);
}

// List a Directory
//
void ListDir(const char *Path) {
  Dir D = LittleFS.openDir(Path);
  Serial.println();
  Serial.print(F("LittleFS File System: "));
  Serial.println(Path);
  Serial.println();
  while (D.next()) {
    Serial.printf("%10d ", D.fileSize());
    Serial.println(D.fileName());
  }
}

// List the content of a File
//
void CatFile(const char *FileName) {
  Serial.print("-");
  Serial.println(StringPad(FileName, 78, '-'));
  File F = LittleFS.open(FileName, "r");
  if (F) {
    int c = F.read();
    while (c >= 0) {
      if (isPrintable(c) || (c == 10) || (c == 13)) {
        Serial.print((char)c);
      } else {
        Serial.print(F("\\x"));
        Serial.print((unsigned int)c, HEX);
      }
      c = F.read();
    }
  }
  Serial.println();
  Serial.println(StringPad("", 79, '-'));
}

// Initialise Configuration to Hard-Coded Defaults
//
void InitConfig(Config &Conf, const char *DevName) {
  Conf.WiFiSSID = "";
  Conf.WiFiPassword = "";
  Conf.WiFiIP = "";
  Conf.WiFiNetMask = "";
  Conf.WiFiGateway = "";
  Conf.WiFiDHCP = true;
  for (unsigned int i = 0; i < NoOfDNS; i++) {
    Conf.WiFiDNS[i] = "";
  }
  Conf.WiFiDNS[0] = F("8.8.8.8");
  //
  String ChipId = String(ESP.getChipId(), HEX);
  ChipId.toUpperCase();
  Conf.AccessPointSSID = DevName + String("-") + ChipId;
  Conf.AccessPointPassword = String(ESP.getChipId(), HEX);
  Conf.AccessPointIP = F("192.168.0.1");
  Conf.AccessPointNetMask = F("255.255.255.0");
  Conf.AccessPointGateway = F("192.168.0.1");
  //
  Conf.HttpPort = 80;
  //
  for (unsigned int i = 0; i < NoOfParameters; i++) {
    Conf.ParameterName[i] = "";
    Conf.ParameterValue[i] = "";
  }
}

// Read the Configuration
//
String ReadConfig(Config &Conf, const char *ConfigFileName, const char *DevName) {
  String Result = "";

  InitConfig(Conf, DevName);

  File F = LittleFS.open(ConfigFileName, "r");
  if (F) {
    StaticJsonDocument<1024> Doc;
    DeserializationError Error = deserializeJson(Doc, F);
    F.close();
    if (Error) {
      Result += F("Reading Config-File failed with Error: ");
      Result += Error.c_str();
    } else {
      Conf.WiFiSSID = Doc[F("WiFi")][F("SSID")] | Conf.WiFiSSID;
      Conf.WiFiPassword = Doc[F("WiFi")][F("Password")] | Conf.WiFiPassword;
      Conf.WiFiIP = Doc[F("WiFi")][F("IP")] | Conf.WiFiIP;
      Conf.WiFiNetMask = Doc[F("WiFi")][F("NetMask")] | Conf.WiFiNetMask;
      Conf.WiFiGateway = Doc[F("WiFi")][F("Gateway")] | Conf.WiFiGateway;
      Conf.WiFiDHCP = Doc[F("WiFi")][F("DHCP")] | Conf.WiFiDHCP;
      JsonArray DNS = Doc[F("WiFi")][F("DNS")].as<JsonArray>();
      if (DNS) {
        unsigned int i;
        for (i = 0; (i < DNS.size()) && (i < NoOfDNS); i++) {
          Conf.WiFiDNS[i] = DNS[i].as<String>();
        }
        while (i < NoOfDNS) {
          Conf.WiFiDNS[i] = "";
          i += 1;
        }
        if (DNS.size() > NoOfDNS) {
          Result += F("Error: More than ");
          Result += String(NoOfDNS);
          Result += F("DNS Entries in Config-File");
        }
      }
      Conf.AccessPointSSID = Doc[F("AccessPoint")][F("SSID")] | Conf.AccessPointSSID;
      Conf.AccessPointPassword = Doc[F("AccessPoint")][F("Password")] | Conf.AccessPointPassword;
      Conf.AccessPointIP = Doc[F("AccessPoint")][F("IP")] | Conf.AccessPointIP;
      Conf.AccessPointNetMask = Doc[F("AccessPoint")][F("NetMask")] | Conf.AccessPointNetMask;
      Conf.AccessPointGateway = Doc[F("AccessPoint")][F("Gateway")] | Conf.AccessPointGateway;
      Conf.HttpPort = Doc[F("HttpPort")] | Conf.HttpPort;
      JsonObject Parameter = Doc[F("Parameter")].as<JsonObject>();
      if (Parameter) {
        unsigned int i = 0;
        for (JsonPair Par : Parameter) {
          Conf.ParameterName[i] = Par.key().c_str();
          Conf.ParameterValue[i] = Par.value().as<String>();
          i += 1;
        }
        while (i < NoOfParameters) {
          Conf.ParameterName[i] = "";
          Conf.ParameterValue[i] = "";
          i += 1;
        }
      }
    }
  } else {
    Result += F("Error: Can not open Config-File: ");
    Result += ConfigFileName;
  }
  return Result;
}

// Convert Configuration to Json
//
String JsonConfig(Config &Conf) {
  StaticJsonDocument<1024> Doc;
  JsonObject WiFi = Doc.createNestedObject(F("WiFi"));
  WiFi[F("SSID")] = Conf.WiFiSSID;
  WiFi[F("Password")] = Conf.WiFiPassword;
  WiFi[F("IP")] = Conf.WiFiIP;
  WiFi[F("NetMask")] = Conf.WiFiNetMask;
  WiFi[F("Gateway")] = Conf.WiFiGateway;
  WiFi[F("DHCP")] = Conf.WiFiDHCP;
  JsonArray DNS;
  for (unsigned int i = 0; i < NoOfDNS; i++) {
    if (Conf.WiFiDNS[i].length()) {
      if (DNS.isNull()) {
        DNS = WiFi.createNestedArray(F("DNS"));
      }
      DNS.add(Conf.WiFiDNS[i]);
    }
  }
  JsonObject AccessPoint = Doc.createNestedObject(F("AccessPoint"));
  AccessPoint[F("SSID")] = Conf.AccessPointSSID;
  AccessPoint[F("Password")] = Conf.AccessPointPassword;
  AccessPoint[F("IP")] = Conf.AccessPointIP;
  AccessPoint[F("NetMask")] = Conf.AccessPointNetMask;
  AccessPoint[F("Gateway")] = Conf.AccessPointGateway;
  Doc[F("HttpPort")] = Conf.HttpPort;
  JsonObject Parameter;
  for (unsigned int i = 0; i < NoOfParameters; i++) {
    if (Conf.ParameterName[i].length()) {
      if (Parameter.isNull()) {
        Parameter = Doc.createNestedObject(F("Parameter"));
      }
      Parameter[Conf.ParameterName[i]] = Conf.ParameterValue[i];
    }
  }
  String Json;
  serializeJsonPretty(Doc, Json);
  return Json;
}

// Print Configuration
//
void PrintConfig(Config &Conf) {
  Serial.println();
  Serial.println(F("*** Configuration ***"));
  Serial.println();
  Serial.println(JsonConfig(Conf));
}

// Write the Configuration
//
String WriteConfig(Config &Conf, const char *ConfigFileName) {
  String Result = "";

  File F = LittleFS.open(ConfigFileName, "w");
  if (F) {
    String Json = JsonConfig(Conf);
    F.write(Json.c_str());
    F.close();
  } else {
    Result += F("Error: Can not create Config-File: ");
    Result += ConfigFileName;
  }
  return Result;
}
