#ifndef L298NDualMotor_h
#define L298NDualMotor_h

#include "Arduino.h"


typedef void (*CallBackFunction) ();

class L298NDualMotor {
  public:
    typedef enum
    {
      FORWARD  = 0,
      BACKWARD = 1
    } Direction;
    L298NDualMotor(uint8_t pinEnableL, uint8_t pinIN1L, uint8_t pinIN2L, uint8_t pinEnableR, uint8_t pinIN1R, uint8_t pinIN2R);
    void setSpeed(unsigned short pwmVal);
    unsigned short getSpeedL();
    unsigned short getSpeedR();
    void forward();
    void forward(unsigned short pwmVal);
    void forward(unsigned short pwmValL, unsigned short pwmValR);
    void left();
    void right();
    void forwardFor(unsigned long delay, CallBackFunction callback);
    void forwardFor(unsigned long delay);
    void backward();
    void backwardFor(unsigned long delay, CallBackFunction callback);
    void backwardFor(unsigned long delay);
    void run(uint8_t direction);
    void stop();
    void reset();
    boolean isMoving();
  private:
    byte _pinEnableL;
    byte _pinIN1L;
    byte _pinIN2L;
    byte _pwmValL;
    byte _pinEnableR;
    byte _pinIN1R;
    byte _pinIN2R;
    byte _pwmValR;
    unsigned long _lastMs;
    boolean _canMove;
    boolean _isMoving;
    static void fakeCallback();
};

#endif
