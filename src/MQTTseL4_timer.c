#include "MQTTseL4.h"
#include <sel4platsupport/timer.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>

extern seL4_timer_t* get_mqtt_timer();

//------------------------------------------------------------------------------
void TimerInit(Timer* timer)
{
    seL4_timer_t *sysTimer = get_mqtt_timer();
    assert(sysTimer);

    // ToDo: support more than one timer

    memset(timer, 0, sizeof(Timer));
    timer->impl = sysTimer;

    return;
}

//------------------------------------------------------------------------------
static uint64_t NowNS(Timer const* timer)
{
    seL4_timer_t *sysTimer = (seL4_timer_t *)(timer->impl);
    assert(sysTimer);

    uint64_t nowNS = 0;
    bool ok = !ltimer_get_time(&(sysTimer->ltimer), &nowNS);
    assert(ok);

    return nowNS;
}


//------------------------------------------------------------------------------
static unsigned ElapsedMS(Timer* timer)
{
    uint64_t timeElapsedMS = (NowNS(timer) - timer->startNS) / 1000;

    return (timeElapsedMS < UINT_MAX) ?
        (unsigned) timeElapsedMS : UINT_MAX;
}


//------------------------------------------------------------------------------
void TimerCountdownMS(Timer* timer, unsigned int timeout)
{
    timer->startNS      = NowNS(timer);
    timer->timeSpanMS   = timeout;
}


//------------------------------------------------------------------------------
void TimerCountdown(Timer* timer, unsigned int timeout)
{
    timer->startNS      = NowNS(timer);
    timer->timeSpanMS   = timeout * 1000;
}


//------------------------------------------------------------------------------
int TimerLeftMS(Timer* timer)
{
    return timer->timeSpanMS - ElapsedMS(timer);
}


//------------------------------------------------------------------------------
char TimerIsExpired(Timer* timer)
{
    return ElapsedMS(timer) > timer->timeSpanMS;
}
