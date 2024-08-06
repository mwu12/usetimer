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

2024-8-5
Creating two timers (timer1 and timer2).
Defining each timer's initial time seconds and interval time seconds.
timer2 is created by using SIGEV_THREAD and assigned with callback function.
I would see:
1. timer1 stops timer2 if timer2 was up and running;
2. timer1 stops timer2 if timer2 was not up yet;
3. after timer2 was stopped by timer1, timer1 then refreshs timer2 again;

Observation:
If use SIGEV_SIGNAL, the timer function will be based on the assigment of sa_handler.
    sa.sa_handler = timer_handler;

If use SIGEV_THREAD, the timer function (callback function) will be based on the assignment of sev.sigev_notify_function.
    sev.sigev_notify_function = t_callback;
In my example, t_callback is pointing to function TimeoutAction.
    timer_timeout_callback t_callback = TimeoutAction;

TODO:
The callback function has standard parameter sigval, what sigval data structure content will give in run-time?
    static void TimeoutAction(union sigval)