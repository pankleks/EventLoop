#include <Arduino.h>
#include <EventLoop.h>

EVENT_LOOP; // redefine void loop()

void setup()
{
#ifdef __AVR__
    pinMode(LED_BUILTIN, OUTPUT);

    bool state = false;
    // set interval
    elp.interval(
        [&state](int count)
        {
            // blink every 500ms
            state = !state;
            digitalWrite(LED_BUILTIN, state);
        },
        500);
#endif
}