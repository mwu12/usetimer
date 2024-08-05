#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>

#define INIT_SEC 3
#define INTV_SEC 3
#define INIT_SEC2 4
#define INTV_SEC2 4

typedef void (*timer_timeout_callback)(union sigval);
static timer_t g_timer2;
static int g_count = 0;
static void TimeoutAction(union sigval)
{
    if (g_count == 0)
        printf("[Thread] Timer expired %d times (%d sec)\n", ++g_count, INIT_SEC2);
    else
        printf("[Thread] Timer expired %d times (%d sec)\n", ++g_count, INTV_SEC2);
    // Add your status and variable checks here
}

void timer_handler(int signum)
{
    static int count = 0;
    struct itimerspec stop_its = {{0,0},{0,0}};
    struct itimerspec refresh_its = {{INTV_SEC2,0},{INIT_SEC2,0}};
    int ret = 0;

    if (g_count == 0)
        printf("Timer expired %d times (%d sec)\n", ++count, INIT_SEC);
    else
        printf("Timer expired %d times (%d sec)\n", ++count, INTV_SEC);

    if (count % 3 == 0)
    {
        ret = timer_settime(g_timer2, 0, &stop_its, NULL);
        if (ret != 0)
        {
            perror("Fail to stop timer2 !!");
            exit(EXIT_FAILURE);
        }
        else
        {
            perror("Succeed to stop timer2 !!");
            //exit(EXIT_FAILURE);
        }
    }
    if (count % 4 == 0)
    {
        ret = timer_settime(g_timer2, 0, &refresh_its, NULL);
        if (ret != 0)
        {
            perror("Fail to refresh timer2 !!");
            exit(EXIT_FAILURE);
        }
        else
        {
            perror("Succeed to refresh timer2 !!");
            //exit(EXIT_FAILURE);
        }
    }

}

int main()
{
    pthread_attr_t attr;
    struct sigaction sa;
    struct sigevent sev;
    struct itimerspec its = {{INTV_SEC,0},{INIT_SEC,0}};
    timer_t timerid;
    timer_timeout_callback t_callback = TimeoutAction;

    // Set up signal handler
    sa.sa_flags = SA_SIGINFO;
    sa.sa_handler = timer_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGRTMIN, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Set up timer 1
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1)
    {
        perror("timer_create SIGEV_SIGNAL");
        exit(EXIT_FAILURE);
    }

    // Set up timer 2
    pthread_attr_init(&attr);
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = t_callback;
    sev.sigev_notify_attributes = &attr;
    if (timer_create(CLOCK_REALTIME, &sev, &g_timer2) == -1)
    {
        perror("timer_create SIGEV_THREAD");
        exit(EXIT_FAILURE);
    }

    // Start the timer
    if (timer_settime(timerid, 0, &its, NULL) == -1)
    {
        perror("timer_settime for timer1");
        exit(EXIT_FAILURE);
    }
    its.it_value.tv_sec = INIT_SEC2;  // Initial expiration
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = INTV_SEC2;  // Interval for periodic timer
    its.it_interval.tv_nsec = 0;
    if (timer_settime(g_timer2, 0, &its, NULL) == -1)
    {
        perror("timer_settime for timer2");
        exit(EXIT_FAILURE);
    }

    // Keep the program running to allow the timer to trigger
    while (1)
    {
        pause();  // Wait for signals
    }

    return 0;
}

