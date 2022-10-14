# Loop event handler

Exchange your `void loop()` with event loop and switch to non-blocking event driven programming style.

See examples for more.

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
- 10/14/2022 (1.2.2)
  - add: full lambda (except AVR) #1