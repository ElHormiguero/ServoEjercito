//--------------------------------------------------------------
//-- Oscillator.pde
//-- Generate sinusoidal oscillations in the servos
//--------------------------------------------------------------
//-- (c) Juan Gonzalez-Gomez (Obijuan), Dec 2011
//-- GPL license
//--------------------------------------------------------------
//-- Modification to control the servo with timer 0 and Attiny 85 (pin 0 and 1) by Javier Vargas

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif
#include "OscillatorAttiny.h"

void OscillatorAttiny::pwmMINMAX(int SERVOMIN_, int SERVOMAX_)
{

SERVOMIN = SERVOMIN_;
SERVOMAX = SERVOMAX_;

}

//-- This function returns true if another sample
//-- should be taken (i.e. the TS time has passed since
//-- the last sample was taken
bool OscillatorAttiny::next_sample()
{

  //-- Read current time
  _currentMillis = millis();

  //-- Check if the timeout has passed
  if(_currentMillis - _previousMillis > _TS) {
    _previousMillis = _currentMillis;

    return true;
  }

  return false;
}

//-- Attach an oscillator to a servo
//-- Input: pin is the arduino pin were the servo
//-- is connected
void OscillatorAttiny::attach(int pin, bool rev) {

    //-- Attach the servo and move it to the home position
//      _servo.attach(pin);
//      _servo.write(90);
      ServoDir = pin;
      pinMode(pin, OUTPUT);

      TCCR0A = 2 << COM0A0 | 2 << COM0B0 | 3 << WGM00;
      TCCR0B = 0 << WGM02 | 1 << CS00 | 1 << CS01 | 0 << CS02;

      OCR0A = 0;
      OCR0B = 0;



      SetServo(90);
      //-- Initialization of oscilaltor parameters
      _TS=30;
      _T=2000;
      _N = _T/_TS;
      _inc = 2*M_PI/_N;

      _previousMillis=0;

      //-- Default parameters
      _A=45;
      _phase=0;
      _phase0=0;
      _O=0;
      _stop=false;

      //-- Reverse mode
      _rev = rev;


}

//-- Detach an oscillator from his servo
void OscillatorAttiny::detach()
{
   //-- If the oscillator is attached, detach it.
//  if(_servo.attached())
//        _servo.detach();

}

/*************************************/
/* Set the oscillator period, in ms  */
/*************************************/
void OscillatorAttiny::SetT(unsigned int T)
{
  //-- Assign the new period
  _T=T;

  //-- Recalculate the parameters
  _N = _T/_TS;
  _inc = 2*M_PI/_N;
};

/*******************************/
/* Manual set of the position  */
/******************************/

void OscillatorAttiny::SetPosition(int position)
{
  SetServo(position+_trim);
};


/*******************************************************************/
/* This function should be periodically called                     */
/* in order to maintain the oscillations. It calculates            */
/* if another sample should be taken and position the servo if so  */
/*******************************************************************/
void OscillatorAttiny::refresh()
{

  //-- Only When TS milliseconds have passed, the new sample is obtained
  if (next_sample()) {

      //-- If the oscillator is not stopped, calculate the servo position
      if (!_stop) {
        //-- Sample the sine function and set the servo pos
         _pos = round(_A * sin(_phase + _phase0) + _O);
	       if (_rev) _pos=-_pos;
         SetServo(_pos+90+_trim);
      }

      //-- Increment the phase
      //-- It is always increased, even when the oscillator is stop
      //-- so that the coordination is always kept
      _phase = _phase + _inc;

  }
}

void OscillatorAttiny::SetServo(int angulo) {
  angulo = constrain(angulo, 0, 180);
  if (ServoDir == 0) OCR0A = map(angulo, 0, 180, SERVOMIN, SERVOMAX);
  if (ServoDir == 1) OCR0B = map(angulo, 0, 180, SERVOMIN, SERVOMAX);
}