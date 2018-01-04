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


/*
const char mbedtls_test_cli_crt_ec[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIIDZTCCAk2gAwIBAgIJAOGkrFczx+WVMA0GCSqGSIb3DQEBCwUAMEkxCzAJBgNV\r\n"
"BAYTAktSMQ4wDAYDVQQIDAVzZW91bDEOMAwGA1UEBwwFc2VvdWwxGjAYBgNVBAoM\r\n"
"EXNlY3VyaXR5IHBsYXRmb3JtMB4XDTE3MTIwNDA2MjczMFoXDTI3MTIwMjA2Mjcz\r\n"
"MFowSTELMAkGA1UEBhMCS1IxDjAMBgNVBAgMBXNlb3VsMQ4wDAYDVQQHDAVzZW91\r\n"
"bDEaMBgGA1UECgwRc2VjdXJpdHkgcGxhdGZvcm0wggEiMA0GCSqGSIb3DQEBAQUA\r\n"
"A4IBDwAwggEKAoIBAQDWOq76EowFFCqL8DJrdqKU2eV5hTgOSuYSSzT/1qaPTtTV\r\n"
"0sa517uxT45ay4Z68xaOx4OVwnWTdEDXmGES6f/bAHXSJGMpUO9/UsLCSsfZX+LR\r\n"
"izo83Ct09euUQO6exdtPG0HywRcLXlvG6cj0cyXqenmKkFaIjqGXif/MhSIE1qmi\r\n"
"KI1Gopy/WqMsEQ1QajOepeICbKardvDQt1NsK3UnA5KWmNJfjkznNilY/EW7RrOP\r\n"
"u8aucJylmlKtGCrwrR5cnPzkrKF2gvYRlsT5Y6MBkDHPVr3Nil0tBGGNYRBMePML\r\n"
"BDaSVM85ak1ldZU1AhbU0VN4B6yrrSGrWgn6quIDAgMBAAGjUDBOMB0GA1UdDgQW\r\n"
"BBQDRiXQeS0+jaGWBwM20T/6ktMPCTAfBgNVHSMEGDAWgBQDRiXQeS0+jaGWBwM2\r\n"
"0T/6ktMPCTAMBgNVHRMEBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQBxmj6VtqfW\r\n"
"aVK+JoN2vSnohtm0WcKMJzI1++yqFpSc4MHM6GuhUjJArzq99p9axD9A+cYTJ3rC\r\n"
"UMLlgmb9qcf0euYob6vCS6Lx+YIAPtGp77V4wIpRqPU+jYwU8Soz+ieFsmI4QIPM\r\n"
"/uH9YFkbZpRTt0WJe3hx5R6VAx48He2MXT1CUoontCMOvftiiOskpMcjMjWFctAp\r\n"
"GdJAIBdZkN9PuY7PGYsHdppNnsLBqdJBUoAJAIw8srDK8wcKpGkXnnopbvUqjvFn\r\n"
"Su+r7M6hhSmgAcRsFGOmSNv9owTWerBdUosj83pgWgl+3S7eMYrDtJoxJek24z2X\r\n"
"NZdvjm4ePhNZ\r\n"
"-----END CERTIFICATE-----\r\n";
*/

/*
const char mbedtls_test_cli_crt_ec2[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIICQzCCASsCCQCcFw9LW7SqhjANBgkqhkiG9w0BAQsFADBJMQswCQYDVQQGEwJL\r\n"
"UjEOMAwGA1UECAwFc2VvdWwxDjAMBgNVBAcMBXNlb3VsMRowGAYDVQQKDBFzZWN1\r\n"
"cml0eSBwbGF0Zm9ybTAeFw0xNzEyMDQwODE1NTdaFw0yNzEyMDIwODE1NTdaMEkx\r\n"
"CzAJBgNVBAYTAktSMQ4wDAYDVQQIDAVzZW91bDEOMAwGA1UEBwwFc2VvdWwxGjAY\r\n"
"BgNVBAoMEXNlY3VyaXR5IHBsYXRmb3JtMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcD\r\n"
"QgAE31eOhMrgYysGKDz4UoIAXD4gZHDt3aksvkAtIKlVPvx0PzK1Y/zzO9kmqV6Q\r\n"
"OsClB7S9DPp1Orq/eo/cqDvNIDANBgkqhkiG9w0BAQsFAAOCAQEA0RuvSevl/X0C\r\n"
"DdvADbx6CTbufj6h8dD0wzeImVqRTCxQGWoBMCi2PcGi4vk9yqEMlJgTOBqCJy33\r\n"
"4aCJOn6LxD89meqAA6A195cr0Ge3GmnBYUxiTOHx8R/0bydMssJ/WwhzZR7taEqR\r\n"
"1Z0vsqJYVhN/kcB9rPhS5CeBgKMHowC8eK2hVpH7/3fXE/9qyFT6Bs3u/n3AfHGT\r\n"
"4bkNA2NQpy4gd5NEGWwHC7fCycvTsVMK0dQWQGfX1CDEKr5wutuU5tNq0cxgq5Fy\r\n"
"1SbTZRwsIjT7JLRxyVSdj4RxfpoTJJYbCllNgk2u+WFUn5zg65smlAYnV1HQ5z+o\r\n"
"FuDmpJQRlg==\r\n"
"-----END CERTIFICATE-----\r\n";

const char mbedtls_test_cli_key_ec2[] = 
"-----BEGIN EC PRIVATE KEY-----\r\n"
"MHcCAQEEIOavZmpx5BEMD0aZ04WFGG1TCcvOz9VqwNOQy6d1KMb8oAoGCCqGSM49\r\n"
"AwEHoUQDQgAE31eOhMrgYysGKDz4UoIAXD4gZHDt3aksvkAtIKlVPvx0PzK1Y/zz\r\n"
"O9kmqV6QOsClB7S9DPp1Orq/eo/cqDvNIA==\r\n"
"-----END EC PRIVATE KEY-----\r\n";

const size_t mbedtls_test_cli_crt_ec2_len = sizeof( mbedtls_test_cli_crt_ec2 );
const size_t mbedtls_test_cli_key_ec2_len = sizeof( mbedtls_test_cli_key_ec2);
*/

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
	UART_PrintInit(Serial.getHandle());
	mbedtls_platform_set_printf(UART_Printf);
	uint16_t ret; 
    ret = ESAL_PKA_FirmwareInit(PKA_BYTELANE_BIG);
    ESAL_TRNG_Init();
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
	mbedtls_debug_set_threshold(3);
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
    //ret = mbedtls_x509_crt_parse( clicert, (const unsigned char *) mbedtls_test_cli_crt_ec2, mbedtls_test_cli_crt_ec2_len );
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

    mbedtls_ssl_conf_authmode( conf, MBEDTLS_SSL_VERIFY_NONE);
    //mbedtls_ssl_conf_authmode( conf, MBEDTLS_SSL_VERIFY_REQUIRED );
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
		//esp8266._state[_socket] = TAKEN;
		esp8266.setState(_socket, TAKEN); //_state[_socket] = TAKEN;
		int16_t rsp = esp8266.tcpConnect(_socket, host, port, keepAlive);
		return rsp;
	}
	return -1;
}
uint8_t ESP8266Mbedtls::connected()
{
	// If data is available, assume we're connected. Otherwise,
	// we'll try to send the status query, and will probably end 
	// up timing out if data is still coming in.
	if (_socket == ESP8266_SOCK_NOT_AVAIL)
		return 0;
	else if (available() > 0)
		return 1;
	else if (status() == ESP8266_STATUS_CONNECTED)
		return 1;
	
	return 0;
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
	
	/*
	MBEDTLS_SSL_DEBUG_MSG(3, ("============= write message begin =============="));
	MBEDTLS_SSL_DEBUG_MSG(3, (req));
	MBEDTLS_SSL_DEBUG_MSG(3, ("============= write message done ==============="));
	*/
	while((ret = mbedtls_ssl_write(ssl, (const unsigned char *)req, len)) <= 0){
		if(ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE){
			return -1;
			//return ssl_write_failed;
		}
	}
	return len; 
	//return exit_ok; 
}

int ESP8266Mbedtls::writeSSL()
{
	int len, ret ;
	char req[256] ;
	
	len = sprintf((char *)req, GET_REQUEST);
	return writeSSL((char *)req, len);
}

size_t ESP8266Mbedtls::write(uint8_t c)
{
	return writeSSL((char *)&c, 1);
}

size_t ESP8266Mbedtls::write(const uint8_t *buf, size_t size)
{
	return writeSSL((char *)buf, size);
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


int ESP8266Mbedtls::read(uint8_t *buf, size_t size)
{
	int ret;
	do{
		ret = mbedtls_ssl_read(ssl, buf, size);
		
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
		
		break;
	}while(1);
	return ret;
}

void ESP8266Mbedtls::stopSSL()
{
	esp8266.close(_socket);
	//esp8266._state[_socket] = AVAILABLE;
	esp8266.setState(_socket, AVAILABLE);
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
	// single connection
	esp8266.updateStatus();
	if(0 == esp8266.getMux()){
		return ESP8266_MAX_SOCK_NUM;
	}
	else{ // multiple connection
		for (int i = 0; i < ESP8266_MAX_SOCK_NUM; i++) 
		{
			//if (esp8266._status.ipstatus[i].linkID == 255)
			if (esp8266.getLinkID(i) == 255)
			{
				return i;
			}
		}
		return ESP8266_SOCK_NOT_AVAIL;
	}
}
