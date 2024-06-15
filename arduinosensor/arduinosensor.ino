#define ONE_WIRE_BUS 2
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int waterSensor = A0;
int waterValue = 0;

#define SensorPin A3
#define Offset 0.11
#define samplingInterval 20
#define printInterval 500  // 0.5초마다 출력
#define ArrayLenth 40
int pHArray[ArrayLenth];
int pHArrayIndex = 0;

// LED 핀 설정
#define TEMP_LED_PIN 8
#define WATER_LEVEL_LED_PIN 9
#define TURBIDITY_LED_PIN 10
#define PH_LED_PIN 11

void setup(void) {
  Serial.begin(9600);
  sensors.begin();
  pinMode(A5, INPUT);
  
  // LED 핀을 출력 모드로 설정
  pinMode(TEMP_LED_PIN, OUTPUT);
  pinMode(WATER_LEVEL_LED_PIN, OUTPUT);
  pinMode(TURBIDITY_LED_PIN, OUTPUT);
  pinMode(PH_LED_PIN, OUTPUT);
}

void loop(void) {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);

  waterValue = analogRead(waterSensor);

  int turbidityData = analogRead(A5);

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

  if (millis() - printTime > printInterval) {
    Serial.print("{\"temperature\":");
    Serial.print(temperatureC);
    Serial.print(", \"water_level\":");
    Serial.print(waterValue);
    Serial.print(", \"turbidity\":");
    Serial.print(turbidityData);
    Serial.print(", \"ph\":");
    Serial.print(pHValue, 2);
    Serial.println("}");

    // 온도 경고 LED
    if (temperatureC < 25 || temperatureC > 29) {
      digitalWrite(TEMP_LED_PIN, HIGH); // 온도 경고 조건이 발생하면 LED 켜기
    } else {
      digitalWrite(TEMP_LED_PIN, LOW); // 온도 경고 조건이 해제되면 LED 끄기
    }

    // 수위 경고 LED
    if (waterValue < 100) {
      digitalWrite(WATER_LEVEL_LED_PIN, HIGH); // 수위 경고 조건이 발생하면 LED 켜기
    } else {
      digitalWrite(WATER_LEVEL_LED_PIN, LOW); // 수위 경고 조건이 해제되면 LED 끄기
    }

    // 탁도 경고 LED
    if (turbidityData < 500) {
      digitalWrite(TURBIDITY_LED_PIN, HIGH); // 탁도 경고 조건이 발생하면 LED 켜기
    } else {
      digitalWrite(TURBIDITY_LED_PIN, LOW); // 탁도 경고 조건이 해제되면 LED 끄기
    }

    // pH 경고 LED
    if (pHValue < 6 || pHValue > 9) {
      digitalWrite(PH_LED_PIN, HIGH); // pH 경고 조건이 발생하면 LED 켜기
    } else {
      digitalWrite(PH_LED_PIN, LOW); // pH 경고 조건이 해제되면 LED 끄기
    }

    printTime = millis();
  }

  delay(500);
}

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

