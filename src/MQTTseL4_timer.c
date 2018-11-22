#include "MQTTseL4.h"
#include <sel4platsupport/timer.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>

extern seL4_timer_t* mqtt_sel4timer;

static uint64_t NowNS(Timer const* timer)
{
    uint64_t nowNS = 0;
    seL4_timer_t *seL4Timer = timer->impl;

    bool ok = !ltimer_get_time(&seL4Timer->ltimer, &nowNS);
    assert(ok);

    return nowNS;
}

static unsigned ElapsedMS(Timer* timer)
{
    uint64_t timeElapsedMS = (NowNS(timer) - timer->startNS) / 1000000;

    return (timeElapsedMS < UINT_MAX) ?
        (unsigned) timeElapsedMS : UINT_MAX;
}

void TimerInit(Timer* timer)
{
    memset(timer, 0, sizeof(Timer));
    timer->impl = mqtt_sel4timer;
    return;
}

char TimerIsExpired(Timer* timer)
{
    return ElapsedMS(timer) > timer->timeSpanMS;
}

void TimerCountdownMS(Timer* timer, unsigned int timeout)
{
    timer->startNS      = NowNS(timer);
    timer->timeSpanMS   = timeout;
}

void TimerCountdown(Timer* timer, unsigned int timeout)
{
    timer->startNS      = NowNS(timer);
    timer->timeSpanMS   = timeout * 1000;
}

int TimerLeftMS(Timer* timer)
{
    return timer->timeSpanMS - ElapsedMS(timer);
}
