project (seos_mqtt)

set(PAHO_DIR "paho.mqtt.embedded-c")

include_directories("${PAHO_DIR}/MQTTPacket/src")

file(GLOB SOURCES
        "src/MQTTseL4_timer.c"
        "${PAHO_DIR}/MQTTPacket/src/*.c"
        "${PAHO_DIR}/MQTTClient-C/src/*.c")
if (LibLwip)
    list(APPEND SOURCES "src/MQTTseL4_lwip.c")
elseif (UartSocket)
    list(APPEND SOURCES "src/MQTTseL4_uart.c")
endif()

add_library(seos_mqtt STATIC ${SOURCES})

target_compile_definitions(seos_mqtt PRIVATE
    MQTT_SERVER
    MQTT_CLIENT
    MQTTCLIENT_PLATFORM_HEADER=MQTTseL4.h
    MQTTCLIENT_QOS2=1)

target_include_directories(seos_mqtt PUBLIC
    "include"
    "${PAHO_DIR}/MQTTPacket/src"
    "${PAHO_DIR}/MQTTClient-C/src")

if (LibLwip)
    target_compile_definitions(seos_mqtt PRIVATE
        LWIP=1)
    target_link_libraries( seos_mqtt lwip )
elseif (UartSocket)
    target_compile_definitions(seos_mqtt PRIVATE
        UART_SOCKET=1)
    target_link_libraries( seos_mqtt sel4platsupport uart_socket )
endif()

