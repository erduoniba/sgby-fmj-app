//
//  timer.c
//  baye-ios
//
//  Created by loong on 15/8/15.
//
//
#include <stdio.h>
#include <emscripten.h>
#include "../common/timer.h"
#include "../../baye/speed.h"

#define NULL ((void*)0)
#define ratio 10.0

typedef void(*timer_cb)(void);

typedef struct {
    int started;
    int active;
    int interval;
    timer_cb cb;
} timer_t;

static timer_t timers[2] = {
    {0, 0, 1, NULL},
    {0, 0, 1, NULL},
};

static void schedule_timer(timer_t* timer);

void _timer_cb(void*t) {
    timer_t* timer = (timer_t*)t;
    if (timer->active && timer->cb) {
        timer->cb();
    }
    schedule_timer(timer);
}

static void schedule_timer(timer_t* timer) {
    /* 使用速度控制调整时间间隔 */
    int adjustedInterval = bayeGetAdjustedInterval(timer->interval * ratio);
    emscripten_async_call(_timer_cb, timer, adjustedInterval);
}

int gam_timer_interval(U8 n)
{
    return timers[n].interval;
}

void gam_timer_set_interval(U8 n, int interval) {
    timers[n].interval = interval;
}

void gam_timer_init()
{
}

void gam_timer_set_callback(U8 n, void(*cb)(void))
{
    timers[n].cb = cb;
}

static void _timer_open(timer_t* timer) {
    timer->active = 1;
    if (!timer->started) {
        timer->started = 1;
        schedule_timer(timer);
    }
}

int gam_timer_open(U8 n, int interval)
{
    int prev = timers[n].active ? timers[n].interval : 0;
    if (interval == 0) {
        gam_timer_close(n);
    } else {
        timers[n].interval = interval;
        _timer_open(&timers[n]);
    }
    return prev;
}

void gam_timer_close(U8 n)
{
    timers[n].active = 0;
}

U8 gam_check_timer_on(U8 n) {
    return timers[n].active;
}

static timer_t timer2 = {
    0, 0, 1, NULL
};

void gam_timer2_open(int interval, void(*callback)())
{
    timer2.interval = interval;
    timer2.cb = callback;
    _timer_open(&timer2);
}