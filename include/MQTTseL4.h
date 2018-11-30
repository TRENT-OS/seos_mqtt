/*
 *  MQTT wrapper
 *
 *  Copyright (C) 2018, Hensoldt Cyber GmbH
 */

#pragma once

#include <stdint.h>


typedef struct {
    uint64_t                    start_ms;
    uint64_t                    timeSpan_ms;
} Timer;


void TimerInit(Timer*);
char TimerIsExpired(Timer*);
void TimerCountdownMS(Timer*, unsigned int);
void TimerCountdown(Timer*, unsigned int);
int TimerLeftMS(Timer*);


typedef struct _Network Network;

typedef int (*mqtt_read_func) (Network*, unsigned char*, int, int);
typedef int (*mqtt_write_func) (Network*, const unsigned char*, int, int);

struct _Network {
    void *ctx;
    mqtt_read_func mqttread;
    mqtt_write_func mqttwrite;
};
