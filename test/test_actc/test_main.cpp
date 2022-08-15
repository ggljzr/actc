#include <Arduino.h>
#include <unity.h>

// these includes has to be here
// otherwise the tests won't build correctly
#include <Wire.h>
#include <HID.h>

#include "Command.hpp"

constexpr uint32_t serialInitDelay = 2000;

void testCommand()
{
    uint8_t buffer[5] = {0x01, 0x00, 0x00, 0x00, 0x08};

    actc::Command command;
    command.readFromBuffer(buffer);

    TEST_ASSERT_EQUAL(0x01, command.getCommandId());
    TEST_ASSERT_EQUAL(0x08, command.getUIntData());
}

void setup()
{
    delay(serialInitDelay);
    UNITY_BEGIN();

    RUN_TEST(testCommand);

    UNITY_END();
}

void loop() {}