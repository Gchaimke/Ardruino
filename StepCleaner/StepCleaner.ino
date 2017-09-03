#include <AccelStepper.h>
#include <Ultrasonic.h>
#define HALFSTEP 8

// motor pins
#define motorPin1  2     // IN1 on the ULN2003 driver 1
#define motorPin2  3     // IN2 on the ULN2003 driver 1
#define motorPin3  4     // IN3 on the ULN2003 driver 1
#define motorPin4  5     // IN4 on the ULN2003 driver 1

#define motorPin5  6     // IN1 on the ULN2003 driver 2
#define motorPin6  7     // IN2 on the ULN2003 driver 2
#define motorPin7  8    // IN3 on the ULN2003 driver 2
#define motorPin8  9    // IN4 on the ULN2003 driver 2

// ultrasonic vars
// Ultrasonic sensors
Ultrasonic ultraHigh(10, 11);     // (Trig PIN,Echo PIN)
Ultrasonic ultraLow(13, 12);   // (Trig PIN,Echo PIN)

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);
AccelStepper stepper2(HALFSTEP, motorPin5, motorPin7, motorPin6, motorPin8);

// variables
char* myStrings[]={"Starting Loop", "Going forword", "Going backword",
"Turning left", "Turning right","End Loop"};
short msgNum= 0;
short lTurnsCount = 0;
unsigned short cmH, cmL;
unsigned long endtime = 360; //stop after seconds
//unsigned long endtime = 1800; //stop after 30 minutes

int turnSteps = 2100; // number of steps for a 90 degree turn
int lineSteps = 12800; //number of steps to drive straight
int stepperSpeed = 1000; //speed of the stepper (steps per second)
float stepperMaxSpeed =  2000.0;
int steps1 = 0; // keep track of the step count for motor 1
int steps2 = 0; // keep track of the step count for motor 2

boolean loopEnd = false;
boolean turn1 = false; //keep track if we are turning or going straight next
boolean turn2 = false; //keep track if we are turning or going straight next

void setup() {
  Serial.begin(9600); // Monitor port
  //delay(3000); //sime time to put the robot down after swithing it on

  stepper1.setMaxSpeed(stepperMaxSpeed);
  stepper1.move(1);  // I found this necessary
  stepper1.setSpeed(stepperSpeed);

  stepper2.setMaxSpeed(stepperMaxSpeed);
  stepper2.move(-1);  // I found this necessary
  stepper2.setSpeed(stepperSpeed);

  //rotate = random(400, 800);
  randomSeed(analogRead(0));

}
void loop() {
  while ( millis() < endtime*1000)
  {
    if (millis() / 1000 % 5 == 0) {
      //Serial.println("================================ " );
      //Serial.print("END AFTER ");
      //Serial.println(endtime-millis()/1000);
      Serial.println(myStrings[msgNum]);
      Serial.println(lTurnsCount);
      distance();
    }

    if (steps1 == 0) {
      int target = 0;
      if (turn1 == true) {
        target = turnSteps;
        msgNum = 3;
        lTurnsCount++;
      }
      else {
        target = lineSteps;
        msgNum = 1;
      }

      stepper1.move(target);
      stepper1.setSpeed(stepperSpeed);
      turn1 = !turn1;
    }

   if (steps2 == 0) {
      int target = 0;
      if (turn2 == true) {
        target = turnSteps;
      }
      else {
        target = -lineSteps;
      }

      stepper2.move(target);
      stepper2.setSpeed(stepperSpeed);
      turn2 = !turn2;
    }

    steps1 = stepper1.distanceToGo();
    steps2 = stepper2.distanceToGo();
    //Serial.println(steps2);

    stepper1.runSpeedToPosition();
    stepper2.runSpeedToPosition();
  } //end timer while
  
  if (endtime < millis()) {
    //TO-DO stop relay
    while(loopEnd == false){
       Serial.println("Work End. ");
       loopEnd = true;
      
    }
  }
}

//ОПРЕДЕЛЕНИЕ ПРЕПЯТСТВИЙ ДАЛЬНОМЕТРАМИ
void distance() {
  //ЛЕВЫЙ ДАЛЬНОМЕР
  cmH = ultraHigh.Ranging(CM);    //посылается и принимается звуковой сигнал
  delay(1);                      //небольшая задержка, для предотвращения смешения сигналов со вторым дальномером
  //ПРАВЫЙ ДАЛЬНОМЕР
  cmL = ultraLow.Ranging(CM);   //посылается и принимается звуковой сигнал
  delay(1);                      //небольшая задержка, для предотвращения смешения сигналов со вторым дальномером

  //Debug of ultrasonic sensors
  //Serial.print("High: ");
  Serial.println(cmH); // CM or INC
  //Serial.print(" cm     " );
  //delay(1);
  //Serial.print("Low: ");
  Serial.println(cmL); // CM or INC
  //Serial.println(" cm" );
  //delay(1);
  //if (cmH == 51) ;
  //if (cmL == 51) ;
}

