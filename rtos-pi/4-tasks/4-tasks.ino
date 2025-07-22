#include <Arduino_FreeRTOS.h>
#include <task.h>

// Variáveis globais
volatile unsigned long totalPontos   = 0;
volatile unsigned long dentroCirculo = 0;
volatile unsigned long countCalc1    = 0;
volatile unsigned long countCalc2    = 0;
volatile unsigned long countCalc3    = 0;

float randFloat() {
  return (float)random(0, 10001) / 10000.0f;
}

void TaskCalculoPi(void *pvParameters) {
  volatile unsigned long *pCount = (volatile unsigned long*)pvParameters;
  for (;;) {
    float x = randFloat();
    float y = randFloat();
    float distancia = x*x + y*y;

    // Incrementa o contador desta instância
    (*pCount)++;

    // Atualiza contadores compartilhados
    taskENTER_CRITICAL();
      totalPontos++;
      if (distancia <= 1.0f) {
        dentroCirculo++;
      }
    taskEXIT_CRITICAL();
  }
}

void TaskPrintPi(void *pvParameters) {
  (void)pvParameters;
  for (;;) {
    unsigned long tP = totalPontos;
    unsigned long dC = dentroCirculo;
    unsigned long c1 = countCalc1;
    unsigned long c2 = countCalc2;
    unsigned long c3 = countCalc3;
    float pi = 0, p1 = 0, p2 = 0, p3 = 0;

    if (tP > 0) {
      pi = 4.0f * ((float)dC / (float)tP);
      p1 = ((float)c1 / (float)tP) * 100.0f;
      p2 = ((float)c2 / (float)tP) * 100.0f;
      p3 = ((float)c3 / (float)tP) * 100.0f;
    }

    Serial.print("C1: "); Serial.print(c1);
    Serial.print(" ("); Serial.print(p1,1); Serial.print("%)  ");
    Serial.print("C2: "); Serial.print(c2);
    Serial.print(" ("); Serial.print(p2,1); Serial.print("%)  ");
    Serial.print("C3: "); Serial.print(c3);
    Serial.print(" ("); Serial.print(p3,1); Serial.print("%)  ");
    Serial.print("Total: "); Serial.print(tP);
    Serial.print("  Pi: "); Serial.println(pi, 6);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  randomSeed(analogRead(0));

  xTaskCreate(TaskCalculoPi, "Calc1", 128, (void*)&countCalc1, 1, NULL);
  xTaskCreate(TaskCalculoPi, "Calc2", 128, (void*)&countCalc2, 1, NULL);
  xTaskCreate(TaskCalculoPi, "Calc3", 128, (void*)&countCalc3, 1, NULL);
  xTaskCreate(TaskPrintPi,   "PrintPi", 128, NULL, 1, NULL);
}

void loop() {
  // Não usado com FreeRTOS
}
