Axio-Builder Mbedtls Client with ESP8266
========================================

[*Fork from SparkFun ESP8266 WiFi Shield Library*](https://github.com/sparkfun/SparkFun_ESP8266_AT_Arduino_Library.git)

Documentation
-------------------
* This code has been tested with the [mbedtls](https://github.com/ARMmbed/mbedtls.git) using the ESP8266 module.
* Please refer to the [user manual page](https://sp-axio.github.io/2017/11/28/How-to-use-mbedtls-with-ESP8266.html) for details.
* Please refer to [developer documentation](https://raw.githubusercontent.com/sp-axio/Mbedtls_ESP8266_for_Axio/master/developer_documentation.md) for API of library.

Prepare
-------------------
* The ESP-01 must be updated to firmware that supports AT command mode.
* Download [mbedtls](https://github.com/ARMmbed/mbedtls.git), compile, and run the 'programs/ssl/ssl_server2' .
* Connect ESP-01 to Axio-Builder with jumper wires as shown below.
![ESP8266](./axio_builder_esp01.png "ESP8266 connect to Axio-Builder")
