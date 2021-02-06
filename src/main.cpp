//
// ESP8266 Framework Program with Web-Server and
// WIFI Setup per Web-Front-End.
//
#include <Arduino.h>
#include <LittleFS.h>
//#include <ESP8266WiFi.h>
//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>
//#include <EEPROM.h>

// Global Definitions
//
#define DEVICE_NAME "TestDevice"

// Global Variables
//
// AsyncWebServer server(80);
const char* ssid = "WagnerNetz_24";
const char* password = "MivHouchYa2@9";
const char* PARAM_MESSAGE = "message";


// void notFound(AsyncWebServerRequest *request) {
//     request->send(404, "text/plain", "Not found");
// }

void setup() {
    Serial.begin(115200);
    Serial.println();
    Serial.println("******************************");
    Serial.println("*                            *");
    Serial.println("*                            *");
    Serial.println("*                            *");
    Serial.println("******************************");
    delay(500);
    Serial.println("Start");
    Serial.setDebugOutput(true);
 //   WiFi.mode(WIFI_STA);
 //   WiFi.begin(ssid, password);
    Serial.println("Start");
 //   WiFi.printDiag(Serial);
 //   if (WiFi.waitForConnectResult() != WL_CONNECTED) {
 //       Serial.printf("WiFi Failed!\n");
 //       return;
 // }

//    Serial.print("IP Address: ");
//    Serial.println(WiFi.localIP());

//    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//        request->send(200, "text/plain", "Hello, world");
//    });

    // Send a GET request to <IP>/get?message=<message>
//    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
//          String message;
//        if (request->hasParam(PARAM_MESSAGE)) {
//            message = request->getParam(PARAM_MESSAGE)->value();
//        } else {
//            message = "No message sent";
//        }
//        request->send(200, "text/plain", "Hello, GET: " + message);
//    });

    // Send a POST request to <IP>/post with a form field message set to <message>
    // server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
    //     String message;
    //     if (request->hasParam(PARAM_MESSAGE, true)) {
    //         message = request->getParam(PARAM_MESSAGE, true)->value();
    //     } else {
    //         message = "No message sent";
    //     }
    //     request->send(200, "text/plain", "Hello, POST: " + message);
    // });

    // server.onNotFound(notFound);

    // server.begin();
}

void loop() {
}