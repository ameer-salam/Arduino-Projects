/*
Author: Ameer Salam
Date: 08th June 2024

Description:
-------------
This Arduino sketch is designed to test and read the sensor values from a water level sensor and a moisture sensor. The readings from these sensors are continuously printed to the serial monitor for monitoring and debugging purposes.

Hardware Setup:
---------------
- **Water Level Sensor**: Connected to analog pin A0.
- **Moisture Sensor**: Connected to analog pin A5.

Instructions:
-------------
1. Upload this code to your Arduino board.
2. Open the Serial Monitor from the Arduino IDE (Ctrl+Shift+M or Tools > Serial Monitor).
3. Ensure the baud rate in the Serial Monitor is set to 9600.
4. Observe the sensor readings printed every second.

Sensor Value Guidelines:
-------------------------
After uploading and running this code, record the sensor values for different conditions to calibrate your sensors. Below are the guidelines for typical sensor values:

1. **Moisture Sensor (A5)**:
   - **High Moisture**: When the sensor is in contact with moist soil.    High moisture value - 240 (sensor completely dipped in water)
   - **Low/Nill Moisture**: When the sensor is in dry soil or air.        Low moisture value - 1023 (sensor completely dry)

2. **Water Level Sensor (A0)**:
   - **Full Capacity**: When the water tank is full.                      Full water level value - 636 (water level sensor dipped in water)
   - **Zero Water Level**: When the water tank is empty.                  Low water level value - 230 (water level sensor completely dry)

Example Values:
---------------
- **Moisture Sensor**:
  - High Moisture Value: 240
  - Low/Nill Moisture Value: 1023

- **Water Level Sensor**:
  - Full Capacity Value: 636
  - Zero Water Level Value: 230

Code:
-----
*/

const int msTank = A0;  // Analog pin for tank water level sensor
const int msField = A5; // Analog pin for field moisture sensor

void setup() {
  // Initialize serial communication at 9600 baud rate
  Serial.begin(9600);

  // Print a startup message to the serial monitor
  Serial.println("Starting sensor readings...");
}

void loop() {
  // Read values from the sensors
  int tankValue = analogRead(msTank);
  int fieldValue = analogRead(msField);

  // Print sensor values to the serial monitor
  Serial.print("Tank Sensor Value: ");
  Serial.print(tankValue);
  Serial.print("\tField Sensor Value: ");
  Serial.println(fieldValue);

  // Wait 1 second before next reading
  delay(1000);
}
