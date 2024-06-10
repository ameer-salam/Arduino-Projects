/*
Author: Ameer Salam
Date: 08th June 2024

Description:
-------------
This Arduino sketch is designed to interface with a GSM module using the SoftwareSerial library. It allows you to send and receive SMS messages through the GSM module. The received messages are displayed on the Serial Monitor, and outgoing messages can be typed directly into the Serial Monitor.

Hardware Setup:
---------------
- **GSM Module**: Connect the GSM module's RX pin to Arduino pin 12 and TX pin to Arduino pin 13.
- **Power Supply**: Ensure that the GSM module is powered properly according to its specifications.

Instructions:
-------------
1. Connect the GSM module to the Arduino as per the hardware setup described above.
2. Upload this code to your Arduino board.
3. Open the Serial Monitor from the Arduino IDE (Ctrl+Shift+M or Tools > Serial Monitor).
4. Set the baud rate in the Serial Monitor to 9600.
5. Type your SMS message into the Serial Monitor and press enter to send it.
6. The Arduino will display any incoming SMS messages in the Serial Monitor.

Functions:
----------
1. **Sending SMS**: 
   - Type a message in the Serial Monitor and press enter to send the message to the predefined phone number.

2. **Receiving SMS**: 
   - When a new SMS is received, it will be displayed on the Serial Monitor immediately.

Code:
-----
*/

#include <SoftwareSerial.h>

SoftwareSerial gsm(12, 13); // RX, TX

void setup() {
  Serial.begin(9600); // Serial monitor communication
  gsm.begin(9600);    // GSM module communication

  Serial.println("GSM Module ready.");
  Serial.println("Type a message and press enter:");

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
    String message = Serial.readStringUntil('\n'); // Read message from serial monitor
    sendSMS(message);
  }

  if (gsm.available() > 0) {
    String incomingMessage = gsm.readString();
    if (incomingMessage.indexOf("+CMT:") != -1) {
      // Print the incoming message
      Serial.print("Incoming message: ");
      Serial.println(incomingMessage);
    }
  }
}

void sendSMS(String message) {
  String phoneNumber = "+1234567890"; // Replace with the target phone number

  gsm.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);

  gsm.print("AT+CMGS=\"");
  gsm.print(phoneNumber);
  gsm.println("\"");
  delay(1000);

  gsm.print(message);
  delay(1000);

  gsm.write(26); // ASCII code for CTRL+Z to send SMS
  delay(1000);

  Serial.println("Message sent.");
}
