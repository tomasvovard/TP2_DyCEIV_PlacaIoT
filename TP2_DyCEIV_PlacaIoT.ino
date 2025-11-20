/*TP2 DyCE IV - Placa IoT
  Manejo de entradas y salidas digitales por SPI y aplicación en control de nivel de un tanque usando freeRTOS
  En este archivo se inicializa el hardware, se crean las tareas: una para manejo de IOs y otra para la lógica.
  Las tareas se comunican entre sí con variables globales, garantizando integridad mediante el uso de un Mutex.
*/
#include "spi_hardware.h"
#include "test_spi_dio.h"
#include "tank_io.h"
#include "fsm_mode.h"

/*Estrucutras globales de entradas y salidas*/
inputs_t entradas; // se actualiza en taskIO y se lee en taskLogic.
outputs_t salidas; // se actualiza en taskLogic y se aplica al hardware en taskIO.

SemaphoreHandle_t ioMutex = nullptr; //Mutex para acceso concurrente.

void setup(){
  /*Inicializaciones*/
  Serial.begin(115200);
  
  spi_init(); 

  ioMutex = xSemaphoreCreateMutex();

  fsm_mode_init();

  xTaskCreate(taskIO, "Maneja IOs", 4096, NULL, 2, NULL);
  xTaskCreate(taskLogic, "Logica", 4096, NULL, 1, NULL);
}

void loop() {
  //test_sequenceDigitalIO();
}

void taskIO( void *pv){
  /* 
  Período = 50ms
  Lee entradas, toma el mutex, actualiza las salidas y escribe.
  Libera el mutex
  */
  const TickType_t period = pdMS_TO_TICKS(TAKS_IO_PERIOD_MS);

  for(;;){
    inputs_t in;
    outputs_t out;

    io_update_inputs(&in);

    xSemaphoreTake(ioMutex, portMAX_DELAY);
    entradas = in;
    out = salidas;
    xSemaphoreGive(ioMutex);

    io_update_output(&out);

    vTaskDelay(period);
  }
}

void taskLogic(void *pv){
  /*
  Período = 100ms
  Lee entradas con mutex, ejecuta las mef y actualiza las salidas.
  */
  const TickType_t period = pdMS_TO_TICKS(TASK_LG_PERIOD_MS);

  for(;;){
    inputs_t in;
    outputs_t out;

    xSemaphoreTake(ioMutex, portMAX_DELAY);
    in = entradas;
    xSemaphoreGive(ioMutex);

    fsm_mode(&in, &out);

    xSemaphoreTake(ioMutex, portMAX_DELAY);
    salidas = out;
    xSemaphoreGive(ioMutex);

    vTaskDelay(period);
  }
}