//obstacle avoiding and line following rover
//by Ameer Salam - 17th July, 2022. Location - Tumkur, India
//linkedin  -  https://www.linkedin.com/in/ameersalam
//github    -  https://github.com/ameer-salam


//components required
/*
 * Arduinio (Preferably Arduino Uno)  (information/tutorial - https://www.arduino.cc/en/Guide/ArduinoUno )
 * L293d motor driver shield          (information/tutorial - https://create.arduino.cc/projecthub/electropeak/arduino-l293d-motor-driver-shield-tutorial-c1ac9b )
 * HC-SR04 Ultrasonic sensor          (information/tutorial - https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/ )
 * 2x Infrared sensors                (information/tutorial - https://create.arduino.cc/projecthub/Raushancpr/arduino-with-ir-sensor-1579b6 )
 * 4x Brushed DC motors (Preferably 4x motor chassis, 2x will work too!)
 * servo motor
 * Jumper wires (male-to-male and male-to-female)
 * power supple (9-12v Battery)
 */

//connections 
/*
 * HS-SR04 GND -to- Motor shield Gnd 
 * HC-SR04 VCC -to- Motor shield 5v Vcc
 * HC-SR04 TRIG -to- Analog 02
 * HC-SR04 ECHO -to- Analog 03
 * IR Sensor VCC - 5V Vcc
 * IR Sensor GND - Gnd
 * IR sensor (Right) out - Analog 01
 * IR sensor (Left)  out - Analog 0
 * Servo motor - Motor shield Servo out
 * Left Motors - Motor shield M1 and M2
 * Right Motors - Motor shield M3 and M4
*/


//librares
#include <NewPing.h>      //link to download - https://github.com/ameer-salam/arduinodrivers/tree/main/NewPing-master 
#include <Servo.h>        //link to download - https://github.com/ameer-salam/arduinodrivers/tree/main/Servo-master
#include <AFMotor.h>      //link to download - https://github.com/ameer-salam/arduinodrivers/tree/main/Adafruit-Motor-Shield-library-master

//Infrared sensor for Line detection
#define irLeft A0
#define irRight A1

//motor
#define MAX_SPEED 200
#define MAX_SPEED_OFFSET 20


//HC-SR04 Ultrasonic sensor detecting objects
#define TRIGGER_PIN A2
#define ECHO_PIN A3
#define max_distance 50

Servo servo;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, max_distance);

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);


int distance = 0;
int leftDistance;
int rightDistance;
boolean object;

void setup() {
  Serial.begin(9600);
  pinMode(irLeft, INPUT);
  pinMode(irRight, INPUT);
  servo.attach(10);
  servo.write(90);

  motor1.setSpeed(120);
  motor2.setSpeed(120);
  motor3.setSpeed(120);
  motor4.setSpeed(120);
}

void loop() {
  if (digitalRead(irLeft) == 0 && digitalRead(irRight) == 0 ) {
    objectAvoid();
    //forword
  }
  else if (digitalRead(irLeft) == 0 && digitalRead(irRight) == 1 ) {
    objectAvoid();
    Serial.println("TL");
    //leftturn
    moveLeft();
  }
  else if (digitalRead(irLeft) == 1 && digitalRead(irRight) == 0 ) {
    objectAvoid();
    Serial.println("TR");
    //rightturn
    moveRight();
  }
  else if (digitalRead(irLeft) == 1 && digitalRead(irRight) == 1 ) {
    //Stop
    Stop();
  }
}

void objectAvoid() {
  distance = getDistance();
  if (distance <= 15) {
    //stop
    Stop();
    Serial.println("Stop");

    lookLeft();
    lookRight();
    delay(100);
    if (rightDistance <= leftDistance) {
      //left
      object = true;
      turn();
      Serial.println("moveLeft");
    } else {
      //right
      object = false;
      turn();
      Serial.println("moveRight");
    }
    delay(100);
  }
  else {
    //forword
    Serial.println("moveforword");
    moveForward();
  }
}

int getDistance() {
  delay(50);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 100;
  }
  return cm;
}

int lookLeft () {
  //lock left
  servo.write(150);
  delay(500);
  leftDistance = getDistance();
  delay(100);
  servo.write(90);
  Serial.print("Left:");
  Serial.print(leftDistance);
  return leftDistance;
  delay(100);
}

int lookRight() {
  //lock right
  servo.write(30);
  delay(500);
  rightDistance = getDistance();
  delay(100);
  servo.write(90);
  Serial.print("   ");
  Serial.print("Right:");
  Serial.println(rightDistance);
  return rightDistance;
  delay(100);
}
void Stop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}
void moveForward() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}
void moveBackward() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);

}
void turn() {
  if (object == false) {
    Serial.println("turn Right");
    moveLeft();
    delay(700);
    moveForward();
    delay(800);
    moveRight();
    delay(900);
    if (digitalRead(irRight) == 1) {
      loop();
    } else {
      moveForward();
    }
  }
  else {
    Serial.println("turn left");
    moveRight();
    delay(700);
    moveForward();
    delay(800);
    moveLeft();
    delay(900);
    if (digitalRead(irLeft) == 1) {
      loop();
    } else {
      moveForward();
    }
  }
}
void moveRight() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}
void moveLeft() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}
