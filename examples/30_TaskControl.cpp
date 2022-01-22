#include <Arduino.h>
#include <EventLoop.h>

EVENT_LOOP; // redefine void loop()

int blinkTaskId;

void setup()
{
    static bool state = false;
    pinMode(LED_BUILTIN, OUTPUT);

    // blink task
    blinkTaskId = elp.interval(
        [](int count)
        {
            state = !state;
            digitalWrite(LED_BUILTIN, state);
        },
        500);

    // pause blink after 5s
    elp.timeout(
        []()
        {
            elp.pause(blinkTaskId); // pauses execution of task
        },
        5 * 1000);

    // resume blink after 10s
    elp.timeout(
        []()
        {
            elp.resume(blinkTaskId); // resumes execution of task
        },
        10 * 1000);

    // kills blink after 15s
    elp.timeout(
        []()
        {
            elp.kill(blinkTaskId); // stop and remove task
        },
        15 * 1000);

    elp.pauseAll();  // pauses all tasks
    elp.resumeAll(); // resumes all tasks
}