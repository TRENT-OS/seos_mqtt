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

#include "uart_hdlc.h"
#include "uart_io_guest.h"
#include "uart_stream.h"
#include "uart_fifo.h"

typedef struct Network
{
    UartIoGuest uart;
    UartHdlc    uartHdlc;
    UartFifo    uartFifo;
    UartStream  uartStream;
    int (*mqttread) (struct Network*, unsigned char*, int, int);
    int (*mqttwrite) (struct Network*, unsigned char*, int, int);
}
Network;

#else

typedef struct Network
{
    int my_socket;
    int (*mqttread) (struct Network*, unsigned char*, int, int);
    int (*mqttwrite) (struct Network*, unsigned char*, int, int);
}
Network;

#endif

void NetworkInit(Network*);
int NetworkConnect(Network*, char*, int);
void NetworkDisconnect(Network*);

#endif
