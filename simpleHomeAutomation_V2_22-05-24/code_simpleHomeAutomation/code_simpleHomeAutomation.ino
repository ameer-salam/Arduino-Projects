// Author: Ameer Salam
// Date: 24th May, 2024

/*
This program allows control of various devices via an HC-05 Bluetooth module.
You can control:
- A light connected to pin 13
- A table lamp connected to pin 11
- A fan connected to pin 10 (relay-controlled)
- A door lock mechanism controlled by a servo motor on pin 9

Commands sent via Bluetooth can turn these devices on/off and control the door lock.
*/

#include <SoftwareSerial.h> // Library for serial communication on other digital pins
#include <Servo.h>          // Library for controlling servo motors

const int RX_PIN = 2; // Connect HC-05 TX to Arduino pin 2
const int TX_PIN = 3; // Connect HC-05 RX to Arduino pin 3

SoftwareSerial bluetoothSerial(RX_PIN, TX_PIN); // RX, TX for HC-05 Bluetooth module

String readString; // String to store incoming commands from Bluetooth

Servo doorServo; // Create a servo object for door lock control

// Pin definitions for the devices
const int lightPin = 13; // Pin for controlling the light
const int lampPin = 11;  // Pin for controlling the table lamp
const int fanPin = 10;   // Pin for controlling the fan (relay)
const int servoPin = 9;  // Pin for controlling the servo motor

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging

  // Set device pins as outputs
  pinMode(lightPin, OUTPUT);
  pinMode(lampPin, OUTPUT);
  pinMode(fanPin, OUTPUT);

  doorServo.attach(servoPin); // Attach the servo to pin 9

  bluetoothSerial.begin(9600); // Initialize serial communication with HC-05 Bluetooth module
}

void loop() {
  // Read incoming data from Bluetooth
  while (bluetoothSerial.available()) {
    delay(3); // Small delay for better serial communication
    char c = bluetoothSerial.read();
    Serial.print(c); // Print each character for debugging
    readString += c; // Append each character to the readString
  }

  // Process the command if the string is not empty
  if (readString.length() > 0) {
    Serial.println(readString); // Print the complete command for debugging

    if (readString == "light on") {
      digitalWrite(lightPin, HIGH); // Turn on the light
      Serial.println("Lights are ON!");
    } else if (readString == "light off") {
      digitalWrite(lightPin, LOW); // Turn off the light
      Serial.println("Lights are OFF!");
    } else if (readString == "table lamp on") {
      digitalWrite(lampPin, HIGH); // Turn on the table lamp
      Serial.println("Table lamp is ON!");
    } else if (readString == "table lamp off") {
      digitalWrite(lampPin, LOW); // Turn off the table lamp
      Serial.println("Table lamp is OFF!");
    } else if (readString == "fan on") {
      digitalWrite(fanPin, HIGH); // Turn on the fan (relay activated)
      Serial.println("Fan is ON!");
    } else if (readString == "fan off") {
      digitalWrite(fanPin, LOW); // Turn off the fan (relay deactivated)
      Serial.println("Fan is OFF!");
    } else if (readString == "door open") {
      doorServo.write(90); // Open the door (servo to 90 degrees)
      Serial.println("Door is OPEN!");
    } else if (readString == "door close") {
      doorServo.write(0); // Close the door (servo to 0 degrees)
      Serial.println("Door is CLOSED!");
    } else if (readString == "all off") {
      // Turn off all devices
      digitalWrite(lightPin, LOW);
      digitalWrite(lampPin, LOW);
      digitalWrite(fanPin, LOW);
      doorServo.write(0); // Ensure door is closed
      Serial.println("All devices are OFF!");
    }

    readString = ""; // Clear the readString after processing
  }
}

/*
Functioning:
This program enables control of various household devices through an HC-05 Bluetooth module.
Commands sent via a Bluetooth terminal app on a mobile device can control the state of a light, a table lamp, a fan, and a door lock mechanism.
The code uses the SoftwareSerial library to handle Bluetooth communication and the Servo library to manage the servo motor for the door lock.
Each command is processed and executed, and the state of each device is printed to the Serial Monitor for debugging and verification purposes.
*/


//Old code
/*
#include <SoftwareSerial.h>

const int RX_PIN = 2; // Connect HC-05 TX to Arduino pin 2
const int TX_PIN = 3; // Connect HC-05 RX to Arduino pin 3

SoftwareSerial bluetoothSerial(RX_PIN, TX_PIN); // RX, TX

String readString;

void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT);       //Light Pin output
  pinMode(11,OUTPUT);       //Table lamp
  pinMode(10,OUTPUT);       //Relay for Fan
  pinMode(9, OUTPUT);
  bluetoothSerial.begin(9600);
}

void loop() {
  while (bluetoothSerial.available()) {
    delay(3);
    char c = bluetoothSerial.read();
    Serial.println(c);
    readString += c;
  }

  if (readString.length() > 0) {
    Serial.println(readString);
    if (readString == "light on") {
      digitalWrite(13, HIGH);
      Serial.println("Lignts are ON!");
    } else if (readString == "light off") {
      digitalWrite(13, LOW);
      Serial.println("Lignts are OFF!");
    } else if (readString == "Table lamp on") {
      digitalWrite(11, HIGH);
      Serial.println("Table lamp is ON!");
    } else if (readString == "Table lamp off") {
      digitalWrite(11, LOW);
      Serial.println("Table lamp is OFF!");
    } else if (readString == "door lock") {
      digitalWrite(10, HIGH);
      Serial.println("fan on");
    } else if (readString == "fan is on") {
      digitalWrite(10, LOW);
      Serial.println("fan off");
    } else if (readString == "fan is off"){
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(13, LOW);
    }
    readString = "";
  }
}
*/