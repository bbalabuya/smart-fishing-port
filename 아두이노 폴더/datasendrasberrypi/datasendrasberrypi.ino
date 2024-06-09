#define ONE_WIRE_BUS 2
// 디지털 핀 2번에서 온도센서 정보 받아옴

#include <OneWire.h>
#include <DallasTemperature.h>
// 온도센서 아날로그말고 디지털 핀으로 받아옴

// 온도센서 정보 가져오기
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// 수위센서 세팅
int waterSensor = A0;           // 수위센서 A0핀 설정
int waterValue = 0;             // loop에서 사용할 변수 설정

// pH센서 세팅
#define SensorPin A3
#define Offset 0.11 // 수돗물 기준 pH를 7에 맞춘 세팅값(조절해주면 됨)
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth 40
int pHArray[ArrayLenth];
int pHArrayIndex = 0;

void setup(void)
{
  Serial.begin(9600); // 시리얼 통신 준비

  sensors.begin(); // 온도센서 세팅
  pinMode(A5, INPUT); // 탁도센서 A5번 핀 입력 설정
}

void loop(void)
{ 
  // 온도센서 루프
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0); // 섭씨온도로 표시
  
  // 수위센서 루프
  waterValue = analogRead(waterSensor);     // 수위센서 값을 읽어 변수 waterValue에 저장

  // 탁도센서 루프
  int turbidityData = analogRead(A5);

  // pH센서 루프
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;
  if (millis() - samplingTime > samplingInterval) {
    pHArray[pHArrayIndex++] = analogRead(SensorPin);
    if (pHArrayIndex == ArrayLenth) pHArrayIndex = 0;
    voltage = avergearray(pHArray, ArrayLenth) * 5.0 / 1024;
    pHValue = 3.5 * voltage + Offset; //3.5 곱하기 전압 더하기 기준값으로 전압이 필요함
    samplingTime = millis();
  }
  
  if (millis() - printTime > printInterval) {
    // 시리얼 출력 포맷을 JSON 형태로 변경
    String data = String("{\"temp\":") + temperatureC + 
                  ",\"water\":" + waterValue + 
                  ",\"turbidity\":" + turbidityData + 
                  ",\"pH\":" + pHValue + "}";
                  
    Serial.println(data);
    printTime = millis();
  }
  
  delay(1000); // 1초 대기
}

// pH센서 전압 구하는 함수
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
