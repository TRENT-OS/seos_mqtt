

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
    src/MQTTseL4_timer.c
)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    MQTT_SERVER
    MQTT_CLIENT
  PUBLIC
    MQTTCLIENT_PLATFORM_HEADER=MQTTseL4.h
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${PAHO_DIR}/MQTTPacket/src
    include
  PRIVATE
    src
)

target_link_libraries(${PROJECT_NAME}
    sel4platsupport
)
