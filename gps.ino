#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Credenciais WiFi e Blynk
char ssid[] = "SEU_SSID";
char pass[] = "SUA_SENHA";
char auth[] = "SEU_TOKEN_BLYNK";

SoftwareSerial gpsSerial(3, 4);  // RX, TX
TinyGPSPlus gps;

BLYNK_WRITE(V1) {  //V1 é o pin virtual do botão
    if (param.asInt()) {  // se o botão for pressionado
        if (gps.location.isValid()) {
            Blynk.virtualWrite(V2, gps.location.lat());
            Blynk.virtualWrite(V3, gps.location.lng());
        } else {
            Blynk.virtualWrite(V2, "Erro");
            Blynk.virtualWrite(V3, "Erro");
        }
    }
}

void setup() {
    Serial.begin(9600);
    gpsSerial.begin(9600);
    Blynk.begin(auth, ssid, pass);
}

void loop() {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
    }
    Blynk.run();
}
