/**
   httpUpdate.ino

    Created on: 06.22.2024

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266HTTPUpdateIncludeHeader.h>

#ifndef APSSID
#define APSSID "APSSID"
#define APPSK "APPSK"
#endif

ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(false);

  Serial.println();

  ESPhttpUpdate.setClientTimeout(2000);  // default was 8000

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(APSSID, APPSK);
}

void update_started() {
  Serial.println("CALLBACK:  HTTP update process started");
}

void update_finished() {
  Serial.println("CALLBACK:  HTTP update process finished");
}

void update_progress(int cur, int total) {
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

void update_error(int err) {
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}


void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    // The line below is optional. It can be used to blink the LED on the board during flashing
    // The LED will be on during download of one buffer of data from the network. The LED will
    // be off during writing that buffer to flash
    // On a good connection the LED should flash regularly. On a bad connection the LED will be
    // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
    // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
    ESP8266HTTPUpdateIncludeHeader.setLedPin(LED_BUILTIN, LOW);

    // Add optional callback notifiers
    ESP8266HTTPUpdateIncludeHeader.onStart(update_started);
    ESP8266HTTPUpdateIncludeHeader.onEnd(update_finished);
    ESP8266HTTPUpdateIncludeHeader.onProgress(update_progress);
    ESP8266HTTPUpdateIncludeHeader.onError(update_error);

    ESP8266HTTPUpdateIncludeHeader ESP8266HTTPUpdate;
    t_httpUpdate_return ret = ESP8266HTTPUpdate.updateIncludeHeader(client, "http://ip:port/update-file", "x-auth-token", "your-jwt");
    
    switch (ret) {
      case HTTP_UPDATE_FAILED: Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str()); break;

      case HTTP_UPDATE_NO_UPDATES: Serial.println("HTTP_UPDATE_NO_UPDATES"); break;

      case HTTP_UPDATE_OK: Serial.println("HTTP_UPDATE_OK"); break;
    }
  }
}
