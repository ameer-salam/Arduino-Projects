/*
Author: Ameer Salam
Date: 08th June 2024

Description:
-------------
This Arduino sketch is designed to test and read the sensor values from a water level sensor and a moisture sensor. 
The readings from these sensors are continuously printed to the serial monitor for monitoring and debugging purposes.

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

How to Read Sensor Values and Calculate the Threshold Value:
1. Keep the sensors dry and note down the value of the sensor when placed in a dry environment.
2. Now, place the sensor in a damp/moist environment (or better immerse the entire sensor into a glass filled with water) and note down the readings.
3. To calculate the threshold:
    1. **Moisture Sensor (A5)**:
      - **High Moisture**: When the sensor is in contact with moist soil.    
        High moisture value - 240 (sensor completely dipped in water)
      - **Low/Nil Moisture**: When the sensor is in dry soil or air.        
        Low moisture value - 1023 (sensor completely dry)

    2. **Water Level Sensor (A0)**:
      - **Full Capacity**: When the water tank is full.                      
        Full water level value - 636 (water level sensor dipped in water)
      - **Zero Water Level**: When the water tank is empty.                  
        Low water level value - 230 (water level sensor completely dry)

Example Values:
---------------
- **Moisture Sensor**:
  - High Moisture Value: 240
  - Low/Nil Moisture Value: 1023

- **Water Level Sensor**:
  - Full Capacity Value: 636
  - Zero Water Level Value: 230

Threshold Calculation:
----------------------
1. **High Value Indicates Low Moisture (Moisture Sensor)**:
   Formula:
   threshold_value = highest_value - (desired_percentage / 100.0) * (highest_value - lowest_value)
   Example: 
   threshold_value = 1023 - (50 / 100.0) * (1023 - 240) 

2. **High Value Indicates High Water Level (Water Level Sensor)**:
   Formula:
   threshold_value = lowest_value + (desired_percentage / 100.0) * (highest_value - lowest_value)
   Example:
   threshold_value = 230 + (50 / 100.0) * (636 - 230)

Code:
-----
*/

const int msTank = A0;  // Analog pin for tank water level sensor
const int msField = A5; // Analog pin for field moisture sensor

// Define the desired moisture and water level percentages
const float desiredMoisturePercentage = 50.0; // 50% threshold for moisture
const float desiredWaterLevelPercentage = 50.0; // 50% threshold for water level

// Define sensor value ranges
const int moistureLow = 1023;   // Dry soil
const int moistureHigh = 240;   // Wet soil
const int waterLevelLow = 230;  // Empty tank
const int waterLevelHigh = 636; // Full tank

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

  // Calculate threshold values
  float moistureThreshold = calculateThresholdLowMoisture(moistureLow, moistureHigh, desiredMoisturePercentage);
  float waterLevelThreshold = calculateThresholdHighMoisture(waterLevelLow, waterLevelHigh, desiredWaterLevelPercentage);

  // Print sensor values and thresholds to the serial monitor
  Serial.print("Tank Sensor Value: ");
  Serial.print(tankValue);
  Serial.print(" (Threshold: ");
  Serial.print(waterLevelThreshold);
  Serial.print(")\tField Sensor Value: ");
  Serial.print(fieldValue);
  Serial.print(" (Threshold: ");
  Serial.print(moistureThreshold);
  Serial.println(")");

  // Wait 1 second before next reading
  delay(1000);
}

/*
 * Function to calculate the threshold value when a high sensor value indicates low moisture.
 * threshold_value = highest_value - (desired_percentage / 100.0) * (highest_value - lowest_value)
 */
float calculateThresholdLowMoisture(int lowest_value, int highest_value, float desired_percentage) {
  return highest_value - (desired_percentage / 100.0) * (highest_value - lowest_value);
}

/*
 * Function to calculate the threshold value when a high sensor value indicates high water level.
 * threshold_value = lowest_value + (desired_percentage / 100.0) * (highest_value - lowest_value)
 */
float calculateThresholdHighMoisture(int lowest_value, int highest_value, float desired_percentage) {
  return lowest_value + (desired_percentage / 100.0) * (highest_value - lowest_value);
}
