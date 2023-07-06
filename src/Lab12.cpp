/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/admin/CTD_2023/Lab12/src/Lab12.ino"
#include "MQTT.h"
#include "oled-wing-adafruit.h"
#include "env.h"
#include "blynk.h"

void setup();
void callback2();
void loop();
void resetDisplay();
void callback(char *topic, byte *payload, unsigned int length);
void displayLongAndLat();
#line 6 "/Users/admin/CTD_2023/Lab12/src/Lab12.ino"
SYSTEM_THREAD(ENABLED);

void callback(char* topic, byte* payload, unsigned int length);

MQTT client("lab.thewcl.com", 1883, callback);

OledWingAdafruit display;

float longitude = 0.0;
float latitude = 0.0;
bool getLatAndLongBoolean = false;

Timer getLatAndLong(15000, callback2, false);

BlynkTimer timer;

void setup() {
  display.setup();
  resetDisplay();
  display.print("connecting...");
  display.display();

  Blynk.begin(BLYNK_AUTH_TOKEN);

  Serial.begin(9600);
  while (!Serial.isConnected()) {}

  getLatAndLong.start();
}

void callback2() {
  getLatAndLongBoolean = true;
}

void loop() {
  Blynk.run();
  timer.run();
  display.loop();
  if (client.isConnected()) {
    client.loop();

    if (getLatAndLongBoolean) {
      client.publish("HipposAreTasty/Get", "yo");
      getLatAndLongBoolean = false;
    }

    displayLongAndLat();
  } else {
    client.connect(System.deviceID());
    client.subscribe("HipposAreTasty/longitude");
    client.subscribe("HipposAreTasty/latitude");
  }
}

void resetDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
}

void callback(char *topic, byte *payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String s = p;
  double value = s.toFloat();

  if (String(topic).equals("HipposAreTasty/latitude")) {
    latitude = value;
    Blynk.virtualWrite(V2, latitude);
  } else if (String(topic).equals("HipposAreTasty/longitude")) {
    longitude = value;
    Blynk.virtualWrite(V7, longitude);
  }
}

void displayLongAndLat() {
  resetDisplay();
  display.print("Longitude: ");
  display.println(longitude);
  display.print("Latitude: ");
  display.print(latitude);
  display.display();
}