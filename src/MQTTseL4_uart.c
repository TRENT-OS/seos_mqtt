#include <stdio.h>
#include <assert.h>
#include "MQTTseL4_uart.h"

///TODO this should be somewhere else or the check in uart_stream.c should be removed 'cause the size is dynamic
#define CAPACITY (4096*16)

static int seL4_read(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
    return 0;
}

static int seL4_write(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
    return 0;
}

void NetworkInit(Network* n)
{
    bool ok = false;

    n->mqttread     = seL4_read;
    n->mqttwrite    = seL4_write;

    UartIoGuestInit(&n->uart, true, NULL);
    UartHdlcInit(&n->uartHdlc, &n->uart.implementation);
    ok = UartFifoInit(&n->uartFifo, CAPACITY);
    assert(ok);
    UartStreamInit(&n->uartStream, &n->uartHdlc, (IUartFifo *) &n->uartFifo);
}

int NetworkConnect(Network* n, char* addr, int port)
{
    return UartStreamOpen(&n->uartStream);
}

void NetworkDisconnect(Network* n)
{
    UartStreamClose(&n->uartStream);
}
