#pragma once

#include <Arduino.h>

/**
 * @file Tasks.h
 *
 * This file contains task definitions for FreeRTOS. This is not a part of
 * the actc library in /lib/ folder because I want it to be independent of FreeRTOS.
 *
 * Tasks are defined separately just to keep main.src tidy.
 */

namespace actcTasks
{
    /**
     * Initializes the Keyboard HID ands starts sending keypresses
     * based on encoder reading.
     */
    void encoderTask(void *pvParameters);

    /**
     * Initializes display and serial interface, reads commands from serial
     * port and displays results.
     */
    void serialTask(void *pvParameters);

    /// Keeps given key pressed for given delay, then releases it.
    void shortPress(uint8_t key, unsigned long releaseDelay = 50);
}