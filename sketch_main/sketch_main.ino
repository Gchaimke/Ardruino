                    // ОБЪЯВЛЕНИЕ ПЕРЕМЕННЫХ //
// переменные ультразвуковых дальномеров
#include <Ultrasonic.h>
#include <L298NDualMotor.h>
// переменные ultrasonic sensors
Ultrasonic ultraleft(3, 2);     // (Trig PIN,Echo PIN)
Ultrasonic ultraright(11, 4);   // (Trig PIN,Echo PIN)

// переменные двигателей
//create a motor instance
L298NDualMotor motor(10, 9, 8, 5, 7, 6);    //(ENL, IN1L, IN2L, ENR, IN1R, IN2R)  , startting defoult speed 100

// Разные переменные
int moving = 1;
int rotate = 0;
int rotate_side = 1;
long cmL, cmR;

// переменные таймеров анти-залипания
int counter = 0;
int cmLCounter = 0;
int cmRCounter = 0;

// переменные вентилятора
int relay = 12;
unsigned long endtime = 600000; //автоматическое выключение через 600 секунд


      // SETUP программы (выполняется 1 раз, при старте) //
void setup() {
  //set motor speed
  motor.setSpeed(100);
  // Fan Pin
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  //генерация рандомного числа со свободной ножки
  randomSeed(analogRead(0));

  Serial.begin(9600); // Monitor port

}


                  // РАБОЧИЙ ЦИКЛ ПРОГРАММЫ //
void loop() {
  //цикл выполняется пока время не достигло endtime
  if (millis() < endtime) {
    
    //ОПРЕДЕЛЕНИЕ НАПРАВЛЕНИЯ ДВИЖЕНИЯ
    distance();

    //Stuck on the place !
    if (cmLCounter > 20 || cmRCounter > 20) {
      Serial.println("Stuck on the place !");
       motor.stop();
       motor.backward();
       delay(300);
       distance();
       motor.right();
       delay(random(200, 400));
       motor.forward();
       delay(100);
       motor.left();
       delay(random(200, 400));
       cmLCounter = 0;
       cmRCounter = 0;
    }
    
    //нет препятствия
    if (cmL >= 20 && cmR >= 20 && counter < 15 && (cmLCounter < 20 && cmRCounter < 20)) { 
      Serial.println("Going Forward");
      motor.forward();
      delay(300);
      counter++;
      Serial.println("counter is =" + counter);
    }

    //препятствие (Have a barrier from the left side)
    if (cmR >= 10  && cmL < 10 && (cmLCounter < 20 && cmRCounter < 20)) {
        Serial.println("Going right because have a barrier from the left");
        motor.stop();
        motor.backward();
        delay(300);
        distance();
        motor.right();
        delay(random(200, 400));
        motor.forward();
        delay(100);
        motor.left();
        delay(random(200, 400)); }

    //препятствие (Have a barrier from the right)
    if (cmL >= 10 && cmR < 10 && (cmLCounter < 20 && cmRCounter < 20)) {
        Serial.println("Going left because have a barrier from the right");
        motor.stop();
        motor.backward();
        delay(300);
        distance();
        motor.left();
        delay(random(200, 400));
        motor.forward();
        delay(100);
        motor.right();
        delay(random(200, 400));

    //препятствие (поворот)
    if (((cmL <=7 && cmR <= 7 && cmL==cmR && (cmLCounter < 20 && cmRCounter < 20)) || counter == 15)) {
      Serial.println("Going back 1");
      counter = 0;
      Serial.println("Zero counter");
      motor.backward();
      rotate_side = random(1, 3); //рандомная сторона поворота
      if (rotate_side == 1) {
        Serial.println("Going randomaly left");
        motor.left();
        delay(random(200, 400));
      } else {
        Serial.println("Going randomaly right");
        motor.right();
        delay(random(200, 400));
      }
    }

    //ОСТАНОВКА РАБОТЫ ЧЕРЕЗ ЗАДАННОЕ ВРЕМЯ
    if (endtime < millis()) {
      digitalWrite(relay, LOW); //torn off FAN
      motor.stop();
    }
  }
 }}
void distance(){
    //ОПРЕДЕЛЕНИЕ ПРЕПЯТСТВИЙ ДАЛЬНОМЕТРАМИ
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
