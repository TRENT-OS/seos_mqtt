project (seos_mqtt)

set(CMAKE_BUILD_TYPE Debug)

message(STATUS "CMake version: " ${CMAKE_VERSION})
message(STATUS "CMake system name: " ${CMAKE_SYSTEM_NAME})

set(PAHO_DIR "paho.mqtt.embedded-c")
set(CMAKE_SCRIPTS "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

## build settings
set(PAHO_VERSION_MAJOR 1)
set(PAHO_VERSION_MINOR 0)
set(PAHO_VERSION_PATCH 0)
set(CLIENT_VERSION ${PAHO_VERSION_MAJOR}.${PAHO_VERSION_MINOR}.${PAHO_VERSION_PATCH})

string(TIMESTAMP BUILD_TIMESTAMP UTC)
message(STATUS "Timestamp is ${BUILD_TIMESTAMP}")

set(CPACK_PACKAGE_VERSION_MAJOR ${PAHO_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PAHO_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PAHO_VERSION_PATCH})
include(CPack)

include_directories("${PAHO_DIR}/MQTTPacket/src")

file(GLOB SOURCES
        "src/*.c"
        "${PAHO_DIR}/MQTTPacket/src/*.c"
        "${PAHO_DIR}/MQTTClient-C/src/*.c")

add_library(seos_mqtt STATIC ${SOURCES})

target_compile_definitions(seos_mqtt PRIVATE
    MQTT_SERVER
    MQTT_CLIENT
    MQTTCLIENT_PLATFORM_HEADER=MQTTseL4.h
    MQTTCLIENT_QOS2=1)

target_include_directories(seos_mqtt PUBLIC
    "src"
    "${PAHO_DIR}/MQTTPacket/src"
    "${PAHO_DIR}/MQTTClient-C/src")

if (LibLwip)
    target_compile_definitions(seos_mqtt PRIVATE
        LWIP=1)
    target_link_libraries( seos_mqtt lwip )

    list(REMOVE_ITEM SOURCES "src/MQTTseL4_uart.c")
elseif (UartSocket)
    target_compile_definitions(seos_mqtt PRIVATE
        UART_SOCKET=1)
    target_link_libraries( seos_mqtt sel4platsupport uart_socket )

    list(REMOVE_ITEM SOURCES "src/MQTTseL4_network.c")
endif()
