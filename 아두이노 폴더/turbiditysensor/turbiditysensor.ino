void setup() {
  Serial.begin(9600); // 시리얼 통신 세팅
  pinMode(A5,INPUT); // 탁도센서 A5번 핀 입력 설정

}

void loop() {
  int turbidityData = analogRead(A5);
  Serial.println(turbidityData);
  delay(1000);
}
