#include "ESP8266WiFi.h"
#include "Arduino.h"
#include "util/ESP8266_AT.h"
#include "ESP8266Mbedtls.h"

#define GET_REQUEST "GET / HTTP/1.0\r\n\r\n"

extern Uart Serial;
//extern esp8266;

static struct stat_t {
    uint32_t rx;
    uint32_t rxto;
    uint32_t tx;
} stat;


ESP8266Mbedtls::ESP8266Mbedtls()
{
	ssl = (mbedtls_ssl_context *)calloc(1, sizeof(mbedtls_ssl_context));	
    memset(&stat, 0, sizeof(struct stat_t));
}

ESP8266Mbedtls::ESP8266Mbedtls(uint8_t sock)
{
	ssl = (mbedtls_ssl_context *)calloc(1, sizeof(mbedtls_ssl_context));	
	_socket = sock;
    memset(&stat, 0, sizeof(struct stat_t));
}

/*
ESP8266Mbedtls::ESP8266Mbedtls(uint8_t sock)
{
	_socket = sock;
	sock_g = sock;
}
*/

uint8_t ESP8266Mbedtls::setupSSL()
{
	UART_PrintInit(&(Serial.uart));
	mbedtls_platform_set_printf(UART_Printf);
	uint16_t ret; 
    ret = ESAL_PKA_FirmwareInit(PKA_BYTELANE_BIG);
    ESAL_TRNG_Init();
}

uint8_t ESP8266Mbedtls::status()
{
	return esp8266.status();
}
	
const unsigned char ca_cert[] = {
    0x30, 0x82, 0x02, 0x52, 0x30, 0x82, 0x01, 0xd7, 0xa0, 0x03, 0x02, 0x01,
    0x02, 0x02, 0x09, 0x00, 0xc1, 0x43, 0xe2, 0x7e, 0x62, 0x43, 0xcc, 0xe8,
    0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02,
    0x30, 0x3e, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
    0x02, 0x4e, 0x4c, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x0a,
    0x13, 0x08, 0x50, 0x6f, 0x6c, 0x61, 0x72, 0x53, 0x53, 0x4c, 0x31, 0x1c,
    0x30, 0x1a, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x13, 0x50, 0x6f, 0x6c,
    0x61, 0x72, 0x73, 0x73, 0x6c, 0x20, 0x54, 0x65, 0x73, 0x74, 0x20, 0x45,
    0x43, 0x20, 0x43, 0x41, 0x30, 0x1e, 0x17, 0x0d, 0x31, 0x33, 0x30, 0x39,
    0x32, 0x34, 0x31, 0x35, 0x34, 0x39, 0x34, 0x38, 0x5a, 0x17, 0x0d, 0x32,
    0x33, 0x30, 0x39, 0x32, 0x32, 0x31, 0x35, 0x34, 0x39, 0x34, 0x38, 0x5a,
    0x30, 0x3e, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
    0x02, 0x4e, 0x4c, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x0a,
    0x13, 0x08, 0x50, 0x6f, 0x6c, 0x61, 0x72, 0x53, 0x53, 0x4c, 0x31, 0x1c,
    0x30, 0x1a, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x13, 0x50, 0x6f, 0x6c,
    0x61, 0x72, 0x73, 0x73, 0x6c, 0x20, 0x54, 0x65, 0x73, 0x74, 0x20, 0x45,
    0x43, 0x20, 0x43, 0x41, 0x30, 0x76, 0x30, 0x10, 0x06, 0x07, 0x2a, 0x86,
    0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x05, 0x2b, 0x81, 0x04, 0x00, 0x22,
    0x03, 0x62, 0x00, 0x04, 0xc3, 0xda, 0x2b, 0x34, 0x41, 0x37, 0x58, 0x2f,
    0x87, 0x56, 0xfe, 0xfc, 0x89, 0xba, 0x29, 0x43, 0x4b, 0x4e, 0xe0, 0x6e,
    0xc3, 0x0e, 0x57, 0x53, 0x33, 0x39, 0x58, 0xd4, 0x52, 0xb4, 0x91, 0x95,
    0x39, 0x0b, 0x23, 0xdf, 0x5f, 0x17, 0x24, 0x62, 0x48, 0xfc, 0x1a, 0x95,
    0x29, 0xce, 0x2c, 0x2d, 0x87, 0xc2, 0x88, 0x52, 0x80, 0xaf, 0xd6, 0x6a,
    0xab, 0x21, 0xdd, 0xb8, 0xd3, 0x1c, 0x6e, 0x58, 0xb8, 0xca, 0xe8, 0xb2,
    0x69, 0x8e, 0xf3, 0x41, 0xad, 0x29, 0xc3, 0xb4, 0x5f, 0x75, 0xa7, 0x47,
    0x6f, 0xd5, 0x19, 0x29, 0x55, 0x69, 0x9a, 0x53, 0x3b, 0x20, 0xb4, 0x66,
    0x16, 0x60, 0x33, 0x1e, 0xa3, 0x81, 0xa0, 0x30, 0x81, 0x9d, 0x30, 0x1d,
    0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0x9d, 0x6d, 0x20,
    0x24, 0x49, 0x01, 0x3f, 0x2b, 0xcb, 0x78, 0xb5, 0x19, 0xbc, 0x7e, 0x24,
    0xc9, 0xdb, 0xfb, 0x36, 0x7c, 0x30, 0x6e, 0x06, 0x03, 0x55, 0x1d, 0x23,
    0x04, 0x67, 0x30, 0x65, 0x80, 0x14, 0x9d, 0x6d, 0x20, 0x24, 0x49, 0x01,
    0x3f, 0x2b, 0xcb, 0x78, 0xb5, 0x19, 0xbc, 0x7e, 0x24, 0xc9, 0xdb, 0xfb,
    0x36, 0x7c, 0xa1, 0x42, 0xa4, 0x40, 0x30, 0x3e, 0x31, 0x0b, 0x30, 0x09,
    0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x4e, 0x4c, 0x31, 0x11, 0x30,
    0x0f, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x08, 0x50, 0x6f, 0x6c, 0x61,
    0x72, 0x53, 0x53, 0x4c, 0x31, 0x1c, 0x30, 0x1a, 0x06, 0x03, 0x55, 0x04,
    0x03, 0x13, 0x13, 0x50, 0x6f, 0x6c, 0x61, 0x72, 0x73, 0x73, 0x6c, 0x20,
    0x54, 0x65, 0x73, 0x74, 0x20, 0x45, 0x43, 0x20, 0x43, 0x41, 0x82, 0x09,
    0x00, 0xc1, 0x43, 0xe2, 0x7e, 0x62, 0x43, 0xcc, 0xe8, 0x30, 0x0c, 0x06,
    0x03, 0x55, 0x1d, 0x13, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xff, 0x30,
    0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x03,
    0x69, 0x00, 0x30, 0x66, 0x02, 0x31, 0x00, 0xc3, 0xb4, 0x62, 0x73, 0x56,
    0x28, 0x95, 0x00, 0x7d, 0x78, 0x12, 0x26, 0xd2, 0x71, 0x7b, 0x19, 0xf8,
    0x8a, 0x98, 0x3e, 0x92, 0xfe, 0x33, 0x9e, 0xe4, 0x79, 0xd2, 0xfe, 0x7a,
    0xb7, 0x87, 0x74, 0x3c, 0x2b, 0xb8, 0xd7, 0x69, 0x94, 0x0b, 0xa3, 0x67,
    0x77, 0xb8, 0xb3, 0xbe, 0xd1, 0x36, 0x32, 0x02, 0x31, 0x00, 0xfd, 0x67,
    0x9c, 0x94, 0x23, 0x67, 0xc0, 0x56, 0xba, 0x4b, 0x33, 0x15, 0x00, 0xc6,
    0xe3, 0xcc, 0x31, 0x08, 0x2c, 0x9c, 0x8b, 0xda, 0xa9, 0x75, 0x23, 0x2f,
    0xb8, 0x28, 0xe7, 0xf2, 0x9c, 0x14, 0x3a, 0x40, 0x01, 0x5c, 0xaf, 0x0c,
    0xb2, 0xcf, 0x74, 0x7f, 0x30, 0x9f, 0x08, 0x43, 0xad, 0x20,
};

#ifdef MBEDTLS_DEBUG_C
static void my_debug( void *ctx, int level,
		const char *file, int line,
		const char *str )
{
	((void) level);
	((void) ctx);

	char buf[1024];
	sprintf(buf, "%s:%04d: %s", file, line, str ); 
	Serial.println(buf);
	//UART_Printf("%s:%04d: %s", file, line, str );
}
#endif

int ESP8266Mbedtls::setup_ssl_context(int transport_type)
{
	int ret = 0;

	ctr_drbg = (mbedtls_ctr_drbg_context *)calloc(1, sizeof(mbedtls_ctr_drbg_context));
    mbedtls_ctr_drbg_init( ctr_drbg );

#ifdef MBEDTLS_DEBUG_C
	mbedtls_debug_set_threshold(1);
#endif
	MBEDTLS_SSL_DEBUG_MSG(3, ("\nStart setup_ssl_context \n"));
    /*
     * 0. Initialize and setup stuff
     */
    mbedtls_ssl_init( ssl );

	conf = (mbedtls_ssl_config *)calloc(1, sizeof(mbedtls_ssl_config));
    mbedtls_ssl_config_init( conf );
#if defined(MBEDTLS_X509_CRT_PARSE_C)
	ca = (mbedtls_x509_crt *)calloc(1, sizeof(mbedtls_x509_crt));
    mbedtls_x509_crt_init( ca );

	clicert = (mbedtls_x509_crt *)calloc(1, sizeof(mbedtls_x509_crt));
    mbedtls_x509_crt_init( clicert );

	pkey = (mbedtls_pk_context *)calloc(1, sizeof(mbedtls_pk_context));
    mbedtls_pk_init( pkey );
#endif

    MBEDTLS_SSL_DEBUG_MSG(3, ("\nSeeding the random number generator...\n") );

	entropy = (mbedtls_entropy_context *)calloc(1, sizeof(mbedtls_entropy_context));
    mbedtls_entropy_init( entropy );
    if( mbedtls_ctr_drbg_seed( ctr_drbg, mbedtls_entropy_func, entropy,
                       NULL, 0 ) != 0 )
    {
        ret = ctr_drbg_seed_failed;
        goto exit;
    }

    MBEDTLS_SSL_DEBUG_MSG(3, ("\nSetting up the SSL/TLS structure...\n") );
    if( mbedtls_ssl_config_defaults( conf,
                MBEDTLS_SSL_IS_CLIENT,
                transport_type,
                MBEDTLS_SSL_PRESET_DEFAULT ) != 0 )
    {
        ret = ssl_config_defaults_failed;
        goto exit;
    }

    mbedtls_ssl_conf_rng( conf, mbedtls_ctr_drbg_random, ctr_drbg );
#ifdef MBEDTLS_DEBUG_C
	mbedtls_ssl_conf_dbg( conf, my_debug, stdout );
#endif

#if defined(MBEDTLS_KEY_EXCHANGE__SOME__PSK_ENABLED)
    mbedtls_ssl_conf_psk( conf, psk, sizeof( psk ),
                (const unsigned char *) psk_id, sizeof( psk_id ) - 1 );
#endif

#if defined(MBEDTLS_X509_CRT_PARSE_C)
    if( (ret = mbedtls_x509_crt_parse_der( ca, ca_cert, sizeof( ca_cert ) )) != 0 )
    {
        ret = x509_crt_parse_failed;
        goto exit;
    }

    //ret = mbedtls_x509_crt_parse( clicert, (const unsigned char *) mbedtls_test_cas_pem, mbedtls_test_cas_pem_len );
    ret = mbedtls_x509_crt_parse( clicert, (const unsigned char *) mbedtls_test_cli_crt_ec, mbedtls_test_cli_crt_ec_len );
    if( ret != 0 )
    {
        //UART_Printf( " failed\n  !  mbedtls_x509_crt_parse returned -0x%x\n\n", -ret );
        goto exit;
    }

    ret = mbedtls_pk_parse_key( pkey, (const unsigned char *) mbedtls_test_cli_key_ec,
                mbedtls_test_cli_key_ec_len, NULL, 0 );
    if( ret != 0 )
    {
        MBEDTLS_SSL_DEBUG_RET(1, " failed!  mbedtls_pk_parse_key \n", -ret );
        goto exit;
    }

    mbedtls_ssl_conf_ca_chain( conf, ca, NULL );

    if( ( ret = mbedtls_ssl_conf_own_cert( conf, clicert, pkey ) ) != 0 )
    {
        MBEDTLS_SSL_DEBUG_RET(1, " failed! mbedtls_ssl_conf_own_cert ", ret );
        goto exit;
    }

    mbedtls_ssl_conf_authmode( conf, MBEDTLS_SSL_VERIFY_REQUIRED );
#endif

    if( mbedtls_ssl_setup( ssl, conf ) != 0 )
    {
        ret = ssl_setup_failed;
        goto exit;
    }

#if defined(MBEDTLS_X509_CRT_PARSE_C)
#define HOSTNAME "localhost"
    if( mbedtls_ssl_set_hostname( ssl, HOSTNAME ) != 0 )
    {
        ret = hostname_failed;
        goto exit;
    }
#endif

exit:
	return ret;
}

int esp8266_net_send(void *p, const unsigned char *buf, size_t len)
{
	uint16_t *linkID = (uint16_t *)p;
    return esp8266.tcpSend(*linkID, (unsigned char *)buf, len);
}

int esp8266_net_recv(void *p, unsigned char *buf, size_t len)
{
	uint16_t *linkID = (uint16_t *)p;
    return esp8266.copyToBuffer(*linkID, buf, len);
}

int ESP8266Mbedtls::connect(const char* host, uint16_t port, uint32_t keepAlive) 
{
	_socket = getFirstSocket();
	//esp8266.updateStatus();
	
    if (_socket != ESP8266_SOCK_NOT_AVAIL)
    {
		esp8266._state[_socket] = TAKEN;
		int16_t rsp = esp8266.tcpConnect(_socket, host, port, keepAlive);
		return rsp;
	}
	return -1;
}

uint8_t ESP8266Mbedtls::connectedSSL(){
	if(ssl->state == MBEDTLS_SSL_HANDSHAKE_OVER)
		return 0;
	else 
		return 1;
}

int ESP8266Mbedtls::connectSSL()
{
    int ret;
	ret = setup_ssl_context(MBEDTLS_SSL_TRANSPORT_STREAM);
    mbedtls_ssl_set_bio(ssl, (void *)&_socket, esp8266_net_send, esp8266_net_recv, NULL);

    if( mbedtls_ssl_handshake( ssl ) != 0 ) {
        ret = ssl_handshake_failed;
    }
	return ret;
}


int ESP8266Mbedtls::writeSSL(char *req, int len)
{
	int ret ;
	
	MBEDTLS_SSL_DEBUG_MSG(3, ("============= write message begin =============="));
	MBEDTLS_SSL_DEBUG_MSG(3, (req));
	MBEDTLS_SSL_DEBUG_MSG(3, ("============= write message done ==============="));

	while((ret = mbedtls_ssl_write(ssl, (const unsigned char *)req, len)) <= 0){
		if(ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE){
			return ssl_write_failed;
		}
	}
	return exit_ok; 
}

int ESP8266Mbedtls::writeSSL()
{
	int len, ret ;
	char req[256] ;
	
	len = sprintf((char *)req, GET_REQUEST);
	return writeSSL((char *)req, len);
}

int ESP8266Mbedtls::readSSL(unsigned char *buf, int len){
	int ret;

	do{
		ret = mbedtls_ssl_read(ssl, buf, len);
		
		if(ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE)
			continue;
		if(ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY){
			MBEDTLS_SSL_DEBUG_MSG(1, ("error! peer close"));
			break;
		}
		if(ret < 0){
			MBEDTLS_SSL_DEBUG_MSG(1, ("mbedtls_ssl_read failed"));
			break;
		}
		if(ret == 0){
			MBEDTLS_SSL_DEBUG_MSG(1, ("EOF"));
			break;
		}
		
		MBEDTLS_SSL_DEBUG_MSG(3, ("============= received message begin =============="));
		MBEDTLS_SSL_DEBUG_MSG(3, ((char *)buf));
		MBEDTLS_SSL_DEBUG_MSG(3, ("============= received message done ==============="));
		break;
	}while(1);
	return ret;
}

int ESP8266Mbedtls::readSSL(){
	int ret, len;
	unsigned char buf[1024] = {0,};
	len = sizeof(buf) -1;	

	return readSSL(buf, len);
}


int ESP8266Mbedtls::available()
{
	int available = esp8266.available();
	if (available == 0)
	{
		// Delay for the amount of time it'd take to receive one character
		delayMicroseconds((1 / esp8266._baud) * 10 * 1E6);
		// Check again just to be sure:
		available = esp8266.available();
	}
	return esp8266.available();
}

int ESP8266Mbedtls::read()
{
	return esp8266.read();
}

int ESP8266Mbedtls::read(uint8_t *buf, size_t size)
{
	if (esp8266.available() < (int)size)
		return 0;
	
	for (int i=0; i<(int)size; i++)
	{
		buf[i] = esp8266.read();
	}
	
	return 1;
}


int ESP8266Mbedtls::peek()
{
	return esp8266.peek();
}

void ESP8266Mbedtls::flush()
{
	esp8266.flush();
}

void ESP8266Mbedtls::stopSSL()
{
	esp8266.close(_socket);
	esp8266._state[_socket] = AVAILABLE;
}

void ESP8266Mbedtls::closeSSL(){
	mbedtls_ssl_close_notify(ssl);
}

void ESP8266Mbedtls::cleanupSSL()
{
	mbedtls_ssl_free(ssl);
	mbedtls_ssl_config_free(conf);
	mbedtls_ctr_drbg_free(ctr_drbg);
	mbedtls_entropy_free(entropy);
#if defined(MBEDTLS_X509_CRT_PARSE_C)
	mbedtls_x509_crt_free(clicert);
	mbedtls_x509_crt_free(ca);
	mbedtls_pk_free(pkey);
#endif
}

uint8_t ESP8266Mbedtls::getFirstSocket()
{
	/*
	for (int i = 0; i < ESP8266_MAX_SOCK_NUM; i++) 
	{
		if (esp8266._state[i] == AVAILABLE)
		{
			return i;
		}
	}
	return ESP8266_SOCK_NOT_AVAIL;
	*/
	// single connection
	esp8266.updateStatus();
	if(0 == esp8266.getMux()){
		return ESP8266_MAX_SOCK_NUM;
	}
	else{ // multiple connection
		for (int i = 0; i < ESP8266_MAX_SOCK_NUM; i++) 
		{
			if (esp8266._status.ipstatus[i].linkID == 255)
			{
				return i;
			}
		}
		return ESP8266_SOCK_NOT_AVAIL;
	}
	/*
	for (int i = 0; i < ESP8266_MAX_SOCK_NUM; i++) 
	{
		if (esp8266._status.ipstatus[i].linkID == 255)
		{
			return i;
		}
	}
	return ESP8266_SOCK_NOT_AVAIL;
	*/
}
