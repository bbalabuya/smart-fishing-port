#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL6jh6_sE8p"
#define BLYNK_TEMPLATE_NAME         "kang test"
#define BLYNK_AUTH_TOKEN            "UK6KJqd7qmgvDrms76McFkrTNwrfwgnL"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "U+NetF391"; // WiFi 이름
char pass[] = "2901410FM!"; // WiFi 비밀번호

// 수위 센서 설정 (ESP8266의 A0 사용)
int waterSensor = A0;
int waterValue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();

  waterValue = analogRead(waterSensor);
  Blynk.virtualWrite(V2, waterValue);

    delay(1000);
}
