#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

void timer_handler(int signum)
{
    static int count = 0;
    printf("Timer expired %d times\n", ++count);
    // Add your status and variable checks here
}

int main()
{
    struct sigaction sa;
    struct sigevent sev;
    struct itimerspec its = {{10,0},{3,0}};
    timer_t timerid;

    // Set up signal handler
    sa.sa_flags = SA_SIGINFO;
    sa.sa_handler = timer_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGRTMIN, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Set up timer
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1)
    {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    // Start the timer
    //its.it_value.tv_sec = 10;  // Initial expiration
    //its.it_value.tv_nsec = 0;
    //its.it_interval.tv_sec = 1;  // Interval for periodic timer
    //its.it_interval.tv_nsec = 0;
    if (timer_settime(timerid, 0, &its, NULL) == -1)
    {
        perror("timer_settime");
        exit(EXIT_FAILURE);
    }

    // Keep the program running to allow the timer to trigger
    while (1)
    {
        pause();  // Wait for signals
    }

    return 0;
}

