#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

#include "php_elasticsearch.h"
#include "elasticsearch_client.h"
#include "utils/curllib.h"

zend_class_entry *elasticsearch_client_ce;

/** {{{ proto public ElasticSearchClient::__construct()
*/
PHP_METHOD(elasticsearch_client, __construct) {
    zval *host;
	zval *port;

    // parse method args
    ZEND_PARSE_PARAMETERS_START(0, 2)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(host)
		Z_PARAM_ZVAL(port)
	ZEND_PARSE_PARAMETERS_END();

    if (Z_TYPE_P(host) == IS_STRING) {
		zend_update_property_string(elasticsearch_client_ce, getThis(), "host", sizeof("host") - 1, Z_STRVAL_P(host));
	}

    // deal with port field string convert to long
    if(Z_TYPE_P(port) == IS_STRING) {
        convert_to_long(port);
    }

	if (Z_TYPE_P(port) == IS_LONG) {
	    zend_update_property_long(elasticsearch_client_ce, getThis(), "port", sizeof("port") - 1, Z_LVAL_P(port));
    }
}
/* }}} */

/** {{{ proto public ElasticSearchClient::add()
*/
PHP_METHOD(elasticsearch_client, add) {
    zval *params = NULL;
    zval *host;
	zval *port;
    HashTable *params_hash;
    zval *zv_id;
    zval *zv_index;
    zval *zv_type;
    zval *zv_body;
    zval *connect_timeout;
    zval *request_timeout;
	zend_string * request_url;

    // parse method args
	/*
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &params) == FAILURE) {
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "this method must hava a params");
		RETURN_FALSE;
	}
	*/

    ZEND_PARSE_PARAMETERS_START(1, 1)
		//Z_PARAM_ARRAY(params)
		Z_PARAM_ZVAL(params)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(params) != IS_ARRAY) {
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params must is array");
		RETURN_FALSE;
	}

	params_hash = Z_ARRVAL_P(params);

/*
    // check and get map value by key 
	zv_index = zend_hash_str_find(params_hash, "index", sizeof("index") - 1);
	if(zv_index == NULL) {
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "must contain 'index' in hash params");
		RETURN_FALSE;
	}
    if(Z_TYPE_P(zv_index) != IS_STRING) {
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params['index'] type must is string");
		RETURN_FALSE;
	}

    zv_type = zend_hash_str_find(params_hash, "type", sizeof("type") - 1);
	if(zv_type == NULL) {
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "must contain 'type' in hash params");
		RETURN_FALSE;
	}
    if(Z_TYPE_P(zv_type) != IS_STRING) {
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params['type'] type must is string");
		RETURN_FALSE;
	}

    zv_body = zend_hash_str_find(params_hash, "body", sizeof("body") - 1);
	if(zv_body == NULL) {
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "must contain 'body' in hash params");
		RETURN_FALSE;
	}
    if(Z_TYPE_P(zv_body) != IS_ARRAY) {
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params['body'] type must is array");
		RETURN_FALSE;
	}

    zv_id = zend_hash_str_find(params_hash, "id", sizeof("id") - 1);
    if(zv_id != NULL && Z_TYPE_P(zv_id) == IS_LONG) {
        convert_to_string(zv_id);
    }

    host = zend_read_property(elasticsearch_client_ce, getThis(), "host", sizeof("host") -1, 0, host);
	port = zend_read_property(elasticsearch_client_ce, getThis(), "port", sizeof("port") -1, 0, port);

    connect_timeout = zend_read_static_property(elasticsearch_client_ce, "connect_timeout", sizeof("connect_timeout") -1, 0);
	request_timeout = zend_read_static_property(elasticsearch_client_ce, "request_timeout", sizeof("request_timeout") -1, 0);

    // make request url
	if((zv_id == NULL)) {
		request_url = strpprintf(0, "http://%s:%d/%s/%s", Z_STRVAL_P(host), Z_LVAL_P(port), Z_STRVAL_P(zv_index), Z_STRVAL_P(zv_type));
	} else if(Z_TYPE_P(zv_id) == IS_STRING) {
		request_url = strpprintf(0, "http://%s:%d/%s/%s/%s", Z_STRVAL_P(host), Z_LVAL_P(port), Z_STRVAL_P(zv_index), Z_STRVAL_P(zv_type), Z_STRVAL_P(zv_id));
	} else {
        zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params exists error argv");
		RETURN_FALSE;
    }
*/
	zend_string_free(request_url);
/*
	// 请求es服务
	chunk ret;
	ret.memory = malloc(1);
	ret.size = 0;

	smart_str buf = {0};
	zend_long options = 0;
	php_json_encode(&buf, zv_body, (int)options);

	if(!libcurlPost(ZSTR_VAL(request_url), ZSTR_VAL(buf.s), &ret, Z_LVAL_P(connect_timeout), Z_LVAL_P(request_timeout))) {
		free(ret.memory);
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "curl request error");
		RETURN_FALSE;
	}

	zend_string *result = strpprintf(0, "%s", ret.memory);

	// free
	free(ret.memory);

	RETURN_STR(result);
*/
	RETURN_TRUE;
}
/* }}} */

/** {{{ proto public ElasticSearchClient::remove()
*/
PHP_METHOD(elasticsearch_client, remove) {
    php_printf("ElasticSearchClient remove!\n");
}
/* }}} */

/** {{{ proto public ElasticSearchClient::update()
*/
PHP_METHOD(elasticsearch_client, update) {
    php_printf("ElasticSearchClient update!\n");
}
/* }}} */

/** {{{ proto public ElasticSearchClient::get()
*/
PHP_METHOD(elasticsearch_client, get) {
    php_printf("ElasticSearchClient get!\n");
}
/* }}} */

/** {{{ proto public ElasticSearchClient::search()
*/
PHP_METHOD(elasticsearch_client, search) {
    php_printf("ElasticSearchClient search!\n");
}
/* }}} */

/** {{{ proto public ElasticSearchClient::getMessage()
*/
PHP_METHOD(elasticsearch_client, getMessage) {
    zval *zv_message;
	zv_message = zend_read_property(elasticsearch_client_ce, getThis(), "message", sizeof("message") -1, 0, zv_message);

	if(Z_TYPE_P(zv_message) != IS_STRING) {
		RETURN_FALSE;
	}

	//RETURN_STR(Z_STR_P(zv_message));
	zend_string *message;
	message = Z_STR_P(zv_message);
	RETURN_STR(message);
}
/* }}} */

/** {{{ proto public ElasticSearchClient::setConnectTimeout()
*/
PHP_METHOD(elasticsearch_client, setConnectTimeout) {
    php_printf("ElasticSearchClient setConnectTimeout!\n");
}
/* }}} */

/** {{{ proto public ElasticSearchClient::setRequestTimeout()
*/
PHP_METHOD(elasticsearch_client, setRequestTimeout) {
    php_printf("ElasticSearchClient setRequestTimeout!\n");
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

    zend_declare_property_string(elasticsearch_client_ce, "host", sizeof("host") - 1, "locaohost", ZEND_ACC_PRIVATE);
	zend_declare_property_long(elasticsearch_client_ce, "port", sizeof("port") - 1, 9023L, ZEND_ACC_PRIVATE);
	zend_declare_property_string(elasticsearch_client_ce, "message", sizeof("message") - 1, "", ZEND_ACC_PRIVATE);
	zend_declare_property_long(elasticsearch_client_ce, "connect_timeout", sizeof("connect_timeout") - 1, 1L, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC);
	zend_declare_property_long(elasticsearch_client_ce, "request_timeout", sizeof("request_timeout") - 1, 2L, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC);

    return SUCCESS;
}