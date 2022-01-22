#include <Arduino.h>
#include <EventLoop.h>

EVENT_LOOP; // redefine void loop()

void setup()
{
    elp.always(
        []()
        {
            // executed with every loop iteration
        });

    elp.timeout(
        []()
        {
            // executed once after 1000ms
        },
        1000);

    elp.interval(
        [](int count)
        {
            // executed every 1000ms
        },
        1000);

    elp.interval(
        [](int count)
        {
            // executed 10 times, every 1000ms
            // count value will be 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 then stops
        },
        1000, false /* if true, first execution will be immediate */, 10 /* 10 times only */);

    elp.analogInput(
        [](int value /* new value */)
        {
            // executed when A0 value changed by +/- 10
        },
        A0 /* analog pin to monitor */, 10 /* threshold */);

    elp.pushButton(
        []()
        {
            // executed when button was pressed
        },
        2 /* button pin */, 500 /* 500ms debounce */);

    elp.stateButton(
        [](int state /* new state */)
        {
            // executed when button changed state (low -> high or high -> low)
        },
        2 /* button pin */);
}