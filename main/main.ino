#include <HTTPClient.h>
#include <WiFi.h>

const int PIN = 39;// Analog pin
const int NETLED = 23;// Led pin
const int ERRLED = 22;// Led pin
const int LED = 19;// Led pin

int checkpoint;
const char* ssid = "rpi4-ubuntu";
const char* password = "12345678";
String server = "http://10.42.0.1:80/";

void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  pinMode(NETLED,OUTPUT);
  pinMode(ERRLED,OUTPUT);
  pinMode(LED,OUTPUT);
  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
  checkpoint = ADC0_promedio(20);
}

void loop(){
  //int raw = ADC0_promedio(3);
  int raw = analogRead(PIN);
  float distance = 289401 * pow(raw, -1.16);
  //float distance = (6787/(raw-3))-4;
  Serial.println(distance);

  if(distance > 20 && distance < 90){
    //int raw2 = ADC0_minimum(3);
    //float distance2 = 289401 * pow(raw2, -1.16);
    //Serial.print("SHOT! ->");
    detection(distance);
    Serial.println(distance);
    delay(1000);
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

int ADC0_minimum(int n)
{
  long suma=0;
  int minimum =200;
  for(int i=0;i<n;i++)
  {
    int readed =analogRead(PIN); 
    if(readed <= minimum){
      minimum = readed;
    }
  }  
  return(minimum);
}

void initWiFi() {
  digitalWrite(ERRLED,HIGH);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(10000);
  }
  Serial.println(WiFi.localIP());
  digitalWrite(NETLED,HIGH);
  digitalWrite(ERRLED,LOW);
}

void detection(float distance){
  if(WiFi.status() == WL_CONNECTED){
      digitalWrite(LED,HIGH);
      HTTPClient http;
      WiFiClient client;
      http.begin(client, server);
      http.addHeader("Content-Type","application/json");
      String string_distance = "";
      string_distance.concat(distance);
      Serial.print(string_distance);
      String httpRequestData = "{\"distance\":\"" + string_distance + "\"}";           
      int httpResponseCode = http.POST(httpRequestData);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      http.end();
      delay(2000);
      digitalWrite(LED,LOW);
    }
    else {
      Serial.println("WiFi Disconnected");
      digitalWrite(NETLED,LOW);
      digitalWrite(ERRLED,HIGH);
    }
}