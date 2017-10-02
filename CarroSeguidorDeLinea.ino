#include "Arduino.h"
// PINES DE MOTOR SHIELD
int PIN_PWM_A = 3;
int PIN_PWM_B = 11;
int PIN_DIR_A = 12;
int PIN_DIR_B = 13;
int PIN_BRAKE_A = 9;
int PIN_BRAKE_B = 8;

// PINES DE SENSORES PARA SEGUIDOR DE LINEA
int PIN_SENSOR1 = 2;
int PIN_SENSOR2 = 4;
int PIN_SENSOR3 = 5;
int PIN_SENSOR4 = 6;

// CONSTANTES PARA VELOCIDAD DEL CARRO
int PWM_AVANCE = 255;
int PWM_GIRO = 255;

// VARIABLES DE INSTANCIA CON LOS VALORES DE LOS SENSORES
boolean sL;
boolean sLL;
boolean sR;
boolean sRR;

boolean hintIzquierda;
boolean hintDerecha;

void setup() {
  // ESTABLECER PINES DE SALIDA (AL MOTOR)
  pinMode(PIN_PWM_A, OUTPUT);
  pinMode(PIN_PWM_B, OUTPUT);
  pinMode(PIN_DIR_A, OUTPUT);
  pinMode(PIN_DIR_B, OUTPUT);
  pinMode(PIN_BRAKE_A, OUTPUT);
  pinMode(PIN_BRAKE_B, OUTPUT);

  // ESTABLECER PINES DE ENTRADA (DESDE LOS SENSORES)
  pinMode(PIN_SENSOR1, INPUT);
  pinMode(PIN_SENSOR2, INPUT);
  pinMode(PIN_SENSOR3, INPUT);
  pinMode(PIN_SENSOR4, INPUT);

  Serial.begin(9600);
}

void loop() {
  leerSensores();
  seguirLinea();
  //avanzarDerecho();
}

void leerSensores() {
  sLL = digitalRead(PIN_SENSOR4) == HIGH;
  sL = digitalRead(PIN_SENSOR3) == HIGH;
  sR = digitalRead(PIN_SENSOR2) == HIGH;
  sRR = digitalRead(PIN_SENSOR1) == HIGH;
}

void seguirLinea() {
  algoritmoLineaSimple();
}

void algoritmoLineaSimple() {
  if (!hintDerecha && !hintIzquierda) {
    if (!sL && !sR && !sLL && !sRR) {
      avanzarDerecho();
    } else if (sR || sRR) {
      rotarDerecha();
    } else if (sL || sLL) {
      rotarIzquierda();
    } else {
      avanzarDerecho();
    }
  } else {
    if (hintDerecha) {
      rotarDerecha();
    } else if (hintIzquierda) {
      rotarIzquierda();
    }
  }

  if (sRR && !hintDerecha && !hintIzquierda) {
    hintDerecha = true;
  } else if (sLL && !hintDerecha && !hintIzquierda) {
    hintIzquierda = true;
  }

  if (hintIzquierda && sR) {
    hintIzquierda = false;
    hintDerecha = false;
  }
  if (hintDerecha && sL) {
    hintIzquierda = false;
    hintDerecha = false;
  }

}

void rotarIzquierda() {
  digitalWrite(PIN_DIR_A, HIGH);
  digitalWrite(PIN_BRAKE_A, LOW);
  analogWrite(PIN_PWM_A, PWM_GIRO);

  digitalWrite(PIN_DIR_B, HIGH);
  digitalWrite(PIN_BRAKE_B, LOW);
  analogWrite(PIN_PWM_B, 0);
}

void rotarDerecha() {
  digitalWrite(PIN_DIR_A, HIGH);
  digitalWrite(PIN_BRAKE_A, LOW);
  analogWrite(PIN_PWM_A, 0);

  digitalWrite(PIN_DIR_B, HIGH);
  digitalWrite(PIN_BRAKE_B, LOW);
  analogWrite(PIN_PWM_B, PWM_GIRO);
}

void avanzarDerecho() {
  digitalWrite(PIN_DIR_A, HIGH);
  digitalWrite(PIN_BRAKE_A, LOW);
  analogWrite(PIN_PWM_A, PWM_AVANCE);

  digitalWrite(PIN_DIR_B, HIGH);
  digitalWrite(PIN_BRAKE_B, LOW);
  analogWrite(PIN_PWM_B, PWM_AVANCE);
}

void detenerse() {
  digitalWrite(PIN_DIR_A, HIGH);
  analogWrite(PIN_PWM_A, 0);
  digitalWrite(PIN_BRAKE_A, HIGH);

  digitalWrite(PIN_DIR_B, HIGH);
  analogWrite(PIN_PWM_B, 0);
  digitalWrite(PIN_BRAKE_B, HIGH);
}
