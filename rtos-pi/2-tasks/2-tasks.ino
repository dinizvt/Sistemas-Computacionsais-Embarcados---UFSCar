#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

// Variáveis globais
volatile unsigned long totalPontos = 0;
volatile unsigned long dentroCirculo = 0;

// Função para gerar número aleatório entre 0 e 1
float randFloat() {
  return (float)random(0, 10001) / 10000.0;
}

// Tarefa para estimar pi
void TaskCalculoPi(void *pvParameters) {
  (void) pvParameters;

  for (;;) {
    float x = randFloat();
    float y = randFloat();
    float distancia = x * x + y * y;
    totalPontos++;
    if (distancia <= 1.0) {
      dentroCirculo++;
    }
  }
}

// Tarefa para imprimir estimativa de pi
void TaskPrintPi(void *pvParameters) {
  (void) pvParameters;

  for (;;) {
    unsigned long total, dentro;
    float pi;
    total = totalPontos;
    dentro = dentroCirculo;
    if (total > 0) {
      pi = 4.0 * ((float)dentro / (float)total);
      Serial.print("Pontos: ");
      Serial.print(total);
      Serial.print(" Pi: ");
      Serial.println(pi, 6);
    }

    vTaskDelay(200 / portTICK_PERIOD_MS); // Atualiza a cada 2 segundos
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Inicializa o gerador de números aleatórios
  randomSeed(analogRead(0));

  // Cria tarefas
  xTaskCreate(TaskCalculoPi, "CalcPi", 128, NULL, 1, NULL);
  xTaskCreate(TaskPrintPi,   "PrintPi", 128, NULL, 1, NULL);
}

void loop() {
  // Não usado com FreeRTOS
}
