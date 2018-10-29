/*
 *  MQTT wrapper
 *
 *  Copyright (C) 2018, Hensoldt Cyber GmbH
 */

#pragma once

#include <stdint.h>

typedef struct {
    void*           impl;
    uint64_t        startNS;
    unsigned        timeSpanMS;
} Timer;

void TimerInit(Timer*);
char TimerIsExpired(Timer*);
void TimerCountdownMS(Timer*, unsigned int);
void TimerCountdown(Timer*, unsigned int);
int TimerLeftMS(Timer*);


typedef struct _Network {
    void *ctx;
    int (*mqttread) (struct _Network*, unsigned char*, int, int);
    int (*mqttwrite) (struct _Network*, unsigned char*, int, int);
} Network;
