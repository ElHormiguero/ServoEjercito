/*Minirobots programados sobre un Attiny 85 con un zumbador, sensor inflarrojo, 2 servomotores y un led.
  Autor: Javier Vargas. El Hormiguero 26/04/2018.
  https://creativecommons.org/licenses/by/4.0/
*/

#define v 4 //Velocidad del robot entre 1 y 5

//PINES
#define pinServoD 0
#define pinServoA 1
#define pinLed 2
#define pinSensor 3
#define pinZumbador 4

//CONFIGURACION
#define LedTime 5 //Periodo de parpadeo del led en el modo 0 (s)
#define BeepTime 100 //Duracion de un beep del zumbador y el led en (ms). Delay en el programa!
#define TimeGiro 5 //Tiempo que el robot gira al detectar un objeto (s)
#define TimeBailar 3 //Tiempo que el robot baila
#define VelMin 1500 //Periodo a la velocidad minima (ms)
#define VelMax 500  //Periodo a la velocidad maxima (ms)

//#define debug_Servos

#include <OscillatorAttiny.h> //Salida pwm controlando el Timer0 de ATtiny85 en los pines 0 y 1
OscillatorAttiny servoD;
OscillatorAttiny servoA;
#define PWMmin 10 //Posicion a 0º 
#define PWMmax 40 //Posicion a 180º 

unsigned long m0 = 0; //Contador
unsigned long m1 = 0; //Contador
unsigned long m2 = 0; //Contador
boolean led = 0; //Estado del led
int modo = 0; //Diferentes estados del robot
int velocidad = VelMin;

void setup() {

  //PinMode
  pinMode(pinServoA, OUTPUT);
  pinMode(pinServoD, OUTPUT);
  pinMode(pinLed, OUTPUT);
  pinMode(pinZumbador, OUTPUT);
  pinMode(pinSensor, INPUT);

  //Servomotores
  servoD.pwmMINMAX(PWMmin, PWMmax);
  servoA.pwmMINMAX(PWMmin, PWMmax);
  servoD.attach(pinServoD);
  servoA.attach(pinServoA);

  //Semilla aleatoria
  randomSeed(micros());

  //Inicio
  Home();
  velocidad = map(v, 1, 5, VelMin, VelMax);
  LedBeep(v); //El led parpadea segun el modo en el que se encuentre 1 - 5
  delay(1000);
  LedBeep(v);
  delay(1000);
  LedBeep(v + 1);
  delay(1000);
}

//////////        //////////
//////////  LOOP  //////////
//////////        //////////

void loop() {
#ifdef debug_Servos
  for (int i = PWMmin; i < PWMmax; ++i) {
    if (i == PWMmin) digitalWrite(pinLed, HIGH);
    else digitalWrite(pinLed, LOW);
    OCR0A = i;
    OCR0B = i;
    delay(100);
  }
  for (int i = PWMmax; i > PWMmin; --i) {
    OCR0A = i;
    OCR0B = i;
    delay(100);
  }
#else

  //MODO 0: normal, no objeto detectado, avanzar
  if (modo == 0) {
    Avanzar(1);
    ParpadeoLed(LedTime * 1000);
    if (ObjetoDetectado()) modo = 1; //Entrar en MODO 1
    else if (m2 != millis() / 10000) {
      m2 = millis() / 10000;
      if (random(0, 7) == 0) modo = 2; //Entrar en MODO 2 (Aleatorio)
    }
  }

  //MODO 1: objeto detectado, girar a la derecha o la izquierda
  if (modo == 1) {
    randomSeed(millis()); //Nueva semilla de numeros aleatorios
    ZumbadorLedBeep(1);
    GirarIzquierda(TimeGiro * 1000);
    if (ObjetoDetectado()) modo = 2;//Entrar en MODO 2
    else modo = 0; //Volver a MODO 0
  }

  //MODO 2: BAILAR
  if (modo == 2) {
    ZumbadorLedBeep(3);
    digitalWrite(pinLed, HIGH);
    Bailar(TimeBailar * 1000);
    digitalWrite(pinLed, LOW);
    modo = 0;
  }

#endif
}

/////////////////////////////////////

boolean ObjetoDetectado() {
  if (digitalRead(pinSensor)) return 0;
  else return 1;
}

void Bailar(int t) {
  servoA.Reset();
  servoD.Reset();
  ServoDelante(0, 30, 200, 0);
  ServoAtras(0, 10, 200, 180);
  unsigned long tf = millis() + t;
  while (millis() < tf) {
    servoA.refresh();
    servoD.refresh();
  }
}

void GirarIzquierda(int t) {
  servoA.Reset();
  servoD.Reset();
  ServoDelante(30, 30, velocidad, 90);
  ServoAtras(0, 10, velocidad, 0);
  unsigned long tf = millis() + t;
  while (millis() < tf) {
    servoA.refresh();
    servoD.refresh();
  }
}

void Avanzar(int t) {
  ServoDelante(0, 45, velocidad, 0);
  ServoAtras(0, 10, velocidad, 90);
  unsigned long tf = millis() + t;
  while (millis() < tf) {
    servoA.refresh();
    servoD.refresh();
  }
}

void ServoDelante(int O, int A, int T, int PH) {
  servoD.SetO(O); //Offset
  servoD.SetA(A); //Amplitud
  servoD.SetT(T); //Periodo
  servoD.SetPh(PH); //Fase
}

void ServoAtras(int O, int A, int T, int PH) {
  servoA.SetO(O); //Offset
  servoA.SetA(A); //Amplitud
  servoA.SetT(T); //Periodo
  servoA.SetPh(PH); //Fase
}

void Home() {
  servoA.SetPosition(90);
  servoD.SetPosition(90);
}

void ParpadeoLed(int T) {
  if (!led) {
    if (m0 != millis() / T) {
      m0 = millis() / T;
      m1 = millis() + BeepTime;
      digitalWrite(pinLed, HIGH);
      led = 1;
    }
  }
  else {
    if (millis() >= m1) {
      digitalWrite(pinLed, LOW);
      led = 0;
    }
  }
}

void ZumbadorBeep(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(pinZumbador, HIGH);
    delay(BeepTime);
    digitalWrite(pinZumbador, LOW);
    if (i != n - 1) delay(BeepTime);
  }
}

void LedBeep(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(pinLed, HIGH);
    delay(BeepTime);
    digitalWrite(pinLed, LOW);
    if (i != n - 1) delay(BeepTime);
  }
}

void ZumbadorLedBeep(int n) {
  for (int i = 0; i < n; i++) {
    digitalWrite(pinLed, HIGH);
    digitalWrite(pinZumbador, HIGH);
    delay(BeepTime);
    digitalWrite(pinLed, LOW);
    digitalWrite(pinZumbador, LOW);
    if (i != n - 1) delay(BeepTime);
  }
}

