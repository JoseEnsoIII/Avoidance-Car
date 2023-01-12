#include "AFMotor.h"
#include <NewPing.h>
#include <Servo.h> 

#define TRIG_PIN A0 
#define ECHO_PIN A1 
#define MAX_DISTANCE 200 
#define MAX_SPEED 190 // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 

const int MOTOR_1 = 1; 
const int MOTOR_2 = 2; 
const int MOTOR_3 = 3; 
const int MOTOR_4 = 4; 



AF_DCMotor motor1(MOTOR_1, MOTOR12_64KHZ); // create motor object, 64KHz pwm
AF_DCMotor motor2(MOTOR_2, MOTOR12_64KHZ); // create motor object, 64KHz pwm
AF_DCMotor motor3(MOTOR_3, MOTOR12_64KHZ); // create motor object, 64KHz pwm
AF_DCMotor motor4(MOTOR_4, MOTOR12_64KHZ); // create motor object, 64KHz pwm

Servo myservo;   

boolean goesForward=false;
int distance = 200;
int speedSet = 0;

int state;
int Speed = 130; 


void setup() {
motor1.setSpeed(Speed);   // set the motor speed to 0-255
motor2.setSpeed(Speed);
motor3.setSpeed(Speed);
motor4.setSpeed(Speed);  
Serial.begin(9600);

  myservo.attach(10);  
  myservo.write(115); 
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
delay(500); 
}

void loop(){   
if(Serial.available() > 0){  //if some date is sent, reads it and saves in state     
state = Serial.read();      
if(state > 10){Speed = state;}      
}
           
motor1.setSpeed(Speed);          // set the motor speed to 0-255
motor2.setSpeed(Speed);
motor3.setSpeed(Speed);
motor4.setSpeed(Speed);
//===============================================================================   
//===============================================================================
//                          Key Control Command
//===============================================================================   
if(state == 1){forward(); }  // if the state is '1' the DC motor will go forward
else if(state == 2){backward();}  // if the state is '2' the motor will Reverse
else if(state == 3){turnLeft();}  // if the state is '3' the motor will turn left
else if(state == 4){turnRight();} // if the state is '4' the motor will turn right
else if(state == 5){Stop(); }     // if the state is '5' the motor will Stop
else if(state == 10){Obstacle();} // if the state is '10' automatic obstacle
/////////////////////////////////////END\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//===============================================================================
//                          Voice Control Command
//===============================================================================
else if(state == 6){turnLeft();  delay(400);  state = 5;}
else if(state == 7){turnRight(); delay(400);  state = 5;}
/////////////////////////////////////END\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

delay(80);    
}

void Obstacle(){
 int distanceR = 0;
 int distanceL =  0;
 delay(40);
 
 if(distance<=45)
 {
  moveStop();
  delay(50);
  moveBackward();
  delay(100);
  moveStop();
  delay(50);
  distanceR = lookRight();
  delay(50);
  distanceL = lookLeft();
  delay(50);

  if(distanceR>=distanceL)
  {
    turnRightOB();
    moveStop();
  }else
  {
    turnLeftOB();
    moveStop();
  }
 }else
 {
  moveForward();
 }
 distance = readPing();
}

int lookRight()
{
    myservo.write(100); 
    delay(200);
    int distance = readPing();
    delay(50);
    myservo.write(115); 
    return distance;
}

int lookLeft()
{
    myservo.write(170); 
    delay(200);
    int distance = readPing();
    delay(50);
    myservo.write(115); 
    return distance;
    delay(50);
}

int readPing() { 
  delay(50);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
}

void moveStop() {
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  } 
  
void moveForward() {

 if(!goesForward)
  {
    goesForward=true;
    motor1.run(FORWARD);      
    motor2.run(FORWARD);
    motor3.run(FORWARD); 
    motor4.run(FORWARD);     
   for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
   {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
   }
  }
}

void moveBackward() {
    goesForward=false;
    motor1.run(BACKWARD);      
    motor2.run(BACKWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);  
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
  {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
  }
}  

void turnRightOB() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);     
  delay(500);
  motor1.run(FORWARD);      
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);      
} 
 
void turnLeftOB() {
  motor1.run(BACKWARD);     
  motor2.run(BACKWARD);  
  motor3.run(FORWARD);
  motor4.run(FORWARD);   
  delay(500);
  motor1.run(FORWARD);     
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void forward(){
motor1.run(FORWARD); // turn it on going forward
motor2.run(FORWARD); 
motor3.run(FORWARD); 
motor4.run(FORWARD);
}

void backward(){
motor1.run(BACKWARD); // the other way
motor2.run(BACKWARD);
motor3.run(BACKWARD); 
motor4.run(BACKWARD); 
}

void turnRight(){
motor3.run(FORWARD); // the other right
motor4.run(FORWARD); 
motor1.run(BACKWARD); 
motor2.run(BACKWARD);
}

void turnLeft(){
motor3.run(BACKWARD); // turn it on going left
motor4.run(BACKWARD);
motor1.run(FORWARD); 
motor2.run(FORWARD); 
}

void Stop(){
 motor1.run(RELEASE); // stopped
 motor2.run(RELEASE);
 motor3.run(RELEASE);
 motor4.run(RELEASE);
}