## Libraries API documentation
### ESP8266 configuration APIs
* begin(unsigned long baudrate, esp8266_serial_port serialport)
 - brief: initialization of the esp8266.
 - params
   - baudrate: baudrate of serial. If omitted, default is 115200.
   - serialport: device of serial. If omitted, default is uart1(D6,D7) of Axio-builder.
 - return
   -  true: Success
   -  false: Failed
* setMux(uint8_t mux)
 - brief: configure connection mode of esp8266.
 - param
   - mux: 0 for single connection mode, 1 for multiple connection mode.
 - return    
   -  \>0: Success    
   -  <0: Failed
* getMode()
 - brief: get Wi-Fi mode.
 - return
   - 1,2,3: 1=Station mode, 2=AP mode, 3=Station+AP mode
   - \<0: Failed
* setMode(uint8_t mode)
 - brief: set Wi-Fi mode.
 - param
   - mode: 1=station, 2=AP, 3=station+AP
 - return
   - \>0: Success
   - <0: Failed
* status()
 - brief: get status of esp8266.
 - return
   - 0,1: 1=GOT IP/CONNECTED, 0=DISCONNECTED/NOWIFI
   - <0: Failed
* connect(const char *ssid, const char *pwd)
 - brief: connect to AP.
 - params
   - ssid: ssid of AP.
   - pwd: passphrase of AP. can be ommited.
 - return
   - \>0: Success
   - <0: Failed
* getAP(char *ssid)
 - brief: get ssid of the AP.
 - param
   - ssid: buffer to store the ssid of the AP.
 - return
   - 1: Success
   - 0,<0: Failed
* localIP()
 - brief: get IP address of esp8266.
 - return
   - ipaddress: Success
   - 0: Failed

### Mbedtls 설정 APIs
* connect(const char* host, uint16_t port, uint32_t keepalive)
 - brief: attempt to connect to the mbedtls server.
 - param
   - host: hostname of the mbedtls server to connect to.
   - port: port number of the mbedtls server to connect to.
   - keepalive: keep alive
 - return
   - \>0: Success
   - <0: Failed
* setupSSL()
 - brief: initialization of mbedtls.

* connectSSL()
 - brief: attempt to ssl connection to the mbedtls server.
 - return
   - 0: Success
   - \>0: Failed
* writeSSL(char *req, int len)
 - brief: send data to the mbedtls server.
 - param
   - req: request message. If ommited, set to 'GET' request.
   - len: length of req. If ommited, set to length of 'GET' request.
 - return
   - 0: Success
   - \>0: Failed
* readSSL(unsigned char *buf, int len)
 - brief: receive data from the mbedtls server.
 - param
   - buf: buffer to store data received from the mbedtls server.
   - len: length of buffer
 - return
   - \>0: Success
   - <=0: Failed
* connectedSSL()
 - brief: check the connection status with mbedtls server.
 - return
   - 1: connected
   - 0: disconnected
* closeSSL()
 - brief: close ssl connection with mbedtls server.
* stopSSL()
 - brief: terminatie the connection of the esp8266.
* cleanupSSL()
 - brief: release the mbedtls related memory.
