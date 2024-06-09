#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6jh6_sE8p"
#define BLYNK_TEMPLATE_NAME "kang test"
#define BLYNK_AUTH_TOKEN "UK6KJqd7qmgvDrms76McFkrTNwrfwgnL"

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

char ssid[] = "U+NetF391";
char pass[] = "2901410FM!";

void setup(void)
{
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  sensors.begin();
}

void loop(void)
{
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.println(temperatureC);

  int scaledValue = map(temperatureC, -40, 125, 0, 100); // 온도 범위를 0~100으로 매핑
  Blynk.virtualWrite(V1, scaledValue);

  delay(1000);
  Blynk.run();
}


