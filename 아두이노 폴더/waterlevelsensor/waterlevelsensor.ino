int waterSensor = A0;                           // 수위센서 A0핀 설정
int waterValue = 0;                               // loop에서 사용할 변수 설정

void setup() 
{
  Serial.begin (9600);                    // 시리얼 통신 설정(보드레이트 9600)
}

void loop() 
{
  watervValue = analogRead(waterSensor);     // 수위센서값을 읽어 변수 value에 저장
  Serial.println(waterValue);                   // 저장된 센서값을 시리얼 모니터에 출력
  delay(1000);                               // 1초의 딜레이
}