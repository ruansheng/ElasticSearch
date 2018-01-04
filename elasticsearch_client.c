#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

#include "php_elasticsearch.h"
#include "elasticsearch_client.h"

zend_class_entry *elasticsearch_client_ce;

/** {{{ proto public ElasticSearchClient::__construct()
*/
PHP_METHOD(elasticsearch_client, __construct) {
    php_printf("ElasticSearchClient __construct!");
    zval *host;
	zval *port;

	// parse method args
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz", &host, &port) == FAILURE) {
		return;
	}

    if (Z_TYPE_P(host) == IS_STRING) {
		zend_update_property_string(elastic_search_client_ce, getThis(), "host", sizeof("host") - 1, Z_STR_P(host));
	}

    // deal with port field string convert to long
    if(Z_TYPE_P(port) == IS_STRING) {
        convert_to_long(port);
    }

	if (Z_TYPE_P(port) == IS_LONG) {
	    zend_update_property_long(elastic_search_client_ce, getThis(), "port", sizeof("port") - 1, Z_LVAL_P(port));
    }
}
/* }}} */

/** {{{ proto public ElasticSearchClient::add()
*/
PHP_METHOD(elasticsearch_client, add) {
    php_printf("ElasticSearchClient add!");
}
/* }}} */

/** {{{ proto public ElasticSearchClient::remove()
*/
PHP_METHOD(elasticsearch_client, remove) {
    php_printf("ElasticSearchClient remove!");
}
/* }}} */

/** {{{ proto public ElasticSearchClient::update()
*/
PHP_METHOD(elasticsearch_client, update) {
    php_printf("ElasticSearchClient update!");
}
/* }}} */

/** {{{ proto public ElasticSearchClient::get()
*/
PHP_METHOD(elasticsearch_client, get) {
    php_printf("ElasticSearchClient get!");
}
/* }}} */

/** {{{ proto public ElasticSearchClient::search()
*/
PHP_METHOD(elasticsearch_client, search) {
    php_printf("ElasticSearchClient search!");
}
/* }}} */

/** {{{ proto public ElasticSearchClient::getMessage()
*/
PHP_METHOD(elasticsearch_client, getMessage) {
    php_printf("ElasticSearchClient getMessage!");
}
/* }}} */

/** {{{ proto public ElasticSearchClient::setConnectTimeout()
*/
PHP_METHOD(elasticsearch_client, setConnectTimeout) {
    php_printf("ElasticSearchClient setConnectTimeout!");
}
/* }}} */

/** {{{ proto public ElasticSearchClient::setRequestTimeout()
*/
PHP_METHOD(elasticsearch_client, setRequestTimeout) {
    php_printf("ElasticSearchClient setRequestTimeout!");
}
/* }}} */

ZEND_BEGIN_ARG_INFO_EX(arginfo_client_add, 0, 0, 1)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_client_remove, 0, 0, 1)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_client_update, 0, 0, 1)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_client_get, 0, 0, 1)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_client_search, 0, 0, 1)
	ZEND_ARG_INFO(0, params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_client_connect_timeout, 0, 0, 1)
	ZEND_ARG_INFO(0, millisecond)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_client_request_timeout, 0, 0, 1)
	ZEND_ARG_INFO(0, millisecond)
ZEND_END_ARG_INFO()

/** {{{ elasticsearch_client_methods
*/
zend_function_entry elasticsearch_client_methods[] = {
        PHP_ME(elasticsearch_client, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
        PHP_ME(elasticsearch_client, add, arginfo_client_add, ZEND_ACC_PUBLIC)
        PHP_ME(elasticsearch_client, remove, arginfo_client_remove, ZEND_ACC_PUBLIC)
        PHP_ME(elasticsearch_client, update, arginfo_client_update, ZEND_ACC_PUBLIC)
        PHP_ME(elasticsearch_client, get, arginfo_client_get, ZEND_ACC_PUBLIC)
        PHP_ME(elasticsearch_client, search, arginfo_client_search, ZEND_ACC_PUBLIC)
        PHP_ME(elasticsearch_client, getMessage, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(elasticsearch_client, setConnectTimeout, arginfo_client_connect_timeout, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(elasticsearch_client, setRequestTimeout, arginfo_client_request_timeout, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        {NULL, NULL, NULL}
};
/* }}} */

ELASTICSEARCH_STARTUP_FUNCTION(client){
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "ElasticSearchClient", elasticsearch_client_methods);
    elasticsearch_client_ce = zend_register_internal_class(&ce);

    zend_declare_property_string(elastic_search_client_ce, "host", sizeof("host") - 1, "locaohost", ZEND_ACC_PRIVATE);
	zend_declare_property_long(elastic_search_client_ce, "port", sizeof("port") - 1, 9023L, ZEND_ACC_PRIVATE);
	zend_declare_property_string(elastic_search_client_ce, "message", sizeof("message") - 1, "", ZEND_ACC_PRIVATE);
	zend_declare_property_long(elastic_search_client_ce, "connect_timeout", sizeof("connect_timeout") - 1, 1L, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC);
	zend_declare_property_long(elastic_search_client_ce, "request_timeout", sizeof("request_timeout") - 1, 2L, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC);

    return SUCCESS;
}