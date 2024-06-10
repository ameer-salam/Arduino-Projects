#include <SoftwareSerial.h>

SoftwareSerial gsm(12, 13); // RX, TX

const int msTank = A0;    // Analog pin for tank water level sensor
const int msField = A5;   // Analog pin for field moisture sensor
const int motor1 = 6;     // Pin for motor 1
const int motor2 = 7;     // Pin for motor 2

bool activeHighRelay = false; // Change to true if using active high relays

void setup() {
  Serial.begin(9600); // Serial monitor communication
  gsm.begin(9600);    // GSM module communication

  Serial.println("GSM Module ready.");
  Serial.println("Type a command or send an SMS:");

  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);

  // Initialize the GSM module
  gsm.println("AT");
  delay(1000);
  gsm.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
  gsm.println("AT+CNMI=2,2,0,0,0"); // Configure to notify when an SMS is received
  delay(1000);
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    executeCommand(command);
  }

  if (gsm.available() > 0) {
    String incomingMessage = gsm.readString();
    Serial.println("Raw GSM response: " + incomingMessage); // Debug statement

    if (incomingMessage.startsWith("+CMTI:")) {
      // Extract the index from the notification
      int indexStart = incomingMessage.indexOf(',') + 1;
      String indexStr = incomingMessage.substring(indexStart);
      indexStr.trim(); // Trim whitespace
      Serial.println("Extracted index: " + indexStr); // Debug statement

      // Read the full message at the given index
      gsm.print("AT+CMGR=");
      gsm.println(indexStr);
      delay(1000);

      // Read the full message response
      String messageResponse = gsm.readString();
      Serial.println("Full message response: " + messageResponse); // Debug statement

      // Find the start of the SMS content
      int metadataEnd = messageResponse.indexOf('\n');
      if (metadataEnd != -1) {
        int contentStart = messageResponse.indexOf('\n', metadataEnd + 1);
        if (contentStart != -1) {
          String smsContent = messageResponse.substring(contentStart + 1);
          smsContent.trim(); // Trim whitespace from the extracted content
          Serial.println("Extracted SMS content: " + smsContent); // Debug statement

          // Execute the command
          executeCommand(smsContent);
        }
      }
    }
  }
}

void executeCommand(String command) {
  command.trim(); // Remove leading and trailing whitespace
  Serial.println("Received command: " + command);

  if (command.equals("status")) {
    String status = getStatus();
    sendSMS(status);
  } else if (command.equals("m1 on")) {
    digitalWrite(motor1, activeHighRelay ? HIGH : LOW);
    sendSMS("Motor 1 turned ON");
  } else if (command.equals("m1 off")) {
    digitalWrite(motor1, activeHighRelay ? LOW : HIGH);
    sendSMS("Motor 1 turned OFF");
  } else if (command.equals("m2 on")) {
    digitalWrite(motor2, activeHighRelay ? HIGH : LOW);
    sendSMS("Motor 2 turned ON");
  } else if (command.equals("m2 off")) {
    digitalWrite(motor2, activeHighRelay ? LOW : HIGH);
    sendSMS("Motor 2 turned OFF");
  } else {
    sendSMS("Unknown command.");
  }
}

String getStatus() {
  int tankValue = analogRead(msTank);
  int fieldValue = analogRead(msField);

  String motor1Status = digitalRead(motor1) == (activeHighRelay ? HIGH : LOW) ? "ON" : "OFF";
  String motor2Status = digitalRead(motor2) == (activeHighRelay ? HIGH : LOW) ? "ON" : "OFF";

  String status = "Moisture level: " + String(fieldValue) + "\nWater level: " + String(tankValue) + "\nMotors status: M1: " + motor1Status + ", M2: " + motor2Status;
  return status;
}

void sendSMS(String message) {
  gsm.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);

  gsm.print("AT+CMGS=\"");
  gsm.print("+919353165243"); // Replace with the target phone number
  gsm.println("\"");
  delay(1000);

  gsm.print(message);
  delay(1000);

  gsm.write(26); // ASCII code for CTRL+Z to send SMS
  delay(1000);

  Serial.println("Message sent.");
}
