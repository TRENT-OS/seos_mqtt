

cmake_minimum_required(VERSION 3.7.2)

project(seos_mqtt C)

set(CMAKE_BUILD_TYPE Debug)

set(PAHO_DIR "paho.mqtt.embedded-c")

add_library(${PROJECT_NAME} STATIC
    ${PAHO_DIR}/MQTTPacket/src/MQTTConnectClient.c
    ${PAHO_DIR}/MQTTPacket/src/MQTTConnectServer.c
    ${PAHO_DIR}/MQTTPacket/src/MQTTDeserializePublish.c
    ${PAHO_DIR}/MQTTPacket/src/MQTTFormat.c
    ${PAHO_DIR}/MQTTPacket/src/MQTTPacket.c
    ${PAHO_DIR}/MQTTPacket/src/MQTTSerializePublish.c
    ${PAHO_DIR}/MQTTPacket/src/MQTTSubscribeClient.c
    ${PAHO_DIR}/MQTTPacket/src/MQTTSubscribeServer.c
    ${PAHO_DIR}/MQTTPacket/src/MQTTUnsubscribeClient.c
    ${PAHO_DIR}/MQTTPacket/src/MQTTUnsubscribeServer.c
    #-----------------------------------------------------------
    ${PAHO_DIR}/MQTTClient-C/src/MQTTClient.c
    #-----------------------------------------------------------
    src/MQTTseL4_timer.c
)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    MQTT_SERVER
    MQTT_CLIENT
    MQTTCLIENT_PLATFORM_HEADER=MQTTseL4.h
    MQTTCLIENT_QOS2=1
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${PAHO_DIR}/MQTTPacket/src
    ${PAHO_DIR}/MQTTClient-C/src
    include
  PRIVATE
    src
)

if (LibLwip)

    target_compile_definitions(${PROJECT_NAME} PRIVATE LWIP=1)
    target_sources(${PROJECT_NAME} PRIVATE src/MQTTseL4_lwip.c)
    target_link_libraries(${PROJECT_NAME} lwip)

elseif (UartSocket)

    target_compile_definitions(${PROJECT_NAME} PRIVATE UART_SOCKET=1)
    target_sources(${PROJECT_NAME} PRIVATE src/MQTTseL4_uart.c)
    target_link_libraries(${PROJECT_NAME} sel4platsupport uart_socket)


else()

    message(FATAL_ERROR "define lower interface: LibLwip, UartSocket")

endif()
