#define BLYNK_PRINT Serial

// 값을 전달 받을 Blynk 설정
#define BLYNK_TEMPLATE_ID "TMPL6jh6_sE8p"
#define BLYNK_TEMPLATE_NAME "kang test"
#define BLYNK_AUTH_TOKEN "UK6KJqd7qmgvDrms76McFkrTNwrfwgnL"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WiFi 설정
char ssid[] = "U+NetF391";
char pass[] = "2901410FM!";


// pH 센서 설정
#define SensorPin A0
#define Offset 0.11
#define samplingInterval 20
#define printInterval 800 //bylnk로 데이터 전송하는 간격
#define ArrayLenth 40
int pHArray[ArrayLenth];
int pHArrayIndex = 0;

void setup() {
  Serial.begin(9600);
  
  // Blynk 시작을 위한 초기화
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  // Blynk 처리
  Blynk.run();
  
  //millis함수는 현재시간을 가져옴
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;
  
  if (millis() - samplingTime > samplingInterval) { // 마지막으로 전송한 시간이 전송하는 간격보다 큰 경우
    pHArray[pHArrayIndex++] = analogRead(SensorPin);
    if (pHArrayIndex == ArrayLenth) pHArrayIndex = 0;
    voltage = avergearray(pHArray, ArrayLenth) * 5.0 / 1024; // 0~1023의 값을 0~5로 변환
    pHValue = 3.5 * voltage + Offset;     // ph공식 = 3.5 x 전압 + 보정 값
    samplingTime = millis(); //현재시간으로 업데이트
  }
  
  // vurtual 4번 핀으로 데이터를 Blynk로 전송
  if (millis() - printTime > printInterval) {
    Blynk.virtualWrite(V4, pHValue);
    printTime = millis();
  }
  
  delay(1000); // 1초 대기
}

// 전압 계산해주는 함수
double avergearray(int* arr, int number) {
  int i;
  int max, min;
  double avg;
  long amount = 0;
  
  if (number <= 0) { // 배열이 없는 경우
    Serial.println("Error number for the array to averaging!/n");
    return 0;
  }
  
  if (number < 5) { //만약 배열이 모자랄 경우
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
