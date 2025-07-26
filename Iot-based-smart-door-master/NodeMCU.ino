#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "temp id here"
#define BLYNK_TEMPLATE_NAME "Smart Door"
#define BLYNK_AUTH_TOKEN "token here"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "SlspTRYq_Oun1CONo_F4XZ-Hc2ryGbC-"; // Replace with your Blynk Auth Token
char ssid[] = "Aditya";
char pass[] = "1236547890";

BlynkTimer timer;
const int doorSensorPin = 4;
bool isDoorUnlocked = false; // Track door status

void setup() {
  Serial.begin(9600);
  pinMode(doorSensorPin, INPUT);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendDoorStatus);
}

void loop() {
  Blynk.run();
  timer.run();
}

BLYNK_WRITE(V1) {
  int pinValue = param.asInt();
  if (pinValue == 1 && digitalRead(doorSensorPin) == LOW) { // Check if door is closed
    Serial.println("Unlock command received");
    // Send command to Arduino to unlock the door
    Serial.println("unlock");
    isDoorUnlocked = true; // Update door status
    Blynk.logEvent("door_unlocked", "Door Unlocked!");
  }
}

void sendDoorStatus() {
  // Send door status to Blynk app
  if (digitalRead(doorSensorPin) == HIGH) {
    Blynk.virtualWrite(V2, "Door is open");
    isDoorUnlocked = false; // Reset door status when open
  } else {
    Blynk.virtualWrite(V2, "Door is closed");
    isDoorUnlocked = false; // Reset door status when closed
  }
}
