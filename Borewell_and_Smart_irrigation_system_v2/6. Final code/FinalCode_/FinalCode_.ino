/*
Author

- **Name**: Ameer Salam
- **Email**: ameersuz3@gmail.com
- **GitHub**: [github.com/ameer-salam](https://github.com/ameer-salam)
- **LinkedIn**: [linkedin.com/in/ameersalam](https://www.linkedin.com/in/ameersalam/)
- **Twitter**: [@ameersalam](https://twitter.com/ameer_salam3)
- Date: 08th June 2024

*/

#include <SoftwareSerial.h>

SoftwareSerial gsm(12, 13); // RX, TX for GSM communication

const int msTank = A0; // Analog pin for tank water level sensor
const int msField = A5; // Analog pin for field moisture sensor
const int borewellMotor = 6; // Pin for motor 1 (Borewell Pump)
const int fieldMotor = 7; // Pin for motor 2 (Field Pump)

bool activeHighRelay = false; // Change to true if using active high relays
bool automaticMode = true; // Mode flag, start in automatic mode

String phoneNumber = "+91"; // Placeholder phone number for outgoing SMS
String lastCaller = ""; // Store the last caller number for SMS response
String senderNumber = ""; // Store the sender's number from incoming SMS

// Timing variables
unsigned long previousCheckTime = 0; // Store the last check time for automatic mode
const unsigned long checkInterval = 2000; // 2 seconds delay

unsigned long previousGSMTime = 0; // Store the last time GSM command was sent
const unsigned long gsmInterval = 1000; // 1 second delay between GSM commands

void setup() {
  Serial.begin(9600); // Serial monitor communication
  gsm.begin(9600); // GSM module communication

  Serial.println("GSM Module ready.");
  Serial.println("Type a command or send an SMS:");

  pinMode(borewellMotor, OUTPUT);
  pinMode(fieldMotor, OUTPUT);

  // Initialize the GSM module
  sendGSMCommand("AT"); // Check GSM module
  sendGSMCommand("AT+CMGF=1"); // Set SMS mode to text
  sendGSMCommand("AT+CNMI=2,2,0,0,0"); // Configure to send SMS directly to the serial port
  sendGSMCommand("AT+CLIP=1"); // Enable caller ID

  Serial.println("System started in automatic mode.");
}

void loop() {
  // Check for serial commands
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    Serial.println("Command received: " + command);
    executeCommand(command, phoneNumber);
  }

  // Check for GSM incoming data
  if (gsm.available() > 0) {
    String incomingData = gsm.readString();
    handleIncomingData(incomingData);
  }

  // Handle automatic mode
  if (automaticMode) {
    unsigned long currentTime = millis();
    if (currentTime - previousCheckTime >= checkInterval) { // Non-blocking delay
      previousCheckTime = currentTime;
      checkSensorsAndControlMotors();
    }
  }
}

void executeCommand(String command, String sender) {
  if (command == "m1 on") {
    automaticMode = false;
    digitalWrite(borewellMotor, activeHighRelay ? HIGH : LOW);
    Serial.println("Borewell Pump turned ON");
    sendSMS(sender, "Borewell Pump turned ON");
  } else if (command == "m1 off") {
    automaticMode = false;
    digitalWrite(borewellMotor, activeHighRelay ? LOW : HIGH);
    Serial.println("Borewell Pump turned OFF");
    sendSMS(sender, "Borewell Pump turned OFF");
  } else if (command == "m2 on") {
    automaticMode = false;
    digitalWrite(fieldMotor, activeHighRelay ? HIGH : LOW);
    Serial.println("Field Pump turned ON");
    sendSMS(sender, "Field Pump turned ON");
  } else if (command == "m2 off") {
    automaticMode = false;
    digitalWrite(fieldMotor, activeHighRelay ? LOW : HIGH);
    Serial.println("Field Pump turned OFF");
    sendSMS(sender, "Field Pump turned OFF");
  } else if (command == "m off") {
    automaticMode = false;
    digitalWrite(borewellMotor, activeHighRelay ? LOW : HIGH);
    digitalWrite(fieldMotor, activeHighRelay ? LOW : HIGH);
    Serial.println("All pumps turned OFF");
    sendSMS(sender, "All pumps turned OFF");
  } else if (command == "status") {
    String status = getStatus();
    Serial.println("Sending status SMS...");
    sendSMS(sender, status);
    Serial.println("Status sent.");
  } else if (command == "auto") {
    automaticMode = true;
    Serial.println("Switched to automatic mode");
    sendSMS(sender, "Switched to automatic mode");
  } else {
    Serial.println("Unknown command.");
    sendSMS(sender, "Unknown command.");
  }
}

// Function to get the current status of the system
String getStatus() {
  int tankValue = analogRead(msTank);
  int fieldValue = analogRead(msField);

  String borewellPumpStatus = digitalRead(borewellMotor) == (activeHighRelay ? HIGH : LOW) ? "ON" : "OFF";
  String fieldPumpStatus = digitalRead(fieldMotor) == (activeHighRelay ? HIGH : LOW) ? "ON" : "OFF";
  String modeStatus = automaticMode ? "auto" : "man";

  String status = "Status:\nField Moisture Sensor: " + String(fieldValue) + 
                  "\nTank Water Level Sensor: " + String(tankValue) + 
                  "\nBorewell Pump status: " + borewellPumpStatus + 
                  "\nField Pump status: " + fieldPumpStatus +
                  "\nMode: " + modeStatus;
  return status;
}

// Function to send an SMS message
void sendSMS(String toNumber, String message) {
  sendGSMCommand("AT+CMGF=1"); // Set SMS mode to text

  gsm.print("AT+CMGS=\"");
  gsm.print(toNumber);
  gsm.println("\"");
  delay(100); // Allow the GSM module time to process

  gsm.print(message);
  delay(100); // Short delay to ensure complete message transmission

  gsm.write(26); // ASCII code for CTRL+Z to send SMS
  delay(100); // Wait for the GSM module to send the message

  Serial.println("Message sent: " + message);
}

// Function to check sensors and control pumps in automatic mode
void checkSensorsAndControlMotors() {
  int tankValue = analogRead(msTank);
  int fieldValue = analogRead(msField);

  // Define moisture and tank level thresholds
  int moistureThreshold = 550; // Moisture level threshold for field
  int tankLevelThreshold = 600; // Water level threshold for tank

  // Automatic control logic
  if (fieldValue > moistureThreshold) {
    // Moisture is low, pump water from tank to field
    digitalWrite(fieldMotor, activeHighRelay ? HIGH : LOW);
    Serial.println("Automatic: Field Pump turned ON (Field moisture low)");
  } else {
    // Moisture is sufficient, turn off the pump
    digitalWrite(fieldMotor, activeHighRelay ? LOW : HIGH);
    Serial.println("Automatic: Field Pump turned OFF (Field moisture sufficient)");
  }

  if (tankValue < tankLevelThreshold) {
    // Tank level is low, pump water from borewell to tank
    digitalWrite(borewellMotor, activeHighRelay ? HIGH : LOW);
    Serial.println("Automatic: Borewell Pump turned ON (Tank level low)");
  } else {
    // Tank level is sufficient, turn off the pump
    digitalWrite(borewellMotor, activeHighRelay ? LOW : HIGH);
    Serial.println("Automatic: Borewell Pump turned OFF (Tank level sufficient)");
  }
}

// Function to send a command to the GSM module and wait for a response
void sendGSMCommand(String command) {
  gsm.println(command);
  waitForGSMResponse();
}

// Function to handle incoming GSM data
void handleIncomingData(String data) {
  Serial.println("Incoming GSM data: " + data);

  parseData(data);
}

// Function to wait for GSM response
void waitForGSMResponse() {
  long timeout = millis() + 5000; // 5 seconds timeout
  while (!gsm.available() && millis() < timeout) {
    delay(100); // Small delay to wait for response
  }
  if (gsm.available()) {
    String response = gsm.readString();
    Serial.println("GSM Response: " + response);
  } else {
    Serial.println("GSM Response timeout.");
  }
}

// Function to extract and handle SMS data
void parseData(String buff) {
  Serial.println(buff);

  unsigned int index;
  // Remove sent "AT Command" from the response string.
  index = buff.indexOf("\r");
  buff.remove(0, index + 2);
  buff.trim();

  if (buff != "OK") {
    index = buff.indexOf(":");
    String cmd = buff.substring(0, index);
    cmd.trim();

    buff.remove(0, index + 2);

    if (cmd == "+CMT") {
      // New SMS received
      index = buff.indexOf(",");
      senderNumber = buff.substring(1, index - 1);
      Serial.println("Sender Number: " + senderNumber);

      index = buff.indexOf("\n");
      buff.remove(0, index + 1);
      buff.trim();
      String msg = buff;
      msg.toLowerCase();
      Serial.println("Extracted SMS: " + msg);

      executeCommand(msg, senderNumber);
    }
  }
}
