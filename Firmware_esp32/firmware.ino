#include<WiFi.h>
#include<HTTPClient.h>
#include<WiFiManager.h>
#include <WiFiClientSecure.h>
#include"DHT.h"

//gg sheet
String ggScript = "https://script.google.com/macros/s/AKfycbwoLzxNFJMTXMpzU5qS2aHyLXA_XKuinqgtE-sTZ9LrT7FD0useYD-jAvwo10mgw_snBQ/exec";

//Thinkspeak
String server = "http://api.thingspeak.com/update";
String apikey = "1VTC56E8BJS70A48";

//type Sensor
struct DHT_Data{
  float temper;
  float humid;
  const uint8_t pin;
};

struct MQ_Data{
  const uint8_t analogPin;
  int analogVal;
};


//Khai báo chân & cấu hình DHT
MQ_Data mSensor[3]={
  {32,0},{34,0},{35,0}
};

const uint8_t led=19;

DHT_Data dSensor={0,0,18};

#define DHTTYPE DHT11
DHT dht(dSensor.pin,DHTTYPE);

float filtered[3] = {0};// chứa dữ liệu để lọc

//khai báo hàm
float ema(float newVal, float &filtered);
void filterDHT(float &t, float &h);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  //Connect wifi
  Serial.println("Connecting...!");
  WiFi.mode(WIFI_STA); //Mode Client

  WiFiManager wm;

  bool res;

  res=wm.autoConnect("ESP32","nigga12345678");
  if(!res){
    Serial.println("Fail to connect...");
    ESP.restart();
  }
  else{
    Serial.println("WiFi done!");
    Serial.println(WiFi.localIP());
    pinMode(led,OUTPUT);
    digitalWrite(led,HIGH);
  }
}

void loop() {
  // Read MQ;
  /*bool ready = true;

  for(int i=0; i<3; i++){
    mSensor[i].analogVal = analogRead(mSensor[i].analogPin);

    if(mSensor[i].analogVal < 100){
      ready = false;
    }

    mSensor[i].analogVal = ema(mSensor[i].analogVal, filtered[i]);
  }

  if(!ready){
    Serial.println("MQ warming up...");
  }*/
  for(int i=0; i<3; i++){
    mSensor[i].analogVal=analogRead(mSensor[i].analogPin);
    //if(mSensor[i].analogVal<100){ // sensor chưa đủ nóng
      Serial.println("MQ not ready!");
      //return;
    //}
    //mSensor[i].analogVal = ema(mSensor[i].analogVal, filtered[i]);
  }

  //Read DHT
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if(isnan(t) || isnan(h)){
    Serial.println("DHT error!");
    return;
  }

  //filterDHT(t, h);

  dSensor.temper = t;
  dSensor.humid  = h;



  //Sent data to ThinkspeakIoT
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Sending to ThinkSpeak...");
    HTTPClient http;
    String url = server + "?api_key=" + apikey +
             "&field1=" + String(dSensor.temper) +
             "&field2=" + String(dSensor.humid) +
             "&field3=" + String(mSensor[0].analogVal) +
             "&field4=" + String(mSensor[1].analogVal) +
             "&field5=" + String(mSensor[2].analogVal);
    Serial.println(url);
    http.begin(url);

    int httpCode=http.GET();

    if(httpCode == 200){
      String res= http.getString();
      Serial.println("ThinkSpeak response:" + res);
    }else{
      Serial.println("HTTP ERROR: "+ String(httpCode));
    }
    http.end();



    //Sent data to gg sheet
    Serial.println("Sending to Google Sheets...");
    WiFiClientSecure client;
    client.setInsecure(); // bỏ verify SSL (nhanh gọn cho test)

    HTTPClient httpGG;
    String urlGG = ggScript +
                  "?temp=" + String(dSensor.temper, 2) +
                  "&humid=" + String(dSensor.humid, 2) +
                  "&mq1=" + String(mSensor[0].analogVal) +
                  "&mq2=" + String(mSensor[1].analogVal) +
                  "&mq3=" + String(mSensor[2].analogVal);

    Serial.println(urlGG);

    httpGG.begin(client, urlGG);
    httpGG.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

    int httpCodeGG = httpGG.GET();

    if (httpCodeGG == 200) {
        Serial.println("GG OK: " + httpGG.getString());
    } else {
        Serial.println("GG ERROR: " + String(httpCodeGG));
        Serial.println(httpGG.getLocation());
    }
    httpGG.end();

    delay(5000);
  }
}

//Hàm lọc nhiễu
float ema(float newVal, float &filtered) {
    float alpha = 0.2;
    filtered = alpha * newVal + (1 - alpha) * filtered;
    return filtered;
}

void filterDHT(float &t, float &h) {
    static float lastT = t;
    static float lastH = h;

    t = (t + lastT) / 2;
    h = (h + lastH) / 2;

    lastT = t;
    lastH = h;
}
