#include <Wire.h>
#include <PID_v1.h>
#include "rgb_lcd.h"

const int sensorPin = A0;
const int ledPin = 3;

float Kp = 0.2;
float Ki = 0.05;
float Kd = 0.06;

double target = 100;

float erroAnterior = 0;
float integral = 0;
double saida;
double leitura;

PID pid(&leitura, &saida, &target, Kp, Ki, Kd, DIRECT);
rgb_lcd lcd;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  pid.SetMode(AUTOMATIC);
  
  lcd.begin(16, 2);
  lcd.setRGB(0, 128, 64); // Escolha uma cor para o backlight
  lcd.print("Iniciando...");
  delay(1000);
  lcd.clear();
}

void loop() {
  leitura = analogRead(sensorPin);
  pid.Compute();
  int pwm = constrain((int)saida, 0, 255);
  analogWrite(ledPin, pwm);

  // Exibição no display LCD
  lcd.setCursor(0, 0);
  lcd.print("Leitura:");
  lcd.print(leitura);
  lcd.print("   "); // Espaço para limpar caracteres extras

  lcd.setCursor(0, 1);
  lcd.print("PWM:");
  lcd.print(pwm);
  lcd.print("        "); // Espaço extra para garantir limpeza

  // Debug serial
  Serial.print("Sensor: ");
  Serial.print(leitura);
  Serial.print(" | PWM: ");
  Serial.println(pwm);
  delay(20);
}
