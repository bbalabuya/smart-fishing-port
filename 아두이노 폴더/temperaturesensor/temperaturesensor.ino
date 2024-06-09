#define ONE_WIRE_BUS 2
//디지털 핀 2번에서 정보 받아옴

#include <OneWire.h>
#include <DallasTemperature.h>
//아날로그말고 디지털로 연결해야  

// 정보 가져오기
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
}

void loop(void)
{ 
  sensors.requestTemperatures(); 

  float temperatureC = sensors.getTempCByIndex(0); // 섭씨온도로 표시
  Serial.println(temperatureC); // 시리얼 포트로 데이터 전송하기

  delay(100); // 1초 대기
}
