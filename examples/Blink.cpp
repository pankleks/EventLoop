#include <Arduino.h>
#include <EventLoop.h>

EVENT_LOOP; // this exchanges your void loop()

void setup()
{
    static bool state = false;
    pinMode(LED_BUILTIN, OUTPUT);

    // add interval to event loop
    elp.add(new Interval(
        [](int count)
        {
            state = !state;
            digitalWrite(LED_BUILTIN, state);
        },
        500));
}