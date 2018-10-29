# seos\_mqtt

Wrapper layer of paho.mqtt.embedded-c that provides the glue code to have MQTT working either with LWIP or with the uart\_socket interface.

## Getting Started

The project builds a static library of mqtt that can be build including his CMakelists.txt to the your cmake build.

### Dependencies

* lwip

    or

* uart\_socket + seos\_libs
