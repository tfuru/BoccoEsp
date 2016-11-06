
/** BOCCO API for Arduino
* bocco_api.cpp
*
*/
#include "bocco_api.h"
#include <ESP8266WiFi.h>
#include <ESP8266TrueRandom.h>

BoccoAPI::BoccoAPI(char* email, char* password, char* apikey)
  :email(email), password(password), apikey(apikey){
}

/** GETする
 *
 */
String BoccoAPI::get(String url, String data, int retryCnt) {
#ifdef BOCCO_DEBUG
  Serial.println( "get" );
  Serial.println( " url "+url );
  Serial.println( " data "+data );
  Serial.println( " retryCnt "+String(retryCnt) );
#endif

  retryCnt--;
  if(retryCnt <= 0){
    //リトライ回数を超えた。
    return "";
  }

  if (this->client.connect(BoccoAPI::HOST, BoccoAPI::PORT)) {
    this->client.println("GET " + url + "?" + data + " HTTP/1.1");
    this->client.println("Host: " + String(BoccoAPI::HOST));
    this->client.println("User-Agent: "+BoccoAPI::USER_AGENT);
    this->client.println("Connection: close");
    this->client.println();

    String response = "";
    while (this->client.connected()) {
      //if(this->client.available() == 0) break;
      char c = this->client.read();
      response += String(c);
      //Serial.write(c);
      delay(1);
    }
    this->client.stop();
#ifdef BOCCO_DEBUG
    Serial.println();
    Serial.print( "response.length " );
    Serial.println( response.length() );
    //Serial.println( "response" );
    //Serial.println( response );
#endif

    if(response.length() == 0){
      //リトライする
      delay(BoccoAPI::RETRY_DELAY_SEC * 1000);
      return this->get(url,data,retryCnt);
    }

    int bodypos =  response.indexOf("\r\n\r\n") + 4;
    response = response.substring(bodypos);
    //BOCCO サーバの返却にゴミデータが入っているので JSON 部分だけを取り出す
    if(response.indexOf("[{") != -1){
      response = response.substring( response.indexOf("[{") );
      response = response.substring( 0, response.indexOf("}]\r\n") );
    }
    //Serial.println( response );

    return response;
  }
  else {
    return "ERROR";
  }
}

/** POSTする
 *
 */
String BoccoAPI::post(String url, String data, int retryCnt) {
#ifdef BOCCO_DEBUG
  Serial.println( "post" );
  Serial.println( " url "+url );
  Serial.println( " data "+data );
  Serial.println( " retryCnt "+String(retryCnt) );
#endif
  retryCnt--;
  if(retryCnt <= 0){
    //リトライ回数を超えた。
    return "";
  }

  if (this->client.connect(BoccoAPI::HOST, BoccoAPI::PORT)) {
    this->client.println("POST " + url + " HTTP/1.1");
    this->client.println("Host: " + String(BoccoAPI::HOST));
    this->client.println("User-Agent: "+BoccoAPI::USER_AGENT);
    this->client.println("Connection: keep-alive");
    this->client.println("Accept-Language: ja");
    this->client.println("Content-Type: application/x-www-form-urlencoded;");
    this->client.print("Content-Length: ");
    this->client.println(data.length());
    this->client.println();
    this->client.println(data);

    String response = "";
    while (this->client.connected()) {
      if(this->client.available() == 0) break;
      char c = this->client.read();
      response += String(c);
      //Serial.write(c);
      delay(1);
    }
    this->client.stop();
#ifdef BOCCO_DEBUG
    Serial.print( "response.length " );
    Serial.println( response.length() );
    //Serial.println( "response" );
    //Serial.println( response );
#endif
    if(response.length() == 0){
      //リトライする
      delay(BoccoAPI::RETRY_DELAY_SEC * 1000);
      return this->post(url,data,retryCnt);
    }

    int bodypos =  response.indexOf("\r\n\r\n") + 4;
    response = response.substring(bodypos);
    //Serial.println( response );

    return response;
  }
  else {
    return "ERROR";
  }
}

/** アクセストークンを取得
*
*/
bool BoccoAPI::createSessions(){
  //POSTパラメータを作る
  const String data = "apikey="+String(apikey)+"&email="+email+"&password="+password;

  //JSONを取得
  String json = this->post(BoccoAPI::API_SESSIONS,data, BoccoAPI::RETRY_CNT);

  //JSONパース
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    return false;
  }

  //アクセストークンを取得
  this->setAccessToken( root["access_token"] );

  return true;
}

/** アクセストークンを設定
 *
 */
void BoccoAPI::setAccessToken(const char* accessToken){
  this->accessToken = String(accessToken);
}

/** チャットルームを取得する
 *
 */
bool BoccoAPI::getFirstRoom(){
  //POSTパラメータを作る
  const String data = "access_token="+this->accessToken;
  //JSONを取得
  const String url = BoccoAPI::API_ROOMS+BoccoAPI::API_ROOMS_JOINED;
  String json = this->get(url,data,BoccoAPI::RETRY_CNT);

  //JSONパースするには大きすぎなので 1件目のルームデータだけ対象にする
  String room = json.substring( json.indexOf("[{\"uuid\":\"")+1 );
  room = room.substring( 0, room.indexOf("\"background_image\"")-1 )+"}\n";

   //JSONパース
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(room);
  if (!root.success()) {
    return false;
  }

  this->firstRoomId = String((const char*) root["uuid"]);

  return true;
}

/** メッセージをルームに送信する
 *
 */
bool BoccoAPI::postMessageText(const char* text){
  //UUIDを生成
  byte uuidNumber[16];
  ESP8266TrueRandom.uuid(uuidNumber);
  const String uid = ESP8266TrueRandom.uuidToString(uuidNumber);

  //POSTパラメータを作る
  const String data = "access_token="+this->accessToken+"&media=text&text="+String(text)+"&unique_id="+uid;
  const String url = BoccoAPI::API_ROOMS+"/"+this->firstRoomId+BoccoAPI::API_ROOMS_MESSAGES;

  //JSONを取得
  String json = this->post(url,data,BoccoAPI::RETRY_CNT);

  return true;
}
