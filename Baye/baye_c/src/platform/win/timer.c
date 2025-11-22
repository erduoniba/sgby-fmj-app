#include <timer.h>
#include <windows.h>
#include <stdio.h>

#define IDT_TIMER 100
#define precise 5

typedef struct
{
    char*name;
    void (*callback)();
    int interval;
    int enabled;
    int tick;
} timer_t;

timer_t timer = {"timer"}, timer2 = {"timer2"};

static void run_timer(timer_t *t)
{
    if (t->enabled)
    {
        if (t->tick == 0 && t->callback)
        {
            t->callback();
            t->tick = t->interval;
        }
        t->tick--;
    }
}

void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    run_timer(&timer);
    run_timer(&timer2);
}

void gam_timer_init()
{
}

void winInitTimer()
{
    SetTimer(NULL, IDT_TIMER, precise, (TIMERPROC)TimerProc);
}

void gam_timer_set_callback(void (*cb)())
{
    timer.callback = cb;
}

void gam_timer_open(int interval)
{
    timer.interval = interval;
    timer.tick = interval;
    timer.enabled = 1;
}

void gam_timer_close()
{
    timer.enabled = 0;
}

int gam_timer_interval()
{
    return timer.interval;
}

void gam_timer2_open(int interval, void (*callback)())
{
    timer2.callback = callback;
    timer2.interval = interval;
    timer2.tick = interval;
    timer2.enabled = 1;
}
