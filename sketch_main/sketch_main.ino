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
int moving = 1;
int rotate = 0;
int rotate_side = 1;
long cmL, cmR;

// Timers
int counter = 0;
int cmLCounter = 0;
int cmRCounter = 0;
int leftTurns = 0;
int rightTurns = 0;

// переменные вентилятора
int relay = 12;
unsigned long endtime = 1800000; //автоматическое выключение через 30 minutes

/* ******************************************************** SETUP программы (выполняется 1 раз, при старте) **************************************** */
void setup() 
 {
   //set motor speed
   motor.setSpeed(100);
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
      motor.forward(70, 80);
      delay(random(100, 500));
      counter++;
      distance();
    }
    while (cmL > 20 && cmR > 20 && counter < 30);
    
    //Zero Counter
    if (counter == 30) {
      Serial.println("Going back, zero counter");
      counter = 0;
      Serial.println("Zero counter");
      motor.backward();
      delay(500);                                                      //go back for 1 second
      rotate_side = random(1, 3);                                      //рандомная сторона поворота
      if (rotate_side == 1) {
        Serial.println("Going randomaly left, zero counter");
        motor.left();
        delay(random(400, 800));
        leftTurns++;
      } else {
        Serial.println("Going randomaly right, zero counter");
        motor.right();
        delay(random(400, 800));
        rightTurns++;
      }
    }   

      //More left turns that needed
      if (leftTurns > 10) 
        {
         motor.stop();
         motor.backward(); 
         delay(800);                                                    //go back for 1 second
         motor.left();
         delay(random(400, 1500));
         leftTurns = 0;
         Serial.println("Zero counter of left turns");
        }
      
      //More right turns that needed
      if (rightTurns > 10) 
        {
         motor.stop();
         motor.backward(); 
         delay(800);                                                    //go back for 1 second
         motor.right();
         delay(random(300, 1500));
         rightTurns = 0;
         Serial.println("Zero counter of right turns");
        }
    
      //Stuck on the same place !
      if (cmLCounter > 15 || cmRCounter > 15) {
       Serial.println("Stuck on the place !");
       motor.stop();
       motor.backward(); 
       delay(800);                                                    //go back for 1 second
       motor.right();
       rightTurns++;
       delay(random(200, 500));
       motor.forward(60, 70);
       delay(100);
       motor.left();
       leftTurns++;
       delay(random(200, 500));
       cmLCounter = 0;
       cmRCounter = 0;
       Serial.println("Zero cmL and cmR counters");
    }
    
    //препятствие (Have a barrier from the left side)
    if (cmR > 15  && cmL < 10 && (cmLCounter < 20 && cmRCounter < 20)) 
      {
        Serial.println("Going right because have a barrier from the left");
        motor.stop();
        delay(200);
        while (cmL < 10)
         {
           motor.right();
           Serial.print("Right...  ");
           rightTurns++;
           distance();
         }
      }

    //препятствие (Have a barrier from the right)
    if (cmL > 15 && cmR < 10 && (cmLCounter < 20 && cmRCounter < 20)) 
      {
        Serial.println("Going left because have a barrier from the right");
        motor.stop();
        delay(200);
        while (cmR < 10)
         {
          motor.left();
          Serial.print("Left...  ");
          leftTurns++;
          distance();
         }
       }

    //near the wall (Have a barrier from the left side)
    if (cmR >= 10  && cmL < 10 && (cmLCounter < 20 && cmRCounter < 20)) 
      {
        Serial.println("Going right because have a barrier from the left");
        motor.stop();
        delay(200);
        motor.backward(); 
        delay(500);                                  //go back for 1 second
        motor.right();
        delay(random(400, 800));
        rightTurns++;
        motor.forward(60, 70);
        delay(200);
      }

    //near the wall (Have a barrier from the right)
    if (cmL >= 10 && cmR < 10 && (cmLCounter < 20 && cmRCounter < 20)) 
      {
        Serial.println("Going left because have a barrier from the right");
        motor.stop();
        delay(200);
        motor.backward(); 
        delay(200);                                  //go back for 1 second
        motor.left();
        delay(random(400, 800));
        leftTurns++;
        motor.forward(60, 70);
        delay(200);
      }

    //препятствие (Turn around)
    if (cmL < 7 && cmR < 7) 
      {
       Serial.println("Going back because have the wall");
       motor.backward(); 
       delay(200);                                  //go back for 1 second
       motor.stop();
       delay(200);
       distance();
       if (cmL > cmR) 
        {
         Serial.println("Going randomaly left");
         motor.left();
         delay(random(400, 800));
         leftTurns++;
        } 
        else 
        {
         Serial.println("Going randomaly right");
         motor.right();
         delay(random(400, 800));
         rightTurns++;
        }
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
  delay(50);                      //небольшая задержка, для предотвращения смешения сигналов со вторым дальномером
  //ПРАВЫЙ ДАЛЬНОМЕР
  cmR = ultraright.Ranging(CM);   //посылается и принимается звуковой сигнал
  delay(50);                      //небольшая задержка, для предотвращения смешения сигналов со вторым дальномером

  //Debug of ultrasonic sensors
  Serial.println("initializing distance");
  Serial.print("Left: ");
  Serial.print(ultraleft.Ranging(CM)); // CM or INC
  Serial.print(" cm     " );
  delay(50);
  Serial.print("Right: ");
  Serial.print(ultraright.Ranging(CM)); // CM or INC
  Serial.println(" cm" );
  delay(50);
  if (cmL == 51) cmLCounter++;
  if (cmR == 51) cmRCounter++;
}
