#include <HTTPClient.h>
#include <WiFi.h>

const int PIN = 39;// Analog pin
int checkpoint;
const char* ssid = "rpi4-ubuntu";
const char* password = "12345678";
String server = "http://10.42.0.1:80/";

void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
  checkpoint = ADC0_promedio(20);
}

void loop(){
  int raw = ADC0_promedio(50);
  float distance = 28940.1 * pow(raw, -1.16);
  Serial.println(distance*10);

  if(distance < 25){
    //Serial.print("SHOT! ->");
    detection(distance*10);
    Serial.println(distance*10);
    //delay(1000);
  }else{
    checkpoint = raw;  
  }
}

int ADC0_promedio(int n)
{
  long suma=0;
  for(int i=0;i<n;i++)
  {
    suma=suma+analogRead(PIN);
  }  
  return(suma/n);
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(10000);
  }
  Serial.println(WiFi.localIP());
}

void detection(float distance){
  if(WiFi.status() == WL_CONNECTED){
      HTTPClient http;
      WiFiClient client;
      http.begin(client, server);
      http.addHeader("Content-Type","application/json");
      String string_distance = "";
      string_distance.concat(distance);
      String httpRequestData = "{\"distance\":\"" + string_distance + "\"}";           
      int httpResponseCode = http.POST(httpRequestData);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      http.end();
      delay(1000);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}
