#if !defined(__MQTT_SEL4_)
#define __MQTT_SEL4_

#include <stdint.h>

typedef struct Timer
{
    void*           impl;
    uint64_t        startNS;
    unsigned        timeSpanMS;
}
Timer;

void TimerInit(Timer*);
char TimerIsExpired(Timer*);
void TimerCountdownMS(Timer*, unsigned int);
void TimerCountdown(Timer*, unsigned int);
int TimerLeftMS(Timer*);

#ifdef UART_SOCKET
#   include "MQTTseL4_uart.h"
#else

typedef struct Network
{
    int my_socket;
    int (*mqttread) (struct Network*, unsigned char*, int, int);
    int (*mqttwrite) (struct Network*, unsigned char*, int, int);
} Network;

void NetworkInit(Network*);
int NetworkConnect(Network*, char*, int);
void NetworkDisconnect(Network*);

#endif

#endif

