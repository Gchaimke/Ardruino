// ОБЪЯВЛЕНИЕ ПЕРЕМЕННЫХ
// переменные ультразвуковых дальномеров
#include <Ultrasonic.h>
Ultrasonic ultraleft(3, 2);  // (Trig PIN,Echo PIN)
Ultrasonic ultraright(5, 4); // (Trig PIN,Echo PIN)
long cm1, cm2;
// переменные двигателей
int speed1_1 = 6;
int speed1_2 = 9;
int speed2_1 = 10;
int speed2_2 = 11;
int moving = 1;
int rotate = 0;
int rotate_side = 1;
// переменные таймеров анти-залипания
int timer1 = 0;
int timer2 = 0;
int timer3 = 0;
int timer4 = 0;
// переменные вентилятора
int relay = 12;
unsigned long endtime = 600000; //автоматическое выключение через 600 секунд
// SETUP программы (выполняется 1 раз, при старте)
void setup() {
  //назначение портов
  pinMode(speed1_1, OUTPUT);
  pinMode(speed1_2, OUTPUT);
  pinMode(speed2_1, OUTPUT);
  pinMode(speed2_2, OUTPUT);
  pinMode(relay, OUTPUT);
  //генерация рандомного числа со свободной ножки
  randomSeed(analogRead(0));
  //включение реле
  digitalWrite(relay, HIGH);
}

// РАБОЧИЙ ЦИКЛ ПРОГРАММЫ
void loop() {
  //цикл выполняется пока время не достигло endtime
  if (millis() < endtime) {

    //ОПРЕДЕЛЕНИЕ ПРЕПЯТСТВИЙ

    //ПЕРВЫЙ ДАЛЬНОМЕР
    //посылается и принимается звуковой сигнал
    cm1 = ultraleft.Ranging(CM);
    //небольшая задержка, для предотвращения смешения сигналов со вторым дальномером
    delay(50);

    //ВТОРОЙ ДАЛЬНОМЕР
    cm2 = ultraright.Ranging(CM);
    delay(50);

    //ОПРЕДЕЛЕНИЕ НАПРАВЛЕНИЯ ДВИЖЕНИЯ
    //ДВИЖЕНИЕ (min-0 max-255)
    //нет препятствия
    if (cm1 >= 10 && cm2 >= 10) {
      analogWrite(speed1_1, 150);
      analogWrite(speed1_2, 0);
      analogWrite(speed2_1, 150);
      analogWrite(speed2_2, 0);
      delay(100);
      timer1 = timer1 + 1;
      timer2 = 0;
      timer3 = 0;
      timer4 = 0;
    }

    //далеко нет препятствий
    if (cm1 >= 40 && cm2 >= 40) {
      analogWrite(speed1_1, 150);
      analogWrite(speed1_2, 0);
      analogWrite(speed2_1, 150);
      analogWrite(speed2_2, 0);
      delay(100);
      timer4 = timer4 + 1;
      timer1 = 0;
      timer2 = 0;
      timer3 = 0;
    }

    //препятствие (объезд вправо)
    if (cm1 > 40 && cm2 < 30 && moving != 5) {
      analogWrite(speed1_1, 150);
      analogWrite(speed1_2, 0);
      analogWrite(speed2_1, 100);
      analogWrite(speed2_2, 0);
      delay(random(400, 800));
    }

    //препятствие (объезд влево)
    if (cm1 < 30 && cm2 > 40 && moving != 6) {
      analogWrite(speed1_1, 100);
      analogWrite(speed1_2, 0);
      analogWrite(speed2_1, 150);
      analogWrite(speed2_2, 0);
      delay(random(400, 800));
    }

    //препятствие (поворот)
    if (cm1 < 10 || cm2 < 10) {
      analogWrite(speed1_1, 150);
      analogWrite(speed1_2, 0);
      analogWrite(speed2_1, 0);
      analogWrite(speed2_2, 150);
      delay(random(400, 800));
    }
    timer2 = timer2 + 1;
    timer1 = 0;
    timer3 = 0;
    timer4 = 0;
  }

  //препятствие (назад)
  if (cm1 < 5 || cm2 < 5) {
    moving = 3;
  }

  //ДВИЖЕНИЕ (min-0 max-255)
  //поворот
  if (moving == 2 || timer1 > 25 || timer3 > 2 || timer4 > 50) {
    rotate_side = random(1, 3); //рандомная сторона поворота
    if (rotate_side == 1) {
      analogWrite(speed1_1, 0);
      analogWrite(speed1_2, 150);
      analogWrite(speed2_1, 150);
      analogWrite(speed2_2, 0);
      delay(random(400, 800));
    }
    //движение назад
    if (moving == 3 || timer2 > 2) {
      analogWrite(speed1_1, 0);
      analogWrite(speed1_2, 150);
      analogWrite(speed2_1, 0);
      analogWrite(speed2_2, 150);
      delay(500);
      timer3 = timer3 + 1;
      timer1 = 0;
      timer2 = 0;
      timer4 = 0;
    }
  }

  //ОСТАНОВКА РАБОТЫ ЧЕРЕЗ ЗАДАННОЕ ВРЕМЯ
  if (endtime < millis()) {
    digitalWrite(relay, LOW);
    digitalWrite(speed1_1, LOW);
    digitalWrite(speed1_2, LOW);
    digitalWrite(speed2_1, LOW);
    digitalWrite(speed2_2, LOW);
  }

}
