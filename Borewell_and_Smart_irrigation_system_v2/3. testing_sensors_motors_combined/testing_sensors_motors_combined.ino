/*
## Author

- **Name**: Ameer Salam
- **Email**: ameersuz3@gmail.com
- **GitHub**: [github.com/ameer-salam](https://github.com/ameer-salam)
- **LinkedIn**: [linkedin.com/in/ameersalam](https://www.linkedin.com/in/ameersalam/)
- **Twitter**: [@ameersalam](https://twitter.com/ameer_salam3)
- Date: 08th June 2024

Description:
-------------
This Arduino sketch integrates sensor reading and motor control. It reads values from a water level sensor and a moisture sensor, and controls two motors via relays. The relay type can be configured using the `relayType` variable. Commands are received through the serial monitor to control the motors and to request the status of sensors and motors.

Relay Type:
-----------
- 0: Active High Relay
- 1: Active Low Relay

Hardware Setup:
---------------
- **Borewell Motor Relay** (m1): Connected to digital pin 6.
- **Tank Motor Relay** (m2): Connected to digital pin 7.
- **Water Level Sensor**: Connected to analog pin A0.
- **Moisture Sensor**: Connected to analog pin A5.

Instructions:
-------------
1. Set the `relayType` variable to `0` for active high relays or `1` for active low relays.
2. Upload this code to your Arduino board.
3. Open the Serial Monitor from the Arduino IDE (Ctrl+Shift+M or Tools > Serial Monitor).
4. Ensure the baud rate in the Serial Monitor is set to 9600.
5. Type commands to control the motors or check the status:
   - "status": Get current sensor readings and motor statuses.
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
const int msTank = A0;  // Analog pin for tank water level sensor
const int msField = A5; // Analog pin for field moisture sensor

// Store motor statuses
bool borewellMotorStatus = false; // false = OFF, true = ON
bool tankMotorStatus = false;     // false = OFF, true = ON

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
  Serial.println("Sensor and motor control started. Type 'status', 'm on', 'm1 on', 'm1 off', 'm2 on', 'm2 off'.");
}

void loop() {
  // Check if data is available in the serial buffer
  if (Serial.available() > 0) {
    // Read the incoming string
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove any trailing whitespace or newlines

    // Handle commands
    if (command == "status") {
      printStatus();
    } else if (command == "m on") {
      controlMotor(borewellMotorRelayPin, true);
      controlMotor(tankMotorRelayPin, true);
      borewellMotorStatus = true;
      tankMotorStatus = true;
      printStatus();
    } else if (command == "m1 on") {
      controlMotor(borewellMotorRelayPin, true);
      borewellMotorStatus = true;
      printStatus();
    } else if (command == "m1 off") {
      controlMotor(borewellMotorRelayPin, false);
      borewellMotorStatus = false;
      printStatus();
    } else if (command == "m2 on") {
      controlMotor(tankMotorRelayPin, true);
      tankMotorStatus = true;
      printStatus();
    } else if (command == "m2 off") {
      controlMotor(tankMotorRelayPin, false);
      tankMotorStatus = false;
      printStatus();
    } else {
      Serial.println("Unknown command. Use 'status', 'm on', 'm1 on', 'm1 off', 'm2 on', 'm2 off'.");
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

// Function to print current status
void printStatus() {
  // Read values from the sensors
  int tankValue = analogRead(msTank);
  int fieldValue = analogRead(msField);
  
  // Print sensor values and motor statuses to the serial monitor
  Serial.print("Moisture level: ");
  Serial.print(fieldValue);
  Serial.print("\tWater level: ");
  Serial.print(tankValue);
  Serial.print("\tMotors status: m1 ");
  Serial.print(borewellMotorStatus ? "ON" : "OFF");
  Serial.print(", m2 ");
  Serial.println(tankMotorStatus ? "ON" : "OFF");
}
