/* ******************************************************************* ОБЪЯВЛЕНИЕ ПЕРЕМЕННЫХ ****************************************************** */
// Libraries
#include <Ultrasonic.h>
#include <L298NDualMotor.h>

// Ultrasonic sensors
Ultrasonic ultraleft(3, 2);     // (Trig PIN,Echo PIN)
Ultrasonic ultraright(11, 4);   // (Trig PIN,Echo PIN)

// Motor driver PINS 
L298NDualMotor motor(10, 9, 8, 5, 7, 6);    //(ENL, IN1L, IN2L, ENR, IN1R, IN2R)  , startting defoult speed 100

// Integers
byte moving = 1;
byte rotate = 0;
byte rotate_side = 1;
unsigned short cmL, cmR;

// Timers
byte forwardCounter = 0;
byte leftTurns = 0;
byte rightTurns = 0;
byte turnSpeed = 120;

// переменные вентилятора
int relay = 15;
unsigned long endtime = 1800000; //автоматическое выключение через 30 minutes

/* ******************************************************** SETUP программы (выполняется 1 раз, при старте) **************************************** */
void setup() 
 {
   //set motor speed
   motor.setSpeed(75);
   // Fan Pin
   pinMode(relay, OUTPUT);
   digitalWrite(relay, HIGH);
   Serial.begin(9600); // Monitor port
 }

/* ******************************************************************* РАБОЧИЙ ЦИКЛ ПРОГРАММЫ ****************************************************** */
void loop() {
  while ( millis() != endtime)
  {
    //ОПРЕДЕЛЕНИЕ НАПРАВЛЕНИЯ ДВИЖЕНИЯ
    distance();
    
    //нет препятствия
    do
    {
      Serial.println("No barriers at the front of me");
      motor.forward();
      delay(50);
      forwardCounter++;
      distance();
    }
    while (cmL > 15 && cmR > 15 && forwardCounter < 50);
    
    //Zero forwardCounter
    if (forwardCounter > 50 && leftTurns < 30 && rightTurns < 30) {
      Serial.println("Going back, zero the forwardCounter");
      forwardCounter =0;
      motor.backward();
      delay(random(500, 3000));                                                      //go back for 1 second
      rotate_side = random(4);                                      //рандомная сторона поворота
      if (rotate_side == 1 || rotate_side == 0) {
        Serial.println("Going randomaly left, after zeroing forwardCounter");
        motor.left(turnSpeed);
        delay(random(3000, 8000));
        leftTurns++;
      } else {
        Serial.println("Going randomaly right, after zeroing forwardCounter");
        motor.right(turnSpeed);
        delay(random(3000, 8000));
        rightTurns++;
      }
      leftTurns =0;
      rightTurns =0;
      distance();
    }   

      //More left turns that needed
      if (leftTurns > 30) 
        {
         motor.stop();
         motor.backward(); 
         delay(random(500, 1500));                                                    //go back for 1 second
         motor.right(turnSpeed);
         delay(random(500, 1500));
         leftTurns = 0;
         rightTurns = 0;
         Serial.println("Zero counter of left turns");
         distance();
        }
      
      //More right turns that needed
      if (rightTurns > 30) 
        {
         motor.stop();
         motor.backward(); 
         delay(random(500, 1500));                                                    //go back for 1 second
         motor.left(turnSpeed);
         delay(random(500, 1500));
         rightTurns = 0;
         leftTurns = 0;
         Serial.println("Zero counter of right turns");
         distance();
        }
    
      //Stuck on the same place !
      if (leftTurns > 15 || rightTurns > 15) {
       Serial.println("Stuck on the place !");
       motor.stop();
       motor.backward(); 
       delay(random(500, 3000));                                                    //go back for 1 second
       rotate_side = random(1, 3);                                      //рандомная сторона поворота
      if (rotate_side == 1) {
        Serial.println("Going randomaly left, zero counter");
        motor.left(turnSpeed);
        delay(random(1000, 3000));
        leftTurns++;
      } else {
        Serial.println("Going randomaly right, zero counter");
        motor.right(turnSpeed);
        delay(random(1000, 3000));
        rightTurns++;
      }
       leftTurns = 0;
       rightTurns = 0;
       Serial.println("Zero cmL and cmR counters");
       distance();
    }
    
    //препятствие (Have a barrier from the left side)
    if (cmL < 15 && cmL > 5) 
      {
        Serial.println("Going right because have a barrier from the left");
        motor.stop();
        motor.backward();
        delay(200);
        while (cmL < 15*1.5 && (leftTurns < 20 && rightTurns < 20))
         {
           motor.right(turnSpeed);
           Serial.print("Have a barrier from the left...  ");
           rightTurns++;
           distance();
         }
         rightTurns =0;
         distance();
      }

    //препятствие (Have a barrier from the right)
    if ( cmR < 15 && cmR < 5) 
      {
        Serial.println("Going left because have a barrier from the right");
        motor.stop();
        motor.backward();
        delay(200);
        while (cmR < 15*1.5 && (leftTurns < 20 && rightTurns < 20))
         {
          motor.left(turnSpeed);
          Serial.print("Have a barrier from the right...  ");
          leftTurns++;
          distance();
         }
         leftTurns =0;
         distance();
       }

    //near the wall (Have a barrier from the left side)
    if (cmR >5  && cmL < 5 ) 
      {
        Serial.println("Going right because have a barrier from the left");
        motor.stop();
        motor.backward(); 
        delay(200); 
        while (cmL < 15  && (leftTurns < 20 && rightTurns < 20))
         {
           motor.right(turnSpeed);
           Serial.print("Near the wall going Right...  ");
           rightTurns++;
           distance();
         }
        distance();
      }

    //near the wall (Have a barrier from the right)
    if (cmL > 5 && cmR < 5 ) 
      {
        Serial.println("Going left because have a barrier from the right");
        motor.stop();
        motor.backward(); 
        delay(200); 
        while (cmR < 15 && (leftTurns < 20 && rightTurns < 20))
         {
           motor.left(turnSpeed);
           Serial.print("Near the wall going Le]]ft...  ");
           leftTurns++;
           distance();
         }
        distance();
      }     

    //препятствие (Turn around)
    if (cmL <=5 && cmR <=5) 
      {
       Serial.println("Going back because have the wall");
       while (cmL<10 && cmR <10){
        motor.backward();
        distance();
       }
       rotate_side = random(1, 3);
       if (rotate_side == 1) {
          Serial.println("Turn around left");
          motor.left(turnSpeed);
          delay(7000); 
        } else {
        
          motor.right(turnSpeed);
          delay(7000);
        }
        distance();
      }

    //ОСТАНОВКА РАБОТЫ ЧЕРЕЗ ЗАДАННОЕ ВРЕМЯ
    if (endtime < millis()) 
      {
        digitalWrite(relay, LOW); //torn off FAN
        motor.stop();
      }
  }
} 
//ОПРЕДЕЛЕНИЕ ПРЕПЯТСТВИЙ ДАЛЬНОМЕТРАМИ
void distance(){
  //ЛЕВЫЙ ДАЛЬНОМЕР
  cmL = ultraleft.Ranging(CM);    //посылается и принимается звуковой сигнал
  delay(25);                      //небольшая задержка, для предотвращения смешения сигналов со вторым дальномером
  //ПРАВЫЙ ДАЛЬНОМЕР
  cmR = ultraright.Ranging(CM);   //посылается и принимается звуковой сигнал
  delay(25);                      //небольшая задержка, для предотвращения смешения сигналов со вторым дальномером

  //Debug of ultrasonic sensors
  Serial.println("initializing distance");
  Serial.print("Left: ");
  Serial.print(ultraleft.Ranging(CM)); // CM or INC
  Serial.print(" cm     " );
  delay(25);
  Serial.print("Right: ");
  Serial.print(ultraright.Ranging(CM)); // CM or INC
  Serial.println(" cm" );
  delay(50);
  if (cmL == 51) ;
  if (cmR == 51) ;
}
