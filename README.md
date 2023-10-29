# Loop event handler

Exchange your `void loop()` with event loop and switch to non-blocking event driven programming style.

See examples for more.

## Tested on

AVR/AVR TINY/STM32/ESP32/RPI PICO

## History
- 4/18/2021
  - add: pause and resume
- 6/26/2021
  - add: EVENT_LOOP define
- 6/28/2021
  - add: immediate flag to interval
  - add: start as paused in add method
- 1/13/2022 (1.0.4)
  - add: interval max count
- 1/23/2022 (1.1.0)
  - add: shorthand methods
  - add: attiny detection to minimalize memory usage
- 10/14/2022 (1.2.0)
  - add: full lambda (except AVR) #1
- 12/12/2022 (1.3.0)
  - add: reset count flag added to interval
- 2/9/2023 (1.3.1)
  - fix: kill, pause, resume crash when negative ids were passed
- 10/29/2023 (1.3.2)
  - add: RPI PICO -> add EVENT_LOOP1 for processing on 2nd core