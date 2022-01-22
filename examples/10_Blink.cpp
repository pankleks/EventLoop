#include <Arduino.h>
#include <EventLoop.h>

EVENT_LOOP; // redefine void loop()

void setup()
{
    static bool state = false;
    pinMode(LED_BUILTIN, OUTPUT);

    // set interval
    elp.add(new Interval(
        [](int count)
        {
            // blink every 500ms
            state = !state;
            digitalWrite(LED_BUILTIN, state);
        },
        500));
}