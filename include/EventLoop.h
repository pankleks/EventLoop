#include <Arduino.h>

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13__)
#define TINY
#endif

#ifdef TINY
#define LOOP_SLOTS 6
#else
#define LOOP_SLOTS 12
#endif

#define EVENT_LOOP \
    EventLoop elp; \
    void loop() { elp.loop(); }

typedef void handler();
typedef void handler_1(int);

class Event
{
public:
    virtual ~Event()
    {
    }

    virtual int loop() = 0;
};

// always -----------------------------------------------------------------------

class Always : public Event
{
    handler *_fn;

public:
    Always(handler fn)
    {
        _fn = fn;
    }

    int loop()
    {
        _fn();
        return 0;
    }
};

// timeout -----------------------------------------------------------------------

class Timeout : public Event
{
    handler *_fn;
    unsigned long _t;

public:
    Timeout(handler fn, long ms)
    {
        _fn = fn;
        _t = millis() + ms;
    }

    int loop()
    {
        if (millis() >= _t)
        {
            _fn();
            return 1;
        }

        return 0;
    }
};

// interval -----------------------------------------------------------------------

class Interval : public Event
{
    handler_1 *_fn;
    long _ms;
    unsigned long _t;
    bool _immediate;
    int _count;

public:
    Interval(handler_1 fn, long ms, bool immediate = false, int count = -1)
    {
        _fn = fn;
        _ms = ms;
        _immediate = immediate;
        _count = count;
        _t = millis() + _ms;
    }

    int loop()
    {
        if ((_count == -1 || _count > 0) && (_immediate || millis() >= _t))
        {
            _immediate = false;
            if (_count > 0)
                --_count;
            _t = millis() + _ms;
            _fn(_count);
        }

        return 0;
    }
};

// analog input -----------------------------------------------------------------------

class AnalogInput : public Event
{
    handler_1 *_fn;
    uint8_t _pin;
    int _lastValue = -32768;
    int _threshold;

public:
    AnalogInput(handler_1 fn, uint8_t pin, int threshold = 0)
    {
        _fn = fn;
        _pin = pin;
        _threshold = threshold;
        pinMode(_pin, INPUT);
    }

    int loop()
    {
        int value = analogRead(_pin);
        if (abs(value - _lastValue) > _threshold)
        {
            _lastValue = value;
            _fn(value);
        }

        return 0;
    }
};

// state button -----------------------------------------------------------------------

class StateButton : public Event
{
    handler_1 *_fn;
    uint8_t _pin;
    int _lastState = 1;

public:
    StateButton(handler_1 fn, uint8_t pin)
    {
        _fn = fn;
        _pin = pin;
        pinMode(_pin, INPUT_PULLUP);
    }

    int loop()
    {
        int state = digitalRead(_pin);
        if (state != _lastState)
        {
            _lastState = state;
            _fn(state);
        }

        return 0;
    }
};

// push button -----------------------------------------------------------------------

class PushButton : public Event
{
    handler *_fn;
    uint8_t _pin;
    unsigned long _lastMillis;
    unsigned int _debounce;

public:
    PushButton(handler fn, uint8_t pin, unsigned int debounce = 300)
    {
        _fn = fn;
        _pin = pin;
        _debounce = debounce;
        _lastMillis = millis();
        pinMode(_pin, INPUT_PULLUP);
    }

    int loop()
    {
        if (digitalRead(_pin) == 0 && millis() - _lastMillis > _debounce)
        {
            _lastMillis = millis();
            _fn();
        }

        return 0;
    }
};

// loop -----------------------------------------------------------------------

class EventLoop
{
    Event *_events[LOOP_SLOTS];
    bool _held[LOOP_SLOTS];

public:
    EventLoop()
    {
        resumeAll();
    }

    void loop()
    {
        for (int i = 0; i < LOOP_SLOTS; i++)
            if (_events[i] != NULL && !_held[i])
                if (_events[i]->loop() == 1)
                    kill(i);
    }

    int add(Event *event, bool paused = false)
    {
        for (int i = 0; i < LOOP_SLOTS; i++)
            if (_events[i] == NULL)
            {
                _events[i] = event;
                if (paused)
                    pause(i);
                return i;
            }

        return -1;
    }

    void kill(int id)
    {
        if (_events[id] != NULL)
        {
            delete _events[id];
            _events[id] = NULL;
            _held[id] = false;
        }
    }

    void pause(int id)
    {
        _held[id] = true;
    }

    void resume(int id)
    {
        _held[id] = false;
    }

    void pauseAll()
    {
        for (int i = 0; i < LOOP_SLOTS; i++)
            _held[i] = true;
    }

    void resumeAll()
    {
        for (int i = 0; i < LOOP_SLOTS; i++)
            _held[i] = false;
    }

#ifndef TINY
    int always(handler fn, bool paused = false)
    {
        return add(new Always(fn), paused);
    }

    int interval(handler_1 fn, long ms, bool immediate = false, int count = -1, bool paused = false)
    {
        return add(new Interval(fn, ms, immediate, count), paused);
    }

    int timeout(handler fn, long ms, bool paused = false)
    {
        return add(new Timeout(fn, ms), paused);
    }

    int analogInput(handler_1 fn, uint8_t pin, int threshold = 0, bool paused = false)
    {
        return add(new AnalogInput(fn, pin, threshold), paused);
    }

    int stateButton(handler_1 fn, uint8_t pin, bool paused = false)
    {
        return add(new StateButton(fn, pin), paused);
    }

    int pushButton(handler fn, uint8_t pin, unsigned int debounce = 300, bool paused = false)
    {
        return add(new PushButton(fn, pin, debounce), paused);
    }
#endif
};
