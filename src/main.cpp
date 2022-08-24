#include <Arduino_FreeRTOS.h>
#include "Tasks.hpp"

void setup()
{
    xTaskCreate(actcTasks::encoderTask, "encoderTask", 128, NULL, 1, NULL);
    xTaskCreate(actcTasks::serialTask, "serialTask", 256, NULL, 2, NULL);
}

void loop()
{
}