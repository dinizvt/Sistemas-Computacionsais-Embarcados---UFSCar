#include <Wire.h>
#include "rgb_lcd.h"

const int sensorPin = A0;
const int ledPin = 3;

float Kp = 0.2;
float Ki = 0.05;
float Kd = 0.06;

int target = 100;

float erroAnterior = 0;
float integral = 0;

rgb_lcd lcd;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setRGB(0, 128, 64); // Escolha uma cor para o backlight
  lcd.print("Iniciando...");
  delay(1000);
  lcd.clear();
}

void loop() {
  int leitura = analogRead(sensorPin);
  float erro = target - leitura;

  // PID
  integral += erro;
  float derivada = erro - erroAnterior;
  float saida = Kp * erro + Ki * integral + Kd * derivada;

  erroAnterior = erro;

  // Converte para PWM válido (0 a 255)
  int pwm = constrain((int)saida, 0, 255);
  analogWrite(ledPin, pwm);

  // Exibição no display LCD
  lcd.setCursor(0, 0);
  lcd.print("Leitura:");
  lcd.print(leitura);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("PWM:");
  lcd.print(pwm);
  lcd.print("        "); 

  // Debug serial
  Serial.print("Sensor: ");
  Serial.print(leitura);
  Serial.print(" | PWM: ");
  Serial.println(pwm);
  delay(20);
}
