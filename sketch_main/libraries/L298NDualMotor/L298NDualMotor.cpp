/*
  Autor:   Chaim Gorbov
*/

#include "L298NDualMotor.h"

typedef void (*CallBackFunction) ();

L298NDualMotor::L298NDualMotor(uint8_t pinEnableL, uint8_t pinIN1L, uint8_t pinIN2L, uint8_t pinEnableR, uint8_t pinIN1R, uint8_t pinIN2R) {
  _pinEnableL = pinEnableL;
  _pinIN1L = pinIN1L;
  _pinIN2L = pinIN2L;
  _pinEnableR = pinEnableR;
  _pinIN1R = pinIN1R;
  _pinIN2R = pinIN2R;
  _pwmValL = 70;
  _pwmValR = 70;
  _isMoving = false;
  _canMove = true;
  _lastMs = 0;

  pinMode(_pinEnableL, OUTPUT);
  pinMode(_pinIN1L, OUTPUT);
  pinMode(_pinIN2L, OUTPUT);
  pinMode(_pinEnableR, OUTPUT);
  pinMode(_pinIN1R, OUTPUT);
  pinMode(_pinIN2R, OUTPUT);
}


void L298NDualMotor::setSpeed(unsigned short pwmVal) {
  _pwmValL = pwmVal;
  _pwmValR = pwmVal;
}

unsigned short L298NDualMotor::getSpeedL() {
  return _pwmValL;
}
unsigned short L298NDualMotor::getSpeedR() {
  return _pwmValR;
}

void L298NDualMotor::forward() {
  digitalWrite(_pinIN1L, HIGH);
  digitalWrite(_pinIN2L, LOW);
  digitalWrite(_pinIN1R, HIGH);
  digitalWrite(_pinIN2R, LOW);

  analogWrite(_pinEnableL, _pwmValL);
  analogWrite(_pinEnableR, _pwmValR);

  _isMoving = true;
}

void L298NDualMotor::forward(unsigned short pwmVal) {
  digitalWrite(_pinIN1L, HIGH);
  digitalWrite(_pinIN2L, LOW);
  digitalWrite(_pinIN1R, HIGH);
  digitalWrite(_pinIN2R, LOW);

  analogWrite(_pinEnableL, pwmVal);
  analogWrite(_pinEnableR, pwmVal);

  _isMoving = true;
}
void L298NDualMotor::forward(unsigned short pwmValL, unsigned short pwmValR) {
  digitalWrite(_pinIN1L, HIGH);
  digitalWrite(_pinIN2L, LOW);
  digitalWrite(_pinIN1R, HIGH);
  digitalWrite(_pinIN2R, LOW);

  analogWrite(_pinEnableL, pwmValL);
  analogWrite(_pinEnableR, pwmValR);

  _isMoving = true;
}


void L298NDualMotor::left() {
  digitalWrite(_pinIN1L, LOW);
  digitalWrite(_pinIN2L, HIGH);
  digitalWrite(_pinIN1R, HIGH);
  digitalWrite(_pinIN2R, LOW);

  analogWrite(_pinEnableL, _pwmValL);
  analogWrite(_pinEnableR, _pwmValR);

  _isMoving = true;
}
void L298NDualMotor::right() {
  digitalWrite(_pinIN1L, HIGH);
  digitalWrite(_pinIN2L, LOW);
  digitalWrite(_pinIN1R, LOW);
  digitalWrite(_pinIN2R, HIGH);

  analogWrite(_pinEnableL, _pwmValL);
  analogWrite(_pinEnableR, _pwmValR);

  _isMoving = true;
}

void L298NDualMotor::forwardFor(unsigned long delay, CallBackFunction callback) {
  if ((_lastMs == 0) && _canMove) {
    _lastMs = millis();
    this->forward();

  }

  if (((millis() - _lastMs) > delay) && _canMove) {
    this->stop();
    _lastMs = 0;
    _canMove = false;

    callback();

  }
}

void L298NDualMotor::forwardFor(unsigned long delay) {
  this->forwardFor(delay, fakeCallback);
}


void L298NDualMotor::backward() {
  digitalWrite(_pinIN1L, LOW);
  digitalWrite(_pinIN2L, HIGH);
  digitalWrite(_pinIN1R, LOW);
  digitalWrite(_pinIN2R, HIGH);

  analogWrite(_pinEnableL, _pwmValL);
  analogWrite(_pinEnableR, _pwmValR);

  _isMoving = true;
}

void L298NDualMotor::backward(unsigned short pwmValL, unsigned short pwmValR) {
  digitalWrite(_pinIN1L, LOW);
  digitalWrite(_pinIN2L, HIGH);
  digitalWrite(_pinIN1R, LOW);
  digitalWrite(_pinIN2R, HIGH);

  analogWrite(_pinEnableL, _pwmValL);
  analogWrite(_pinEnableR, _pwmValR);

  _isMoving = true;
}

void L298NDualMotor::backwardFor(unsigned long delay, CallBackFunction callback) {

  if ((_lastMs == 0) && _canMove) {
    _lastMs = millis();
    this->backward();
  }

  if (((millis() - _lastMs) > delay) && _canMove) {
    this->stop();
    _lastMs = 0;
    _canMove = false;

    callback();

  }
}

void L298NDualMotor::backwardFor(unsigned long delay) {
  this->backwardFor(delay, fakeCallback);
}

void L298NDualMotor::run(uint8_t direction) {
  switch (direction) {
    case BACKWARD:
      this->backward();
      break;
    case FORWARD:
      this->forward();
      break;
  }
}

void L298NDualMotor::stop() {
  digitalWrite(_pinIN1L, LOW);
  digitalWrite(_pinIN2L, LOW);
  digitalWrite(_pinIN1R, LOW);
  digitalWrite(_pinIN2R, LOW);

  analogWrite(_pinEnableL, 255);
  analogWrite(_pinEnableR, 255);

  _isMoving = false;
}

void L298NDualMotor::reset() {
  _canMove = true;
}

boolean L298NDualMotor::isMoving() {
  return _isMoving;
}

void L298NDualMotor::fakeCallback() {

}