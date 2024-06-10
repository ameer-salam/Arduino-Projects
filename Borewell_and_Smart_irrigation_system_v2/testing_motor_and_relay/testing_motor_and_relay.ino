/*
Author: Ameer Salam
Date: 08th June 2024

Description:
-------------
This Arduino sketch controls two motors using relays, which can be either active high or active low. The relay type can be configured using the `relayType` variable. Commands are received through the serial monitor to turn the motors on or off.

Relay Type:
-----------
- 0: Active High Relay
- 1: Active Low Relay

Hardware Setup:
---------------
- **Borewell Motor Relay** (m1): Connected to digital pin 6.
- **Tank Motor Relay** (m2): Connected to digital pin 7.

Instructions:
-------------
1. Set the `relayType` variable to `0` for active high relays or `1` for active low relays.
2. Upload this code to your Arduino board.
3. Open the Serial Monitor from the Arduino IDE (Ctrl+Shift+M or Tools > Serial Monitor).
4. Ensure the baud rate in the Serial Monitor is set to 9600.
5. Type commands to control the motors:
   - "m on": Turn both motors on.
   - "m1 on": Turn borewell motor on.
   - "m1 off": Turn borewell motor off.
   - "m2 on": Turn tank motor on.
   - "m2 off": Turn tank motor off.

Code:
-----
*/

// Define relay type: 0 for active high relay, 1 for active low relay
const int relayType = 1; // Change this to 0 if using active high relays

// Arduino pin assignments
const int borewellMotorRelayPin = 6; // Digital pin for borewell motor relay (m1)
const int tankMotorRelayPin = 7;     // Digital pin for tank motor relay (m2)

void setup() {
  // Initialize serial communication at 9600 baud rate
  Serial.begin(9600);
  
  // Set relay pins as outputs
  pinMode(borewellMotorRelayPin, OUTPUT);
  pinMode(tankMotorRelayPin, OUTPUT);
  
  // Set initial state of relays (OFF state)
  if (relayType == 0) { // Active High relay
    digitalWrite(borewellMotorRelayPin, LOW); 
    digitalWrite(tankMotorRelayPin, LOW);
  } else { // Active Low relay
    digitalWrite(borewellMotorRelayPin, HIGH); 
    digitalWrite(tankMotorRelayPin, HIGH);
  }
  
  // Print a startup message to the serial monitor
  Serial.println("Motor control test started. Type 'm on', 'm1 on', 'm1 off', 'm2 on', 'm2 off'.");
}

void loop() {
  // Check if data is available in the serial buffer
  if (Serial.available() > 0) {
    // Read the incoming string
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove any trailing whitespace or newlines

    // Handle commands
    if (command == "m on") {
      controlMotor(borewellMotorRelayPin, true);
      controlMotor(tankMotorRelayPin, true);
      Serial.println("Both motors are ON");
    } else if (command == "m1 on") {
      controlMotor(borewellMotorRelayPin, true);
      Serial.println("Borewell motor (m1) is ON");
    } else if (command == "m1 off") {
      controlMotor(borewellMotorRelayPin, false);
      Serial.println("Borewell motor (m1) is OFF");
    } else if (command == "m2 on") {
      controlMotor(tankMotorRelayPin, true);
      Serial.println("Tank motor (m2) is ON");
    } else if (command == "m2 off") {
      controlMotor(tankMotorRelayPin, false);
      Serial.println("Tank motor (m2) is OFF");
    } else {
      Serial.println("Unknown command. Use 'm on', 'm1 on', 'm1 off', 'm2 on', 'm2 off'.");
    }
  }
}

// Function to control motors based on relay type
void controlMotor(int pin, bool turnOn) {
  if (relayType == 0) { // Active High relay
    digitalWrite(pin, turnOn ? HIGH : LOW);
  } else { // Active Low relay
    digitalWrite(pin, turnOn ? LOW : HIGH);
  }
}
