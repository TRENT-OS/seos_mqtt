#if !defined(__MQTT_SEL4_UART_)
#define __MQTT_SEL4_UART_

#include <stdint.h>
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

void NetworkInit(Network*);
int NetworkConnect(Network*, char*, int);
void NetworkDisconnect(Network*);

#endif
