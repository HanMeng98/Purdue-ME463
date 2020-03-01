/*******************************
*Project: ME463 Senior Design
*Author : Helping the Human
*Create : 02/27/2020
*Update : 03/01/2020
*******************************/
#include <Servo.h>
#include <Stepper.h>

#define STEPS_X 200
#define STEPS_Y 200
#define STEPS_Z 1818

Stepper x_motor(STEPS_X,1,2,3,4);
Stepper y_motor_left(STEPS_Y,5,6,7,8), y_motor_right(STEPS_Y,5,6,7,8);
Stepper z_motor_left, z_motor_right;
Servo z_servo_left, z_servo_right;
Servo clamp_left, clamp_right;
int sensorPin = A0;


void baseMovement(float ratio){
  // x motion
  int whole_steps = 1000; //////////// TEST //////////
  int steps = (int)(ratio*whole_steps);
  x_motor.step(steps);
  delay(500);
}

void liftMovement(float ratio){
  int whole_steps = 1000; //////????? TEST ??//////////////
  int steps = (int)(ratio*whole_steps);
  y_motor_left.step(steps);
  y_motor_right.step(-steps);
  delay(500);
}

void hookMovement(float inch){
  float ratio = inch/2/3.14;
  int steps = (int) (STEPS_Z*ratio);
  z_motor_left.step(steps);
  z_motor_right.step(steps);
  delay(500);
}

void hookRotation(int degree){
  z_motor_left.step(degree);
  z_motor_right.step(degree);
  delay(1000);
}

void clampMovement(int flag){
  int pos = 40; ///???? NEED TEST ?/////
  if(flag == 1){// close clamp
    z_servo_left.write(pos);
    z_servo_right.write(-pos);
    delay(1000);
  }else{
        z_servo_left.write(-pos);
    z_servo_right.write(pos);
    delay(1000);
  }
}

/*********************************************
 * NEED TO TEST THE THRESHOLD
 *********************************************/
int pressureDetection(){
//   // pressure plate connect to Analog 0
  int volt;
  int threshold = 10?;
  int sum = 0 ;
  for(int i=0;i<1000;i++){
    volt = analogRead(sensorPin);
    if (volt > threshold) sum = sum + 1;
  }
  if (sum>=900) return 1; //ready to start
  else return 0; //not ready
}

void setup() {
  // put your setup code here, to run once:
  x_motor.setSpeed(60);
  y_motor_left.setSpeed(60);
  y_motor_right.setSpeed(60);
  z_motor_left.setSpeed(60);
  z_motor_right.setSpeed(60);
  z_servo_left.attach(9);
  z_servo_right.attach(10);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int flag = pressureDetection();
  if (flag == 1){
    // go all the length
    baseMovement(1);
    // back to center
    baseMovement(-1/2);
    // lift up
    liftMovement(1/2);
    // move close
    hookMovement(inch);
    // rotate a little'
    hookRotation(degree);
    // close clamp
    clampMovement(1);
    // pull away
    hookMovement(-inch);
    // open clamp
    clampMovement(0);

  }
}