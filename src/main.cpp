//
// ESP8266 Framework Program with Web-Server and
// WIFI Setup per Web-Front-End.
//
#include <Arduino.h>
#include <LittleFS.h>
//#include <ESP8266WiFi.h>
//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>
#include "Utils.h"

// Global Definitions from Build System
//
#ifndef DEVICE_NAME
#define DEVICE_NAME "TestDevice"
#endif
#ifndef VERSION
#define VERSION "Test"
#endif

void setup()
{
    Serial.begin(115200);
    delay(3000);
    Serial.println();
    Serial.println("******************************");
    Serial.println("*                            *");
    Serial.print("*  ");
    Serial.print(StringPad(DEVICE_NAME, 24, ' '));
    Serial.println("  *");
    Serial.println("*                            *");
    Serial.println("******************************");
    Serial.println("Start");
    Serial.println("Start");

    LittleFS.begin();
    Dir D = LittleFS.openDir("/");
    while (D.next())
    {
        Serial.println(D.fileName());
        Serial.print("-");
        Serial.println(StringPad(D.fileName(), 39, '-'));
        File F = D.openFile("r");
        int c = 0;
        while (c >= 0)
        {
            c = F.read();
            if (c >= 0)
            {
                Serial.print((char)c);
            }
        }
        Serial.println();
        Serial.println("----------------------------------------");
    }

    pinMode(LED_BUILTIN, OUTPUT);
}

void ShowLED(int Pin, int Delay, bool On)
{
    if (On)
    {
        digitalWrite(Pin, LOW);
        Serial.println("The LED is on");
    }
    else
    {
        digitalWrite(Pin, HIGH);
        Serial.println("The LED is off");
    }
    delay(Delay);
}

void loop()
{
    ShowLED(LED_BUILTIN, 1000, true);
    ShowLED(LED_BUILTIN, 1000, false);
}