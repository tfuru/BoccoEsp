/** BOCCO API for Arduino
* bocco_api.h
*/
#ifndef BoccoAPI_H_
#define BoccoAPI_H_

#include "arduino.h"
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

//デバッグを有効にする場合 定義する
//#define BOCCO_DEBUG

class BoccoAPI
{
public:
 BoccoAPI(char* email, char* password, char* apikey);

public:
  bool createSessions();
  void setAccessToken(const char* accessToken);
  bool getFirstRoom();
  bool postMessageText(const char* text);

private:
  String get(String url, String data,int retryCnt);
  String post(String url, String data,int retryCnt);

private:
 WiFiClientSecure client;
 char* email;
 char* password;
 char* apikey;

 String accessToken;
 String firstRoomId;

private:
 //ユーザーエージェント
 const String USER_AGENT = "Yanari-ESP8266";

 //リトライ回数
 const int RETRY_CNT = 3;
 const int RETRY_DELAY_SEC = 3;

 const char* HOST = "api.bocco.me";
 const int PORT = 443;

 const String API_SESSIONS = "/alpha/sessions";
 const String API_ROOMS = "/alpha/rooms";
 const String API_ROOMS_JOINED = "/joined";
 const String API_ROOMS_MESSAGES = "/messages";
};

#endif
