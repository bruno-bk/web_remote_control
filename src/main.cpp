#include <Arduino.h>

int IN1 = 26;
int IN2 = 27;
int velocidadeA = 25;

int IN3 = 14;
int IN4 = 12;
int velocidadeB = 13;

int velocidade = 0;

void setup() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(velocidadeA, OUTPUT);
    pinMode(velocidadeB, OUTPUT);

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void loop() {
    while (velocidade < 100) {
        analogWrite(velocidadeA, velocidade);
        analogWrite(velocidadeB, velocidade);
        velocidade = velocidade + 2;
        delay(200);
    }

    while (velocidade > 50) {
        analogWrite(velocidadeA, velocidade);
        analogWrite(velocidadeB, velocidade);
        velocidade = velocidade - 2;
        delay(200);
    }
}
