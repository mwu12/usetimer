Common usage:
    struct itimerspec its;

Example:
struct itimerspec its = {{5,0},{1,0}}; // first pair is the interval for periodic timer; second pair is for initial timer.

Requirement before using timer_settime(timerid, 0, &its, NULL);
1. sa   (signal action structure for signal handler)
2. sev  (signal event 'sigevent' structure for timer_create())
3. its  (structure for startup the timer)


gcc -o timer1 main.c
When I press CTL+C on the running timer1, the SIGINT interrupt is sent to timer1.

In sigevent struct, the option SIGEV_THREAD and SIGEV_SIGNAL are provided.

If using SIGEV_SIGNAL, the timer1 program just receives the interrupt without further handling.
If using SIGEV_THREAD, the timer1 can add an local function (callback) to do something.
It allows me to run further process if timer1 timed out...
