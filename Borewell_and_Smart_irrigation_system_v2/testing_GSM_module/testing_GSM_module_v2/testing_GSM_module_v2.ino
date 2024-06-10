#include <SoftwareSerial.h>

SoftwareSerial gsm(12, 13); // RX, TX

void setup() {
  Serial.begin(9600); // Serial monitor communication
  gsm.begin(9600);    // GSM module communication

  Serial.println("GSM Module ready.");

  // Initialize the GSM module
  gsm.println("AT");
  delay(1000);
  gsm.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
  
  gsm.println("AT+CNMI=2,2,0,0,0"); // Configure to notify when an SMS is received
  delay(1000);
}

void loop() {
  if (gsm.available() > 0) {
    String incomingMessage = gsm.readString();
    Serial.println("Received SMS: " + incomingMessage);
  }
}
