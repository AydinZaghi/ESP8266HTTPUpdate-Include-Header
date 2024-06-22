# ESP8266HTTPUpdate-Include-Header

A copy of the ESPhttpUpdate library with a difference in the update function. In this library, a header (such as jwt) can be inserted directly for OTA update in the Arduino program for ESP8266 by using updateIncludeHeader.

example :

	  #include <ESP8266WiFi.h>
	  #include <ESP8266HTTPClient.h>
	  #include <WiFiClient.h>
	  #include <ESP8266HTTPUpdateIncludeHeader.h>
	  WiFiClient client;
	  HTTPClient http;


	  String host = "http://192.168.31.51:3000/api/esp/";
   
	  ESP8266HTTPUpdateIncludeHeader ESP8266HTTPUpdate;
   
	  ESP8266HTTPUpdate.rebootOnUpdate(false); 
   
	  t_httpUpdate_return ret = ESP8266HTTPUpdate.updateIncludeHeader(client, host+"update-file","x-auth-token", "json web token");
	  switch(ret) {
		case HTTP_UPDATE_FAILED:
		  Serial.print("UPDATE FAILED");
		  break;
		case HTTP_UPDATE_NO_UPDATES:
		  Serial.print("NO UPDATES");
		  break;
		case HTTP_UPDATE_OK:
		  Serial.print("UPDATE HardWare");
		  ESP.restart();
		  break;
	  }
