file(GLOB SOURCES "*.c")

add_library(
  paho-embed-mqtt3cc STATIC
  ${SOURCES}
)

target_compile_definitions(paho-embed-mqtt3cc PRIVATE
             MQTTCLIENT_PLATFORM_HEADER=MQTTseL4.h MQTTCLIENT_QOS2=1)

target_include_directories(paho-embed-mqtt3cc PUBLIC seL4)
target_include_directories(paho-embed-mqtt3cc PUBLIC .)

target_link_libraries( paho-embed-mqtt3cc muslc )

if (LibLwip)
    target_compile_definitions(paho-embed-mqtt3cc PRIVATE
        LWIP=1)
    target_link_libraries( paho-embed-mqtt3cc lwip )
elseif (UartSocket)
    target_compile_definitions(paho-embed-mqtt3cc PRIVATE
        UART_SOCKET=1)
    target_link_libraries( paho-embed-mqtt3cc sel4platsupport uart_socket)
endif()

