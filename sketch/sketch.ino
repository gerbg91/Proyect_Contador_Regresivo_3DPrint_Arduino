#include <Config.h>
#include <EasyBuzzer.h>
#include <TM1637Display.h>

long tiempoContadorConstante = 10;
long tiempoContador = tiempoContadorConstante;
long tiempoIntervalo = 1000;
long tiempoAnterior = 0;
long tiempoActual = 0;
long tiempoMaximo = 25;
boolean iniciaPartida = false;
int valuePulsador = 0;
int frecuenciaHercios = 2000;

const int pulsador_verde = 11;
const int pulsador_amarillo = 10;
const byte inputBuzzer = 5;



#define CLK 3
#define DIO 4

// Crea objeto display
TM1637Display display = TM1637Display(CLK, DIO);

void setup() {
  // Realiza el brillo
  display.setBrightness(5);
  pinMode(pulsador_verde, INPUT_PULLUP);
  pinMode(pulsador_amarillo, INPUT_PULLUP);
  muestraTiempo(tiempoContadorConstante);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(pulsador_verde) == LOW) {
    delay(100);
    while (digitalRead(pulsador_verde) == LOW) {}
    iniciaPartida = true;
    tiempoContador = tiempoContadorConstante;
  }


  if (digitalRead(pulsador_amarillo) == LOW && iniciaPartida == false) {
    delay(100);
    while (digitalRead(pulsador_amarillo) == LOW) {}
    if (tiempoContadorConstante < tiempoMaximo) {
      tiempoContadorConstante = tiempoContadorConstante + 5;
    }
    muestraTiempo(tiempoContadorConstante);
  }

  if (tiempoContador >= 0 && iniciaPartida) {
    tiempoActual = millis();
    if ((tiempoActual - tiempoAnterior) >= tiempoIntervalo) {
      muestraTiempo(tiempoContador);
      tiempoContador--;
      tiempoAnterior = tiempoActual;
    }
  }
}

void muestraTiempo(int tiempoContador) {
  display.showNumberDec(tiempoContador);
  if (tiempoContador == 0) {
    tiempoContador = tiempoContadorConstante;
    muestraTiempo(tiempoContador);
    for (int i = 0; i <= 3; i++) {
      tone(inputBuzzer, frecuenciaHercios);
      delay(500);
      noTone(inputBuzzer);
      delay(800);
    }
    iniciaPartida = false;
  }
}