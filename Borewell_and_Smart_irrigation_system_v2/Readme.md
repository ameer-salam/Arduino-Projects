# GSM Based Smart Irrigation System

- **Name**: Ameer Salam
- **Email**: ameersuz3@gmail.com
- **GitHub**: [github.com/ameer-salam](https://github.com/ameer-salam)
- **LinkedIn**: [linkedin.com/in/ameersalam](https://www.linkedin.com/in/ameersalam/)
- **Twitter**: [@ameersalam](https://twitter.com/ameer_salam3)

## Overview

The GSM based smart irrigation system is designed to automate the process of watering fields based on soil moisture levels and tank water levels. It utilizes GSM communication for remote control and monitoring, allowing users to manage irrigation pumps via SMS commands. This system aims to optimize water usage and improve crop yield by ensuring timely watering as per the field's moisture requirements.

## How It Works

The system consists of:
- **GSM Module**: Facilitates communication with the irrigation system via SMS.
- **Sensors**: Includes a moisture sensor for measuring field moisture and a water level sensor for the tank.
- **Relays and Motors**: Controls the water pumps (borewell and field pumps) based on sensor readings.
- **Arduino Board**: Acts as the central controller to process sensor data, execute commands, and communicate with the GSM module.

### Modes of Operation

- **Automatic Mode**: In this mode, the system autonomously monitors the moisture level of the field and the water level in the tank. It automatically turns on the pumps as needed to irrigate the field when moisture levels are low or refill the tank when water levels are low.
  
- **Manual Mode**: Allows manual control of the pumps via SMS or Serial Monitor commands. This mode disables automatic operation, and pumps are controlled solely based on user commands.


## Testing Procedures

### 1. [Testing Moisture and Water Level Sensor](https://github.com/ameer-salam/Arduino-Projects/tree/main/Borewell_and_Smart_irrigation_system_v2/1.%20testing_moisture_and_water_level_sensor)

To test the moisture and water level sensors:
- Ensure the Arduino is connected to the sensors and powered on.
- Open the Serial Monitor to observe sensor readings.
- Place the moisture sensor in soil and observe readings corresponding to soil moisture.
- Check the water level sensor to verify readings corresponding to the tank's water level.

### 2. [Testing Motor and Relay](https://github.com/ameer-salam/Arduino-Projects/tree/main/Borewell_and_Smart_irrigation_system_v2/2.%20testing_motor_and_relay)

To test the motor and relay:
- Connect the Arduino to the relays controlling the pumps.
- Send commands via Serial Monitor or SMS to turn on/off the motors.
- Verify the pumps respond correctly to commands by observing their operation.

### 3. [Testing Sensors and Motors Combined](https://github.com/ameer-salam/Arduino-Projects/tree/main/Borewell_and_Smart_irrigation_system_v2/3.%20testing_sensors_motors_combined)

To test sensors and motors together:
- Combine steps from Testing Moisture and Water Level Sensor and Testing Motor and Relay.
- Ensure the system reacts appropriately based on sensor readings to control pump operations.

### 4. [Testing GSM Module](https://github.com/ameer-salam/Arduino-Projects/tree/main/Borewell_and_Smart_irrigation_system_v2/4.%20testing_GSM_module)

To test the GSM module:
- Ensure the GSM module is correctly connected to the Arduino and powered on.
- Send test SMS messages to the system with valid commands.
- Verify the system receives SMS commands and executes corresponding actions (e.g., turning pumps on/off).

### 5. [Final Circuit Diagram](https://github.com/ameer-salam/Arduino-Projects/tree/main/Borewell_and_Smart_irrigation_system_v2/5.%20Final%20Circuit%20Diagram)

- Ensure all connections are correctly made as per the circuit diagram.
- Verify power supplies, sensor connections, relay connections, and GSM module connections.
- Double-check wiring against the schematic to prevent any potential issues.

### 6. [Final Code](https://github.com/ameer-salam/Arduino-Projects/tree/main/Borewell_and_Smart_irrigation_system_v2/6.%20Final%20code/FinalCode_)

## Commands

### Commands via SMS:

- **status**: Retrieves system status including sensor readings and pump status.
- **m1 on**: Turns on the borewell pump.
- **m1 off**: Turns off the borewell pump.
- **m2 on**: Turns on the field pump.
- **m2 off**: Turns off the field pump.
- **m off**: Turns off both pumps.
- **auto**: Switches to automatic mode.
- **manual**: Switches to manual mode.

### Commands via Serial Monitor:

- Same commands as SMS commands listed above.

## Note

- Ensure proper initialization and configuration of the GSM module with AT commands.
- Calibrate sensor thresholds (moisture and water level) based on your specific environment and needs.
- Test the system thoroughly before deploying it in the field to ensure reliability and correct operation.

---

This README provides a comprehensive overview of the GSM based smart irrigation system, including its functionality, testing procedures, author information, commands for operation, modes of operation, and considerations for deployment. Adjust the content as per your specific project details and requirements.
