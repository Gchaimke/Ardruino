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
int timer1 = 0;
int timer2 = 0;
int timer3 = 0;
int timer4 = 0;

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
  //включение реле

}


                  // РАБОЧИЙ ЦИКЛ ПРОГРАММЫ //
void loop() {
  //цикл выполняется пока время не достигло endtime
  if (millis() < endtime) {

    //ОПРЕДЕЛЕНИЕ ПРЕПЯТСТВИЙ ДАЛЬНОМЕТРАМИ
    //ЛЕВЫЙ ДАЛЬНОМЕР
    cmL = ultraleft.Ranging(CM);    //посылается и принимается звуковой сигнал
    delay(50);                      //небольшая задержка, для предотвращения смешения сигналов со вторым дальномером
    //ПРАВЫЙ ДАЛЬНОМЕР
    cmR = ultraright.Ranging(CM);   //посылается и принимается звуковой сигнал
    delay(50);                      //небольшая задержка, для предотвращения смешения сигналов со вторым дальномером

    //ОПРЕДЕЛЕНИЕ НАПРАВЛЕНИЯ ДВИЖЕНИЯ
    //нет препятствия
    if (cmL >= 10 && cmR >= 10) {
      if (cmL > 1 && cmL != 0) {
        motor.forward(100 / cmL);
      } else {
        motor.stop();
      }
      delay(100);
      timer1 = timer1 + 1;
      timer2 = 0;
      timer3 = 0;
      timer4 = 0;
      moving = 1;
    }

    //далеко нет препятствий
    if (cmL >= 40 && cmR >= 40) {
      motor.forward();
      delay(100);
      timer4 = timer4 + 1;
      timer1 = 0;
      timer2 = 0;
      timer3 = 0;
      moving = 2;
    }

    //препятствие (объезд влево)
    if (cmL > 40 && cmR < 30 && moving != 5) {
      motor.right();
      delay(300);
      motor.forward();
      delay(random(400, 800));
      motor.left();
      delay(300);
      moving = 3;
    }

    //препятствие (объезд вправо)
    if (cmL < 30 && cmR > 40 && moving != 6) {
      motor.left();
      delay(300);
      motor.forward();
      delay(random(400, 800));
      motor.right();
      delay(300);
      moving = 4;
    }

    //препятствие (поворот)
    if (cmL < 10 || cmR < 10) {
      motor.right();
      delay(random(400, 800));
      timer2 = timer2 + 1;
      timer1 = 0;
      timer3 = 0;
      timer4 = 0;
      moving = 5;
    }

    //препятствие (назад)
    if (cmL <= 5 || cmR <= 5) {
      motor.backward();
      delay(random(400, 800));
      timer2 = timer2 + 1;
      timer1 = 0;
      timer3 = 0;
      timer4 = 0;
      rotate_side = random(1, 3); //рандомная сторона поворота
      if (rotate_side == 1) {
        motor.left();
        delay(random(400, 800));
      } else {
        motor.right();
        delay(random(400, 800));
      }
      moving = 6;
    }

    //движение назад
    if (moving == 3 || timer2 > 2) {
      motor.backward();
      delay(500);
      timer3 = timer3 + 1;
      timer1 = 0;
      timer2 = 0;
      timer4 = 0;
    }

    //поворот
    if (moving == 2 || timer1 > 25 || timer3 > 2 || timer4 > 50) {
      rotate_side = random(1, 3); //рандомная сторона поворота
      if (rotate_side == 1) {
        motor.left();
        delay(random(400, 800));
        }
        else {
        motor.right();
        delay(random(400, 800));
             }

    //движение назад
    if (moving == 3 || timer2 > 2) {
        motor.backward();
        delay(500);
        timer3 = timer3 + 1;
        timer1 = 0;
        timer2 = 0;
        timer4 = 0;
      }
    }

    //ОСТАНОВКА РАБОТЫ ЧЕРЕЗ ЗАДАННОЕ ВРЕМЯ
    if (endtime < millis()) {
      //digitalWrite(relay, LOW);
      motor.stop();
    }
  }
}
