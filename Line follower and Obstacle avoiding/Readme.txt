For more information and resources visit - https://github.com/ameer-salam/Arduino-Projects/tree/main/Line%20follower%20and%20Obstacle%20avoiding


Component requirements:
1. Arduino Uno 
2. Ultrasonic sensor Hc-sr04 with case to obstacle avoidance.
3. 2 x Ir Sensor for tracing the line.  
4. L293d motor driver shield for controlling the motor.
5. Servo motor
6. 4 x motor gear with wheel (4x motor chassis).
Jumper wire
Power supply

connection:

1. First connect motor driver shield (Adafruit shield) onto the arduino board.

2. Now connect the bo motor's to the l293d motor driver shield.
Motor 1 to motor driver M1 (left)
Motor 2 to motor driver M2 (left)
Motor 3 to motor driver M3 (right)
Motor 4 to motor driver M4 (right)

 3.  connect the IR sensor to motor driver.
IR sensor OUT pin is connected to motor driver analog pins
       right IR sensor out - Analog 01
       left  IR sensor out - Analog 00
IR sensor GND pin is connected to motor driver GND pin.
IR sensor VCC pin is connected to motor driver 5v pin. 

4. Connect the servo motor to motor driver servo1 slot.

5. Connect ultrasonic sensor to motor driver.
Hc-sr04 TRIG pin to motor driver A2.
Hc-sr04 ECHO pin to motor driver A3.
Hc-sr04 5v pin to motor driver 5v.
Hc-sr04 GND pin to motor driver GND.

Circuit diagram - https://github.com/ameer-salam/Arduino-Projects/blob/main/Line%20follower%20and%20Obstacle%20avoiding/Line_follower_and_Obstacle_avoiding/line%20follow%20and%20object%20avoid_bb%20(1).png

upload the code.

code link - https://github.com/ameer-salam/Arduino-Projects/blob/main/Line%20follower%20and%20Obstacle%20avoiding/Line_follower_and_Obstacle_avoiding/Line_follower_and_Obstacle_avoiding.ino


Download Library files required to compile the code successfully
link - https://github.com/ameer-salam/Arduino-Projects/tree/main/Line%20follower%20and%20Obstacle%20avoiding/Drivers
