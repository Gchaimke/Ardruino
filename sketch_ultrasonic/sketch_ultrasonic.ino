// ОБЪЯВЛЕНИЕ ПЕРЕМЕННЫХ
// переменные ультразвуковых дальномеров
#include <Ultrasonic.h>
Ultrasonic ultraleft(3, 2);  // (Trig PIN,Echo PIN)
Ultrasonic ultraright(11, 4); // (Trig PIN,Echo PIN)

long cm1, cm2;
// переменные двигателей
int speed1_1 = 9;
int speed1_2 = 8;
int speed2_1 = 7;
int speed2_2 = 6;
int ENA = 10;
int ENB = 5;
int moving = 1;
int rotate = 0;
int rotate_side = 1;
const int SPEED = 60;
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
  pinMode (ENA, OUTPUT);
  pinMode(speed2_1, OUTPUT);
  pinMode(speed2_2, OUTPUT);
  pinMode (ENB, OUTPUT);
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
      wheelsMove(HIGH, HIGH);
      delay(100);
      timer1 = timer1 + 1;
      timer2 = 0;
      timer3 = 0;
      timer4 = 0;
    }

    //далеко нет препятствий
    if (cm1 >= 40 && cm2 >= 40) {
      wheelsMove(HIGH, HIGH);
      delay(100);
      timer4 = timer4 + 1;
      timer1 = 0;
      timer2 = 0;
      timer3 = 0;
    }

    //препятствие (объезд вправо)
    if (cm1 > 40 && cm2 < 30 && moving != 5) {
      wheelsMove(HIGH, HIGH);
      delay(random(400, 800));
    }

    //препятствие (объезд влево)
    if (cm1 < 30 && cm2 > 40 && moving != 6) {
      wheelsMove(HIGH, HIGH);
      delay(random(400, 800));
    }

    //препятствие (поворот)
    if (cm1 < 10 || cm2 < 10) {
    wheelsMove(HIGH, 0);
    delay(random(400, 800));
    timer2 = timer2 + 1;
    timer1 = 0;
    timer3 = 0;
    timer4 = 0;
  }

  //препятствие (назад)
  if (cm1 <= 5 || cm2 <= 5) {
    wheelsMove(LOW, LOW);
    delay(random(400, 800));
    timer2 = timer2 + 1;
    timer1 = 0;
    timer3 = 0;
    timer4 = 0;
    rotate_side = random(1, 3); //рандомная сторона поворота
    if (rotate_side == 1) {
      wheelsMove(0, HIGH);
      delay(random(400, 800));
    }
    //движение назад
    if (moving == 3 || timer2 > 2) {
      wheelsMove(HIGH, LOW);
      delay(500);
      timer3 = timer3 + 1;
      timer1 = 0;
      timer2 = 0;
      timer4 = 0;
  }}

  //ДВИЖЕНИЕ (min-0 max-255)
  //поворот
  if (moving == 2 || timer1 > 25 || timer3 > 2 || timer4 > 50) {
    rotate_side = random(1, 3); //рандомная сторона поворота
    if (rotate_side == 1) {
      wheelsMove(LOW, HIGH);
      delay(random(400, 800));
    }
    //движение назад
    if (moving == 3 || timer2 > 2) {
[      wheelsMove(HIGH, LOW);
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
}

void wheelsMove(String a, String b) {
  // Иди вперед
  if (a > 0 && b > 0) {    
    digitalWrite(speed1_1, a);
    digitalWrite(speed1_2, 0);
    digitalWrite(speed2_1, b);
    digitalWrite(speed2_2, 0);
    analogWrite(ENA, 55);
    analogWrite(ENB, 55);
 } 
 // Поверни на вправо
  else if(a > 0 && b == 0){
    digitalWrite(speed1_1, a);
    digitalWrite(speed1_2, 0);
    digitalWrite(speed2_1, 0);
    digitalWrite(speed2_2, 0);
    analogWrite(ENA, 55);
    analogWrite(ENB, 55);
  }
  // Поверни на лево
  else if(a == 0 && b > 0){
    digitalWrite(speed1_1, 0);
    digitalWrite(speed1_2, 0);
    digitalWrite(speed2_1, b);
    digitalWrite(speed2_2, 0);
    analogWrite(ENA, 55);
    analogWrite(ENB, 55);
  }
  // Иди назад
  else{
    digitalWrite(speed1_1, 0);
    digitalWrite(speed1_2, a);
    digitalWrite(speed2_1, 0);
    digitalWrite(speed2_2, b);
    analogWrite(ENA, 55);
    analogWrite(ENB, 55);
  }
}
