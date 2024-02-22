/*
 * Glue layer for PAHO MQTT
 *
 * Copyright (C) 2018-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

#include "MQTTseL4.h"
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>


// somebody must implement this function and provide a timestmap in ms
// resolution and ms accuracy. For now this is good enough that everybody is
// just polling the timer and for every call to a blocking function the
// TimerLeftMS() is used. One day we may get a use case where somebody wants
// to wait on the timer and we need an implementation that support it better
// than just polling.
extern uint64_t glue_tls_mqtt_getTimeMs(void);


//------------------------------------------------------------------------------
static uint64_t get_elapsed_ms(Timer* timer)
{
    uint64_t timestamp_ms = glue_tls_mqtt_getTimeMs();
    uint64_t start_ms = timer->start_ms;

    // A 64-bit timer counting ms will never roll over in real life, assuming
    // the counter starts at 0 every time and not at a random value. Even a
    // with us resolution thing would be very safe. A ns counter will a roll
    // over after 584,9 years, even that might not be worth starting to
    // consider potential issues.
    // There is more than one situation, where the current time stamp and the
    // start time stamp are equal:
    //   - we have a roll over. It's quite unlikely we hit this very moment
    //   - the underlying timer is not running or broken and we don't see time
    //       progressing.
    //   - timer does not increment in 1s but jumps in 10s or 100s. That is a
    //       bad thing, as we can't accurately see time progressing.
    //   - less than one ms has passed. It's quite likely on a fast CPU that
    //       executed a lot if instructions in one ms. It's less likely
    //       for higher counter resolutions.
    // Considering all these cases, returning 0 as the elapsed time span seems
    // a good choice. The creator of a timer should do some sanity test on
    // startup to ensure that is work well.

    if (timestamp_ms < start_ms)
    {
        return UINT64_MAX - start_ms + timestamp_ms;
    }

    return timestamp_ms - start_ms;
}


//------------------------------------------------------------------------------
static void set_countdown_ms(Timer* timer, uint64_t timeout_ms)
{
    timer->start_ms     = glue_tls_mqtt_getTimeMs();
    timer->timeSpan_ms  = timeout_ms;
}


//------------------------------------------------------------------------------
static uint64_t get_time_left_ms(Timer* timer)
{
    uint64_t elapsed_ms = get_elapsed_ms(timer);
    uint64_t timeSpan_ms = timer->timeSpan_ms;

    if (timeSpan_ms < elapsed_ms)
    {
        return 0; // no time left
    }

    return timeSpan_ms - elapsed_ms;
}


//------------------------------------------------------------------------------
void TimerInit(Timer* timer)
{
    memset(timer, 0, sizeof(Timer));

    // ensure the timer has expired
    set_countdown_ms(timer, 0);
}


//------------------------------------------------------------------------------
void TimerCountdownMS(Timer* timer, unsigned int timeout_ms)
{
    set_countdown_ms(timer, timeout_ms);
}


//------------------------------------------------------------------------------
void TimerCountdown(Timer* timer, unsigned int timeout_s)
{
    // must use the proper 64-bit multiplication here to avoid accidental
    // integer truncation.
    uint64_t timeout_ms = (uint64_t)(1000) * timeout_s;

    set_countdown_ms(timer, timeout_ms);
}


//------------------------------------------------------------------------------
int TimerLeftMS(Timer* timer)
{
    uint64_t time_left_ms = get_time_left_ms(timer);

    // even if this functon returns an int, it seem it's not intended that
    // negative values are returned if the timer as expired. Furthermore, we
    // have to ensure that we return values in the right range. Our internal
    // reprensentation is uint64_t and thing might get truncated.
    // if we don't have limits.h, this can be used
    //   int max_int = (int)( ((unsigned int )(-1)) / 2);
    return (time_left_ms > INT_MAX) ? INT_MAX : time_left_ms;
}


//------------------------------------------------------------------------------
char TimerIsExpired(Timer* timer)
{
    return (0 == get_time_left_ms(timer));
}
