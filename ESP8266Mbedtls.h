#ifndef _ESP8266MBEDTLS_H_
#define _ESP8266MBEDTLS_H_

#include "Arduino.h"
//#include <SoftwareSerial.h>
#include "IPAddress.h"
#include "Client.h"
#include "ESP8266WiFi.h"
#include "ESP8266Client.h"

#include "clp300.h"
#include "spacc.h"
#include "trng.h"

#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/ssl.h"
#include "mbedtls/net.h"
#include "mbedtls/certs.h"

#include "mbedtls/debug.h"
#include "mbedtls/platform.h"
#include "mbedtls/config.h"

#include "ewbm_uart.h"

enum exit_codes{
	exit_ok = 0,
	ctr_drbg_seed_failed,
	ssl_config_defaults_failed,
	ssl_setup_failed,
	hostname_failed,
	socket_failed,
	connect_failed,
	x509_crt_parse_failed,
	ssl_handshake_failed,
	ssl_write_failed,
};

class ESP8266Mbedtls : public ESP8266Client {
public:
	ESP8266Mbedtls();
	ESP8266Mbedtls(uint8_t sock);

	uint8_t setupSSL();
	int connect(const char* host, uint16_t port, uint32_t ka);
	int connectSSL();
				
	virtual size_t write(uint8_t);
	virtual size_t write(const uint8_t *buf, size_t size);
	virtual int read(uint8_t *buf, size_t size);

	int writeSSL();
	int writeSSL(char *req, int len);
	int readSSL();
	int readSSL(unsigned char *buf, int len);
	void stopSSL();
	void closeSSL();
	void cleanupSSL();
	uint8_t connectedSSL();

	virtual uint8_t connected();
	//using Print::write;

private:
	uint16_t  _socket;
	static uint16_t _srcport;
	bool ipMuxEn;
	mbedtls_ssl_context *ssl;
	mbedtls_ssl_config *conf;
	mbedtls_ctr_drbg_context *ctr_drbg;
	mbedtls_entropy_context *entropy;
#if defined(MBEDTLS_X509_CRT_PARSE_C)
	mbedtls_x509_crt *clicert;
	mbedtls_x509_crt *ca;
	mbedtls_pk_context *pkey;
#endif
	uint8_t getFirstSocket();
	int setup_ssl_context(int type);
};
#endif
