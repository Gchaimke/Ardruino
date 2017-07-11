#include <L298NDualMotor.h>
/*
//pin definition
#define EN 9
#define IN1 8
#define IN2 7
*/
//create a motor instance
L298NDualMotor motor(10, 9, 8, 5, 7, 6);    //(ENL, IN1L, IN2L, ENR, IN1R, IN2R)  , startting defoult speed 100

void setup() {

  //used for display information
  Serial.begin(9600);

  motor.setSpeed(80); // an integer between 0 and 255

}

void loop() {

  //tell the motor to go forward (may depend by your wiring)
  motor.forward();

  //print the motor satus in the serial monitor
  Serial.print("Is moving = ");
  Serial.println(motor.isMoving());

  delay(3000);

  //stop running
  motor.stop();

  Serial.print("Is moving = ");
  Serial.println(motor.isMoving());

  delay(3000);

  //change the initial speed
  motor.setSpeed(100);

  //tell the motor to go back (may depend by your wiring)
  motor.backward();

  Serial.print("Is moving = ");
  Serial.println(motor.isMoving());

  delay(3000);

  //stop running
  motor.stop();

  Serial.print("Is moving = ");
  Serial.println(motor.isMoving());

  //change the initial speed
  motor.setSpeed(255);

  Serial.print("Get new speed of left motor = ");
  Serial.println(motor.getSpeedL());
  Serial.print("Get new speed of right motor = ");
  Serial.println(motor.getSpeedR());

  delay(3000);
}
