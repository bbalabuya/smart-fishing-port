#define BLYNK_PRINT Serial

// Blynk 설정
#define BLYNK_TEMPLATE_ID "TMPL6jh6_sE8p"
#define BLYNK_TEMPLATE_NAME "kang test"
#define BLYNK_AUTH_TOKEN "UK6KJqd7qmgvDrms76McFkrTNwrfwgnL"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi 설정
char ssid[] = "U+NetF391"; // WiFi 이름
char pass[] = "2901410FM!"; // WiFi 비밀번호

// 온도 센서 설정
#define ONE_WIRE_BUS D2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// 수위 센서 설정 (ESP8266의 A0 사용)
int waterSensor = D0;
int waterValue = 0;

// pH 센서 설정
#define SensorPin A0
#define Offset 0.11
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth 40
int pHArray[ArrayLenth];
int pHArrayIndex = 0;

// 탁도 센서 설정 (ESP8266의 D1 사용)
#define TURBIDITY_SENSOR_PIN D1

void setup() {
  // 시리얼 시작
  Serial.begin(9600);
  
  // Blynk 시작
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  // 온도 센서 시작
  sensors.begin();
  
  // 핀 모드 설정
  pinMode(TURBIDITY_SENSOR_PIN, INPUT); // 탁도 센서 D1 핀 입력 설정
}

void loop() {
  // Blynk 처리
  Blynk.run();
  
  // 온도 센서 데이터 읽기
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  
  // 수위 센서 데이터 읽기
  waterValue = analogRead(waterSensor);
  
  // 탁도 센서 데이터 읽기
  int turbidityData = analogRead(TURBIDITY_SENSOR_PIN);
  
  // pH 센서 데이터 읽기
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;
  
  if (millis() - samplingTime > samplingInterval) {
    pHArray[pHArrayIndex++] = analogRead(SensorPin);
    if (pHArrayIndex == ArrayLenth) pHArrayIndex = 0;
    voltage = avergearray(pHArray, ArrayLenth) * 5.0 / 1024;
    pHValue = 3.5 * voltage + Offset;
    samplingTime = millis();
  }
  
  // 데이터를 Blynk로 전송
  if (millis() - printTime > printInterval) {
    Blynk.virtualWrite(V1, temperatureC);
    Blynk.virtualWrite(V2, waterValue);
    Blynk.virtualWrite(V3, turbidityData);
    Blynk.virtualWrite(V4, pHValue);
    printTime = millis();
  }
  
  delay(1000); // 1초 대기
}

// pH 센서 전압 계산 함수
double avergearray(int* arr, int number) {
  int i;
  int max, min;
  double avg;
  long amount = 0;
  
  if (number <= 0) {
    Serial.println("Error number for the array to averaging!/n");
    return 0;
  }
  
  if (number < 5) {
    for (i = 0; i < number; i++) {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
  } else {
    if (arr[0] < arr[1]) {
      min = arr[0];
      max = arr[1];
    } else {
      min = arr[1];
      max = arr[0];
    }
    for (i = 2; i < number; i++) {
      if (arr[i] < min) {
        amount += min;
        min = arr[i];
      } else {
        if (arr[i] > max) {
          amount += max;
          max = arr[i];
        } else {
          amount += arr[i];
        }
      }
    }
    avg = (double)amount / (number - 2);
  }
  
  return avg;
}
