#include <WiFi.h>

const int PIN = 39;// Analog pin
int checkpoint;
const char* ssid = "secret";
const char* password = "secret";

void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
  checkpoint = ADC0_promedio(20);
}

void loop(){
  int raw = ADC0_promedio(20);
  float distance = 289401 * pow(raw, -1.16);
  Serial.println(distance);

  if(abs(checkpoint - raw) > 200){
    Serial.print("SHOT! ->");
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

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}
