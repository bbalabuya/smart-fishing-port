#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL6jh6_sE8p"
#define BLYNK_TEMPLATE_NAME         "kang test"
#define BLYNK_AUTH_TOKEN            "UK6KJqd7qmgvDrms76McFkrTNwrfwgnL"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "U+NetF391";
char pass[] = "2901410FM!";

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  Blynk.run();
}

