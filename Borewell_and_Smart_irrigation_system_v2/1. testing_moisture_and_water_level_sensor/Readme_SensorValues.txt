## Author

- **Name**: Ameer Salam
- **Email**: ameersuz3@gmail.com
- **GitHub**: [github.com/ameer-salam](https://github.com/ameer-salam)
- **LinkedIn**: [linkedin.com/in/ameersalam](https://www.linkedin.com/in/ameersalam/)
- **Twitter**: [@ameersalam](https://twitter.com/ameer_salam3)

## This Folder contains two programs :

	1.testing_moisture_and_water_level_sensor 	[I strongly recommend you to use this code]
	2.testing_moisture_and_water_level_sensor_v2 	[V2(this code) is not well tested]

# The Program - 
## 1.testing_moisture_and_water_level_sensor

Performs the following function
1. Measures the sensor readings,
2. Prints the values on the screen
3. note down the highest and the lowest values of the sensor
4. manually calculate the threshold based on the percentage you want
example: 
* 1. High Sensor Value Indicates High Moisture
 *
 *    Scenario:
 *    - Higher sensor values mean higher moisture levels.
 *    - Example: A sensor value of 1000 indicates the sensor is fully submerged in water, and a 
 *               value of 0 indicates completely dry soil.
 *
 *    Formula:
 *    threshold_value = lowest_value + (desired_percentage / 100.0) * (highest_value - lowest_value)
 *
 *    Explanation:
 *    - lowest_value: The sensor reading in completely dry soil (e.g., 0).
 *    - highest_value: The sensor reading when fully submerged in water (e.g., 1000).
 *    - desired_percentage: The moisture level at which you want to activate the motor (e.g., 50%).
 *
 *    Example Calculation:
 *    For a 50% threshold:
 *    threshold_value = 0 + (50 / 100.0) * (1000 - 0) = 500
 *
 * 2. High Sensor Value Indicates Low Moisture
 *
 *    Scenario:
 *    - Higher sensor values mean lower moisture levels.
 *    - Example: A sensor value of 1000 indicates completely dry soil, and a value of 0 indicates 
 *               fully saturated soil.
 *
 *    Formula:
 *    threshold_value = highest_value - (desired_percentage / 100.0) * (highest_value - lowest_value)
 *
 *    Explanation:
 *    - highest_value: The sensor reading in completely dry soil (e.g., 1000).
 *    - lowest_value: The sensor reading when fully submerged in water (e.g., 0).
 *    - desired_percentage: The moisture level at which you want to activate the motor (e.g., 50%).
 *
 *    Example Calculation:
 *    For a 50% threshold:
 *    threshold_value = 1000 - (50 / 100.0) * (1000 - 0) = 500
 

# The Program - 
## 2.testing_moisture_and_water_level_sensor_v2

Performs the following function
1. Measures the sensor readings,
2. Calculates the threshold values and Prints the values on the screen (logic defined above and in the code)
3. this program is not well tested by the developer!


# Circuit Diagram
The circuit diagram is given in the file