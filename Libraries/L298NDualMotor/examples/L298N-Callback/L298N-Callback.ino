#include <L298NDualMotor.h>

//pin definition
#define EN 9
#define IN1 8
#define IN2 7

//create a motor instance
L298NDualMotor motor(10, 9, 8, 5, 7, 6);    //(ENL, IN1L, IN2L, ENR, IN1R, IN2R)  , startting defoult speed 100

//initial speed
unsigned short theSpeed = 0;

void setup() {

  //set the initial speed
  motor.setSpeed(theSpeed);

}

void loop() {

  //move motor for 5 seconds and then execute the callback function
  motor.forwardFor(5000, callback);

}

void callback() {

  //each time the callback function is called increase the speed of the motor or reset to 0
  if (theSpeed <= 255) {
    theSpeed += 25;
  } else {
    theSpeed = 0;
  }

  //re-enable motor movements
  motor.reset();
  //set the new speed
  motor.setSpeed(theSpeed);

}
