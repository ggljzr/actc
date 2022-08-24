#include <Arduino_FreeRTOS.h>
#include "Tasks.hpp"

void setup()
{
    xTaskCreate(actcTasks::encoderTask, "encoderTask", 128, NULL, 1, NULL);

    // Note that serialTask will become unstable at about 96 bytes of stack depth
    // and I experienced some crashesh/freezing with 128 bytes of stack depth
    xTaskCreate(actcTasks::serialTask, "serialTask", 256, NULL, 2, NULL);
}

void loop()
{
}