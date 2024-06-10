#include <SoftwareSerial.h>

SoftwareSerial gsm(12, 13); // RX, TX for GSM communication

const int msTank = A0; // Analog pin for tank water level sensor
const int msField = A5; // Analog pin for field moisture sensor
const int motor1 = 6; // Pin for motor 1 (Field Pump)
const int motor2 = 7; // Pin for motor 2 (Tank Pump)

bool activeHighRelay = false; // Change to true if using active high relays
bool automaticMode = true; // Mode flag, start in automatic mode

String phoneNumber = "+919353165243"; // Placeholder phone number for outgoing SMS
String lastCaller = ""; // Store the last caller number for SMS response

// Timing variables
unsigned long previousCheckTime = 0; // Store the last check time for automatic mode
const unsigned long checkInterval = 2000; // 10 seconds delay

unsigned long previousGSMTime = 0; // Store the last time GSM command was sent
const unsigned long gsmInterval = 1000; // 1 second delay between GSM commands

// SMS data variables
String smsStatus, senderNumber, receivedDate, msg;
boolean isReply = false;

void setup() {
  Serial.begin(9600); // Serial monitor communication
  gsm.begin(9600); // GSM module communication

  Serial.println("GSM Module ready.");
  Serial.println("Type a command or send an SMS:");

  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);

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
    digitalWrite(motor1, activeHighRelay ? HIGH : LOW);
    Serial.println("Motor 1 turned ON");
    sendSMS(sender, "Motor 1 turned ON");
  } else if (command == "m1 off") {
    automaticMode = false;
    digitalWrite(motor1, activeHighRelay ? LOW : HIGH);
    Serial.println("Motor 1 turned OFF");
    sendSMS(sender, "Motor 1 turned OFF");
  } else if (command == "m2 on") {
    automaticMode = false;
    digitalWrite(motor2, activeHighRelay ? HIGH : LOW);
    Serial.println("Motor 2 turned ON");
    sendSMS(sender, "Motor 2 turned ON");
  } else if (command == "m2 off") {
    automaticMode = false;
    digitalWrite(motor2, activeHighRelay ? LOW : HIGH);
    Serial.println("Motor 2 turned OFF");
    sendSMS(sender, "Motor 2 turned OFF");
  } else if (command == "m off") {
    automaticMode = false;
    digitalWrite(motor1, activeHighRelay ? LOW : HIGH);
    digitalWrite(motor2, activeHighRelay ? LOW : HIGH);
    Serial.println("All motors turned OFF");
    sendSMS(sender, "All motors turned OFF");
  } else if (command == "status") {
    String status = getStatus();
    Serial.println("Sending status SMS...");
    sendSMS(sender, status);
    printStatusToSerial(status);
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

  String motor1Status = digitalRead(motor1) == (activeHighRelay ? HIGH : LOW) ? "ON" : "OFF";
  String motor2Status = digitalRead(motor2) == (activeHighRelay ? HIGH : LOW) ? "ON" : "OFF";
  String modeStatus = automaticMode ? "auto" : "man";

  String status = "Status:\nMoisture sensor: " + String(fieldValue) + 
                  "\nWater level sensor: " + String(tankValue) + 
                  "\nMotor status: M1 " + motor1Status + 
                  ", M2 " + motor2Status +
                  "\nMode: " + modeStatus;
  return status;
}

// Function to send an SMS message
void sendSMS(String toNumber, String message) {
  sendGSMCommand("AT+CMGF=1"); // Set SMS mode to text

  gsm.print("AT+CMGS=\"");
  gsm.print(toNumber);
  gsm.println("\"");
  waitForGSMResponse();

  gsm.print(message);
  waitForGSMResponse();

  gsm.write(26); // ASCII code for CTRL+Z to send SMS
  waitForGSMResponse();

  Serial.println("Message sent.");
}

// Function to print status to the serial monitor
void printStatusToSerial(String status) {
  // Convert the status message to the serial output format
  String formattedStatus = status;
  formattedStatus.replace("\n", "\t");
  formattedStatus.replace("Motor status:", "\tMotor status:");
  Serial.println(formattedStatus);
}

// Function to check sensors and control motors in automatic mode
void checkSensorsAndControlMotors() {
  int tankValue = analogRead(msTank);
  int fieldValue = analogRead(msField);

  // Define moisture and tank level thresholds
  int moistureThreshold = 550; // Moisture level threshold for field //value<1000 is no moisture
  int tankLevelThreshold = `; // Water level threshold for tank //value>700 is full tank value<500 is low water

  // Automatic control logic
  if (fieldValue > moistureThreshold) {
    // Moisture is low, pump water from tank to field
    digitalWrite(motor2, activeHighRelay ? HIGH : LOW);
    Serial.println("Automatic: Field Pump (Motor 2) turned ON (Field moisture low)");
  } else {
    // Moisture is sufficient, turn off the pump
    digitalWrite(motor2, activeHighRelay ? LOW : HIGH);
    Serial.println("Automatic: Field Pump (Motor 2) turned OFF (Field moisture sufficient)");
  }

  if (tankValue < tankLevelThreshold) {
    // Tank level is low, pump water from borewell to tank
    digitalWrite(motor2, activeHighRelay ? HIGH : LOW);
    Serial.println("Automatic: Tank Pump (Motor 1) turned ON (Tank level low)");
  } else {
    // Tank level is sufficient, turn off the pump
    digitalWrite(motor2, activeHighRelay ? LOW : HIGH);
    Serial.println("Automatic: Tank Pump (Motor 1) turned OFF (Tank level sufficient)");
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

  unsigned int len, index;
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
      msg = buff;
      msg.toLowerCase();
      Serial.println("Extracted SMS: " + msg);

      executeCommand(msg, senderNumber);
    }
  }
}
