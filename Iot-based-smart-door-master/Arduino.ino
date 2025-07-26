#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address of the LCD
Servo servoMotor;
SoftwareSerial espSerial(0, 1);    // RX to D10, TX to D12 for communication with NodeMCU

String correctPassword = "1234";     // Simplified password storage
String inputPassword = "";            // To collect the entered password

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};    // D2 to D5 for rows
byte colPins[COLS] = {6, 7, 8, 9};    // D6 to D9 for columns
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

int doorSensorPin = 13;               // MC38 door sensor pin

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);              // Begin communication with NodeMCU
  lcd.begin(16, 2);
  lcd.backlight();

  pinMode(doorSensorPin, INPUT_PULLUP);  // Configure MC38 door sensor
  
  servoMotor.attach(11);                 // Servo motor attached to D11
  lockDoor();                            // Start with the door locked

  lcd.print(" Electronic ");
  lcd.setCursor(0, 1);
  lcd.print(" Keypad Lock ");
  delay(2000);
  lcd.clear();
  lcd.print("Enter Passkey:");
}

void loop() {
  char customKey = customKeypad.getKey();

  // Check if unlock command received from NodeMCU
  if (Serial.available()) {
    String command = espSerial.readStringUntil('\n');
    if (command.indexOf("unlock") >= 0) {
      unlockDoor();
      waitForDoorToClose();  // Wait for door to close before locking
    }
  }

  if (customKey == '#') {
    changePassword();
  }

  if (customKey) {
    inputPassword += customKey;
    lcd.print('');  // Display '' instead of the actual key

    // Check if the entered password is correct when 4 characters are entered
    if (inputPassword.length() == 4) {
      delay(200);
      if (inputPassword == correctPassword) {
        unlockDoor();
        lcd.clear();
        lcd.print("Passkey Accepted");
        delay(2000);
        lcd.clear();
        lcd.print("Enter Passkey:");
        waitForDoorToClose();
      } else {
        lcd.clear();
        lcd.print("Access Denied");
        delay(2000);
        lcd.clear();
        lcd.print("Enter Passkey:");
      }
      inputPassword = "";  // Clear input after checking
    }
  }
}

void changePassword() {
  lcd.clear();
  lcd.print("Enter New Passkey:");
  String newPass = "";
  while (newPass.length() < 4) {
    char key = customKeypad.getKey();
    if (key) {
      newPass += key;
      lcd.print('*');
    }
  }
  correctPassword = newPass;  // Update the correct password
  lcd.clear();
  lcd.print("Password Changed");
  delay(2000);
  lcd.clear();
  lcd.print("Enter Passkey:");
}

void lockDoor() {
  servoMotor.write(0);  // Lock position
}

void unlockDoor() {
  servoMotor.write(90);  // Unlock position
  delay(5000);           // Wait 5 seconds before checking door status
}

void waitForDoorToClose() {
  while (digitalRead(doorSensorPin) == HIGH) {
    // Door is open; wait until it closes
    delay(100);  // Small delay to avoid rapid polling
  }
  lockDoor();  // Lock door once the sensor detects it's closed
}