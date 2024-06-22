/**
 *
 * @file ESP8266HTTPUpdateIncludeHeader.cpp
 * @date 06.22.2024
 * @author Aydin Zaghi
 *
 * Copyright (c) 2024 Aydin Zaghi. All rights reserved.
 * This file is part of the ESP8266 Http Updater with Header.
 *
 */

#ifndef ESP8266HTTPUPDATEINCLUDEHEADER_H_
#define ESP8266HTTPUPDATEINCLUDEHEADER_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>

#ifdef DEBUG_ESP_HTTP_UPDATE
#ifdef DEBUG_ESP_PORT
#define DEBUG_HTTP_UPDATE(fmt, ...) DEBUG_ESP_PORT.printf_P( (PGM_P)PSTR(fmt), ## __VA_ARGS__ )
#endif
#endif

#ifndef DEBUG_HTTP_UPDATE
#define DEBUG_HTTP_UPDATE(...) do { (void)0; } while(0)
#endif

/// note we use HTTP client errors too so we start at 100
//TODO - in v3.0.0 make this an enum
constexpr int HTTP_UE_TOO_LESS_SPACE            = (-100);
constexpr int HTTP_UE_SERVER_NOT_REPORT_SIZE    = (-101);
constexpr int HTTP_UE_SERVER_FILE_NOT_FOUND     = (-102);
constexpr int HTTP_UE_SERVER_FORBIDDEN          = (-103);
constexpr int HTTP_UE_SERVER_WRONG_HTTP_CODE    = (-104);
constexpr int HTTP_UE_SERVER_FAULTY_MD5         = (-105);
constexpr int HTTP_UE_BIN_VERIFY_HEADER_FAILED  = (-106);
constexpr int HTTP_UE_BIN_FOR_WRONG_FLASH       = (-107);
constexpr int HTTP_UE_SERVER_UNAUTHORIZED       = (-108);

enum HTTPUpdateResult {
    HTTP_UPDATE_FAILED,
    HTTP_UPDATE_NO_UPDATES,
    HTTP_UPDATE_OK
};

typedef HTTPUpdateResult t_httpUpdate_return; // backward compatibility

using HTTPUpdateStartCB = std::function<void()>;
using HTTPUpdateEndCB = std::function<void()>;
using HTTPUpdateErrorCB = std::function<void(int)>;
using HTTPUpdateProgressCB = std::function<void(int, int)>;

class ESP8266HTTPUpdateIncludeHeader
{
public:
    ESP8266HTTPUpdateIncludeHeader(void);
    ESP8266HTTPUpdateIncludeHeader(int httpClientTimeout);
    ~ESP8266HTTPUpdateIncludeHeader(void);

    void rebootOnUpdate(bool reboot)
    {
        _rebootOnUpdate = reboot;
    }

    /**
     * set true to follow redirects.
     * @param follow
     * @deprecated Please use `setFollowRedirects(followRedirects_t follow)`
      */
    void followRedirects(bool follow) __attribute__ ((deprecated))
    {
        _followRedirects = follow ? HTTPC_STRICT_FOLLOW_REDIRECTS : HTTPC_DISABLE_FOLLOW_REDIRECTS;
    }
    /**
      * set redirect follow mode. See `followRedirects_t` enum for available modes.
      * @param follow
      */
    void setFollowRedirects(followRedirects_t follow)
    {
        _followRedirects = follow;
    }

    void closeConnectionsOnUpdate(bool sever)
    {
        _closeConnectionsOnUpdate = sever;
    }

    void setLedPin(int ledPin = -1, uint8_t ledOn = HIGH)
    {
        _ledPin = ledPin;
        _ledOn = ledOn;
    }

    void setMD5sum(const String &md5Sum) 
    {
        _md5Sum = md5Sum;
    }

    void setAuthorization(const String& user, const String& password);
    void setAuthorization(const String& auth);

    t_httpUpdate_return update(WiFiClient& client, const String& url, const String& currentVersion = "");
    t_httpUpdate_return updateIncludeHeader(WiFiClient& client, const String& url, const String& typeHeader, const String& valueHeader, const String& currentVersion = "");
    t_httpUpdate_return update(WiFiClient& client, const String& host, uint16_t port, const String& uri = "/",
                               const String& currentVersion = "");
    t_httpUpdate_return updateFS(WiFiClient& client, const String& url, const String& currentVersion = "");
    t_httpUpdate_return update(HTTPClient& httpClient, const String& currentVersion = "");
    t_httpUpdate_return updateFS(HTTPClient& httpClient, const String& currentVersion = "");
    
    // Notification callbacks
    void onStart(HTTPUpdateStartCB cbOnStart)          { _cbStart = cbOnStart; }
    void onEnd(HTTPUpdateEndCB cbOnEnd)                { _cbEnd = cbOnEnd; }
    void onError(HTTPUpdateErrorCB cbOnError)          { _cbError = cbOnError; }
    void onProgress(HTTPUpdateProgressCB cbOnProgress) { _cbProgress = cbOnProgress; }

    int getLastError(void);
    String getLastErrorString(void);

    void setClientTimeout(int timeout) {
        _httpClientTimeout = timeout;
    }
protected:
    t_httpUpdate_return handleUpdate(HTTPClient& http, const String& currentVersion, bool spiffs = false);
    bool runUpdate(Stream& in, uint32_t size, const String& md5, int command = U_FLASH);

    // Set the error and potentially use a CB to notify the application
    void _setLastError(int err) {
        _lastError = err;
        if (_cbError) {
            _cbError(err);
        }
    }
    int _lastError;
    bool _rebootOnUpdate = true;
    bool _closeConnectionsOnUpdate = true;
    String _user;
    String _password;
    String _auth;
    String _md5Sum;
private:
    int _httpClientTimeout;
    followRedirects_t _followRedirects = HTTPC_DISABLE_FOLLOW_REDIRECTS;

    // Callbacks
    HTTPUpdateStartCB    _cbStart;
    HTTPUpdateEndCB      _cbEnd;
    HTTPUpdateErrorCB    _cbError;
    HTTPUpdateProgressCB _cbProgress;

    int _ledPin = -1;
    uint8_t _ledOn;
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_HTTPUPDATE)
extern ESP8266HTTPUpdateIncludeHeader ESPHTTPUpdateIncludeHeader;
#endif

#endif /* ESP8266HTTPUPDATEINCLUDEHEADER_H_ */
