#define ONE_WIRE_BUS 2
//디지털 핀 2번에서 온도센서 정보 받아옴

#include <OneWire.h>
#include <DallasTemperature.h>
//온도센서는 아날로그말고 디지털 핀으로 정보를 받아옴

// 온도센서 정보 가져오기
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// 수위센서 세팅
int waterSensor = A0;           // 수위센서 A0핀 설정
int waterValue = 0;             // loop에서 사용할 변수

void setup(void)
{
  Serial.begin(9600); // 시리얼 통신 준비

  sensors.begin(); //온도센서 초기화
  pinMode(A5,INPUT); // 탁도센서 A5번 핀 입력 설정
}

void loop(void)
{ 

  //온도센서 받아오기
  sensors.requestTemperatures(); 

  float temperatureC = sensors.getTempCByIndex(0); // 섭씨온도로 표시
  Serial.println("온도: " + String(temperatureC)); // 시리얼 포트로 온도 정보 전송
  //온도센서 루프

  //수위센서 루프
  waterValue = analogRead(waterSensor);     // 수위센서 값을 읽어 변수 value에 저장
  Serial.println("물높이: " + String(waterValue));    // 수위센서  출력
  //수위센서 루프

  //탁도센서 루프
  int turbidityData = analogRead(A5); // 탁도센서 값 받아오기
  Serial.println("탁도" + String(turbidityData)); // 탁도센서 출력
  //탁도센서 루프

  Serial.println();
  delay(2000); // 2초 대기
}
