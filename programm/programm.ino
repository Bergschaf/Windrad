#include <Servo.h>

// Servo an 11 angeschlossen
// LEDs an 10 angeschlossen
// Zähler an 9 angeschlossen
// Taster an 8
const int servoPort = 11;
const int ledPort = 10;
const int zaehlerPort = 9;
const int tasterPort = 8;

const int servo_bremsen = -100; 
const int servo_release = 100; 

long long last_count = 0;

int drehzahl[100];  // Wie oft dreht sich das Windrad alle 100ms
int drehzahl_min;

Servo servo;

void setup() {
  servo.attach(servoPort);
  pinMode(zaehlerPort, INPUT);
  Serial.begin(9600);
}

void shiftRight() {
  // Schiebt die Werte im Array um eine Stelle nach rechts
  for (int i = 100; i > 0; i--) {
    drehzahl[i] = drehzahl[i - 1];
  }
  drehzahl[0] = 0;
}

void bremsen() {
  servo.write(servo_bremsen);
}

void bremse_release() {
  servo.write(servo_release);
}

void loop() {
  if (last_count + 100 < millis()) {
    shiftRight();
    last_count += 100;
    // Die Werte werden alle 100ms um eine Stelle nach rechts verschoben
  }

  if (digitalRead(zaehlerPort)) {
    drehzahl[0]++;
    while (digitalRead(zaehlerPort)) {}
  }

  // Die Drehzahl wird in Umdrehungen pro Minute umgerechnet
  drehzahl_min = 0;
  for (int i = 0; i < 100; i++) {
    drehzahl_min += drehzahl[0];
  }
  drehzahl_min *= 6;


  Serial.println("Drehzahl: " + String(drehzahl_min) + " Umdrehungen/min");

  // Wenn der Taster gedrückt wird, wird die Bremse aktiviert
  if (digitalRead(tasterPort)){
    bremsen();
  }
  else{
    bremse_release();
  }
}