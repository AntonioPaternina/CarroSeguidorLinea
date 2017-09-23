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
int PWM_AVANCE = 100;
int PWM_GIRO = 100;

// VARIABLES DE INSTANCIA CON LOS VALORES DE LOS SENSORES
bool sL;
bool sLL;
bool sR;
bool sRR;

// VARIABLES DE INSTANCIA DE CONTROL DE EJECUCIÓN DEL PROGRAMA
bool cruzandoIzquierda;
bool cruzandoDerecha;
bool cruzandoDerecho;
int contadorRetardo;

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
}

void leerSensores() {
	sLL = digitalRead(PIN_SENSOR1);
	sL = digitalRead(PIN_SENSOR2);
	sR = digitalRead(PIN_SENSOR3);
	sRR = digitalRead(PIN_SENSOR4);
}

void seguirLinea() {
	if (!sLL && !sRR) { // si los sensores laterales no detectan señal
		algoritmoLineaSimple();
	} else if ((sLL || sRR) // si alguno de los sensores laterales detecta señal es porque se trata de un giro L, T o +
			&& ((cruzandoIzquierda || cruzandoDerecha || cruzandoDerecho)
					|| contadorRetardo++ > 3)) { // esto es para dar tiempo a todos los sensores de encenderse en el caso de ángulos difíciles
		algoritmoGiro90Grados();
	}

	if (!sLL && !sRR && !sL && !sR) {
		resetearFlagsControl();
	}
}

void algoritmoLineaSimple() {
	if (!sL && !sR) {
		avanzarDerecho();
	} else if (sR) {
		girarDerecha();
	} else if (sL) {
		girarIzquierda();
	}
}

void algoritmoGiro90Grados() {
	if (sL && sLL && !sR && !sRR && !cruzandoDerecho && !cruzandoDerecha
			&& !cruzandoIzquierda) { // caso de giro en L hacia la izquierda
		cruzandoIzquierda = true;
	} else if (sR && sRR && !sL && !sLL && !cruzandoDerecho && !cruzandoDerecha
			&& !cruzandoIzquierda) { // caso de giro en L hacia la derecha
		cruzandoDerecha = true;
	} else if (sR && sRR && sL && sLL && !cruzandoDerecho && !cruzandoDerecha
			&& !cruzandoIzquierda) { // caso de T o +
		int decision = random(2);

		if (decision == 0) {
			cruzandoIzquierda = true;
		} else if (decision == 1) {
			cruzandoDerecha = true;
		} else if (decision == 2) {
			cruzandoDerecho = true;
		}
	}

	if (cruzandoIzquierda) {
		rotarIzquierda();
	} else if (cruzandoDerecha) {
		rotarDerecha();
	} else if (cruzandoDerecho) {
		//avanzarDerecho();
		rotarDerecha();
	}
}

void rotarIzquierda() {
	digitalWrite(PIN_DIR_A, LOW);
	digitalWrite(PIN_BRAKE_A, LOW);
	analogWrite(PIN_PWM_A, PWM_GIRO);

	digitalWrite(PIN_DIR_B, HIGH);
	digitalWrite(PIN_BRAKE_B, LOW);
	analogWrite(PIN_PWM_B, PWM_GIRO);
}

void rotarDerecha() {
	digitalWrite(PIN_DIR_A, HIGH);
	digitalWrite(PIN_BRAKE_A, LOW);
	analogWrite(PIN_PWM_A, PWM_GIRO);

	digitalWrite(PIN_DIR_B, LOW);
	digitalWrite(PIN_BRAKE_B, LOW);
	analogWrite(PIN_PWM_B, PWM_GIRO);
}

void girarIzquierda() {
	digitalWrite(PIN_DIR_A, HIGH);
	digitalWrite(PIN_BRAKE_A, LOW);
	analogWrite(PIN_PWM_A, (PWM_GIRO / 4));

	digitalWrite(PIN_DIR_B, HIGH);
	digitalWrite(PIN_BRAKE_B, LOW);
	analogWrite(PIN_PWM_B, PWM_GIRO);
}

void girarDerecha() {
	digitalWrite(PIN_DIR_A, HIGH);
	digitalWrite(PIN_BRAKE_A, LOW);
	analogWrite(PIN_PWM_A, PWM_GIRO);

	digitalWrite(PIN_DIR_B, HIGH);
	digitalWrite(PIN_BRAKE_B, LOW);
	analogWrite(PIN_PWM_B, (PWM_GIRO / 4));
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

void resetearFlagsControl() {
	contadorRetardo = 0;
	cruzandoIzquierda = false;
	cruzandoDerecha = false;
	cruzandoDerecho = false;
}

