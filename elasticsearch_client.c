#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

#include "php_elasticsearch.h"
#include "elasticsearch_client.h"
#include "utils/curllib.h"

static inline void trace(const char *file, int line, const char* function, const char *fmt, ...) {
    fprintf(stderr, "%s(%s:%d) - ", function, file, line);
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

zend_class_entry *elasticsearch_client_ce;

/** {{{ proto public ElasticSearchClient::__construct([string host] [,string|int port])
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

/** {{{ proto public ElasticSearchClient::add(array params)
*/
PHP_METHOD(elasticsearch_client, add) {
	zval *zv_body;
	zend_string *request_url;
	
	// format 'request_url' and 'reques tbody'
	es_client_add_parse(INTERNAL_FUNCTION_PARAM_PASSTHRU, &request_url, &zv_body);

	// request es server
	es_client_request(INTERNAL_FUNCTION_PARAM_PASSTHRU, ES_REPUEST_ADD, &request_url, &zv_body);
}
/* }}} */

/* {{{ es_client_add_parse - ElasticSearchClient::add helper */
PHPAPI void es_client_add_parse(INTERNAL_FUNCTION_PARAMETERS, zend_string **request_url, zval **zv_body) 
{
	zval *params = NULL;
    zval *host;
	zval *port;
    HashTable *params_hash;
    zval *zv_id;
    zval *zv_index;
    zval *zv_type;

    // parse method args
    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(params)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(params) != IS_ARRAY) 
	{
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params must is array");
		RETURN_FALSE;
	}

	params_hash = Z_ARRVAL_P(params);

    // check and get map value by key 
	zv_index = zend_hash_str_find(params_hash, "index", sizeof("index") - 1);
	if(zv_index == NULL) 
	{
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "must contain 'index' in hash params");
		RETURN_FALSE;
	}
    if(Z_TYPE_P(zv_index) != IS_STRING) 
	{
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params['index'] type must is string");
		RETURN_FALSE;
	}

    zv_type = zend_hash_str_find(params_hash, "type", sizeof("type") - 1);
	if(zv_type == NULL) 
	{
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "must contain 'type' in hash params");
		RETURN_FALSE;
	}
    if(Z_TYPE_P(zv_type) != IS_STRING) 
	{
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params['type'] type must is string");
		RETURN_FALSE;
	}

    *zv_body = zend_hash_str_find(params_hash, "body", sizeof("body") - 1);
	if(*zv_body == NULL) 
	{
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "must contain 'body' in hash params");
		RETURN_FALSE;
	}
    if(Z_TYPE_P(*zv_body) != IS_ARRAY) 
	{
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params['body'] type must is array");
		RETURN_FALSE;
	}

    zv_id = zend_hash_str_find(params_hash, "id", sizeof("id") - 1);
    if(zv_id != NULL && Z_TYPE_P(zv_id) == IS_LONG) 
	{
        convert_to_string(zv_id);
    }

    host = zend_read_property(elasticsearch_client_ce, getThis(), "host", sizeof("host") -1, 0, host);
	port = zend_read_property(elasticsearch_client_ce, getThis(), "port", sizeof("port") -1, 0, port);

    // make request url
	if((zv_id == NULL)) 
	{
		*request_url = strpprintf(0, "http://%s:%d/%s/%s", Z_STRVAL_P(host), Z_LVAL_P(port), Z_STRVAL_P(zv_index), Z_STRVAL_P(zv_type));
	} 
	else if(Z_TYPE_P(zv_id) == IS_STRING) 
	{
		*request_url = strpprintf(0, "http://%s:%d/%s/%s/%s", Z_STRVAL_P(host), Z_LVAL_P(port), Z_STRVAL_P(zv_index), Z_STRVAL_P(zv_type), Z_STRVAL_P(zv_id));
	} 
	else 
	{
        zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params exists error argv");
		RETURN_FALSE;
    }

#if RS_DEBUG == 1
	TRACE("%s", ZSTR_VAL(*request_url));
#endif
}
/* }}} */

/* {{{ es_client_add_parse - ElasticSearchClient::add helper */
PHPAPI void es_client_request(INTERNAL_FUNCTION_PARAMETERS, int type, zend_string **request_url, zval **zv_body) 
{
	zval *connect_timeout;
    zval *request_timeout;    

	connect_timeout = zend_read_static_property(elasticsearch_client_ce, "connect_timeout", sizeof("connect_timeout") -1, 0);
	request_timeout = zend_read_static_property(elasticsearch_client_ce, "request_timeout", sizeof("request_timeout") -1, 0);

	// 请求es服务
	chunk ret;
	ret.memory = malloc(1);
	ret.size = 0;

	zval call_func_name;
	zval call_func_ret;
	uint32_t param_count = 1;
	zval func_params[1];

	ZVAL_STRING(&call_func_name, "json_encode");
	ZVAL_ZVAL(&func_params[0], *zv_body, 0, 0);
	if(SUCCESS != call_user_function(EG(function_table), NULL, &call_func_name, &call_func_ret, param_count, func_params)) 
	{
		goto out;
	}

	switch(type) 
	{
		case ES_REPUEST_ADD:
			if(!libcurlPost(ZSTR_VAL(*request_url), Z_STRVAL(call_func_ret), &ret, Z_LVAL_P(connect_timeout), Z_LVAL_P(request_timeout))) 
			{
				zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "curl request error");
				goto out;
			}
			break;
		case ES_REPUEST_REMOVE:
			if(!libcurlPost(ZSTR_VAL(*request_url), Z_STRVAL(call_func_ret), &ret, Z_LVAL_P(connect_timeout), Z_LVAL_P(request_timeout))) 
			{
				zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "curl request error");
				goto out;
			}
			break;	
		case ES_REPUEST_UPDATE:
			if(!libcurlPost(ZSTR_VAL(*request_url), Z_STRVAL(call_func_ret), &ret, Z_LVAL_P(connect_timeout), Z_LVAL_P(request_timeout))) 
			{
				zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "curl request error");
				goto out;
			}
			break;
		case ES_REPUEST_GET:	
			if(!libcurlPost(ZSTR_VAL(*request_url), Z_STRVAL(call_func_ret), &ret, Z_LVAL_P(connect_timeout), Z_LVAL_P(request_timeout))) 
			{
				zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "curl request error");
				goto out;
			}		
			break;			
		case ES_REPUEST_SEARCH:	
			if(!libcurlPost(ZSTR_VAL(*request_url), Z_STRVAL(call_func_ret), &ret, Z_LVAL_P(connect_timeout), Z_LVAL_P(request_timeout))) 
			{
				zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "curl request error");
				goto out;
			}		
			break;			
		default:

			break;			
	}

	zend_string *result = strpprintf(0, "%s", ret.memory);

	// free
	free(ret.memory);
	zend_string_free(*request_url);
	zval_ptr_dtor(&call_func_name);
	zval_ptr_dtor(&call_func_ret);
	
	RETURN_NEW_STR(result);

out:
	free(ret.memory);
	zend_string_free(*request_url);
	zval_ptr_dtor(&call_func_name);
	zval_ptr_dtor(&call_func_ret);
	RETURN_FALSE;
}
/* }}} */

/** {{{ proto public ElasticSearchClient::remove(array params)
*/
PHP_METHOD(elasticsearch_client, remove) {
    zval *params = NULL;
    zval *host;
	zval *port;
    HashTable *params_hash;
    zval *zv_id;
    zval *zv_index;
    zval *zv_type;
    zval *connect_timeout;
    zval *request_timeout;
	zend_string * request_url;

    // parse method args
    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(params)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(params) != IS_ARRAY) {
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params must is array");
		RETURN_FALSE;
	}

	params_hash = Z_ARRVAL_P(params);

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

    zv_id = zend_hash_str_find(params_hash, "id", sizeof("id") - 1);
    if(zv_id == NULL) {
        zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "must contain 'id' in hash params");
		RETURN_FALSE;
    }
    if(Z_TYPE_P(zv_id) == IS_LONG) {
        convert_to_string(zv_id);
    }
	if(Z_TYPE_P(zv_id) != IS_STRING) {
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params['id'] type must is string");
		RETURN_FALSE;
	}

    host = zend_read_property(elasticsearch_client_ce, getThis(), "host", sizeof("host") -1, 0, host);
	port = zend_read_property(elasticsearch_client_ce, getThis(), "port", sizeof("port") -1, 0, port);

    connect_timeout = zend_read_static_property(elasticsearch_client_ce, "connect_timeout", sizeof("connect_timeout") -1, 0);
	request_timeout = zend_read_static_property(elasticsearch_client_ce, "request_timeout", sizeof("request_timeout") -1, 0);

	// make request url
	request_url = strpprintf(0, "http://%s:%d/%s/%s/%s", Z_STRVAL_P(host), Z_LVAL_P(port), Z_STRVAL_P(zv_index), Z_STRVAL_P(zv_type), Z_STRVAL_P(zv_id));

	zend_string_free(request_url);

	RETURN_TRUE;
}
/* }}} */

/** {{{ proto public ElasticSearchClient::update(array params)
*/
PHP_METHOD(elasticsearch_client, update) {
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
    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(params)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(params) != IS_ARRAY) {
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params must is array");
		RETURN_FALSE;
	}

	params_hash = Z_ARRVAL_P(params);

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

	zv_id = zend_hash_str_find(params_hash, "id", sizeof("id") - 1);
    if(zv_id == NULL) {
        zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "must contain 'id' in hash params");
		RETURN_FALSE;
    }
    if(Z_TYPE_P(zv_id) == IS_LONG) {
        convert_to_string(zv_id);
    }
	if(Z_TYPE_P(zv_id) != IS_STRING) {
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params['id'] type must is string");
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

    host = zend_read_property(elasticsearch_client_ce, getThis(), "host", sizeof("host") -1, 0, host);
	port = zend_read_property(elasticsearch_client_ce, getThis(), "port", sizeof("port") -1, 0, port);

    connect_timeout = zend_read_static_property(elasticsearch_client_ce, "connect_timeout", sizeof("connect_timeout") -1, 0);
	request_timeout = zend_read_static_property(elasticsearch_client_ce, "request_timeout", sizeof("request_timeout") -1, 0);

    // make request url
	request_url = strpprintf(0, "http://%s:%d/%s/%s/%s", Z_STRVAL_P(host), Z_LVAL_P(port), Z_STRVAL_P(zv_index), Z_STRVAL_P(zv_type), Z_STRVAL_P(zv_id));

	zend_string_free(request_url);

	RETURN_TRUE;
}
/* }}} */

/** {{{ proto public ElasticSearchClient::get(array params)
*/
PHP_METHOD(elasticsearch_client, get) {
    zval *params = NULL;
    zval *host;
	zval *port;
    HashTable *params_hash;
    zval *zv_id;
    zval *zv_index;
    zval *zv_type;
    zval *connect_timeout;
    zval *request_timeout;
	zend_string * request_url;

    // parse method args
    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(params)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(params) != IS_ARRAY) {
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params must is array");
		RETURN_FALSE;
	}

	params_hash = Z_ARRVAL_P(params);

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

    zv_id = zend_hash_str_find(params_hash, "id", sizeof("id") - 1);
    if(zv_id == NULL) {
        zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "must contain 'id' in hash params");
		RETURN_FALSE;
    }
    if(Z_TYPE_P(zv_id) == IS_LONG) {
        convert_to_string(zv_id);
    }
	if(Z_TYPE_P(zv_id) != IS_STRING) {
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params['id'] type must is string");
		RETURN_FALSE;
	}

    host = zend_read_property(elasticsearch_client_ce, getThis(), "host", sizeof("host") -1, 0, host);
	port = zend_read_property(elasticsearch_client_ce, getThis(), "port", sizeof("port") -1, 0, port);

    connect_timeout = zend_read_static_property(elasticsearch_client_ce, "connect_timeout", sizeof("connect_timeout") -1, 0);
	request_timeout = zend_read_static_property(elasticsearch_client_ce, "request_timeout", sizeof("request_timeout") -1, 0);

	// make request url
	request_url = strpprintf(0, "http://%s:%d/%s/%s/%s", Z_STRVAL_P(host), Z_LVAL_P(port), Z_STRVAL_P(zv_index), Z_STRVAL_P(zv_type), Z_STRVAL_P(zv_id));

	zend_string_free(request_url);

	RETURN_TRUE;
}
/* }}} */

/** {{{ proto public ElasticSearchClient::search(array params)
*/
PHP_METHOD(elasticsearch_client, search) {
    zval *params = NULL;
    zval *host;
	zval *port;
    HashTable *params_hash;
    zval *zv_index;
    zval *zv_type;
    zval *zv_body;
    zval *connect_timeout;
    zval *request_timeout;
	zend_string * request_url;

    // parse method args
    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(params)
	ZEND_PARSE_PARAMETERS_END();

	if (Z_TYPE_P(params) != IS_ARRAY) {
		zend_update_property_string(elasticsearch_client_ce,  getThis(), "message", sizeof("message") - 1, "params must is array");
		RETURN_FALSE;
	}

	params_hash = Z_ARRVAL_P(params);

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

    host = zend_read_property(elasticsearch_client_ce, getThis(), "host", sizeof("host") -1, 0, host);
	port = zend_read_property(elasticsearch_client_ce, getThis(), "port", sizeof("port") -1, 0, port);

    connect_timeout = zend_read_static_property(elasticsearch_client_ce, "connect_timeout", sizeof("connect_timeout") -1, 0);
	request_timeout = zend_read_static_property(elasticsearch_client_ce, "request_timeout", sizeof("request_timeout") -1, 0);

    // make request url
	request_url = strpprintf(0, "http://%s:%d/%s/%s", Z_STRVAL_P(host), Z_LVAL_P(port), Z_STRVAL_P(zv_index), Z_STRVAL_P(zv_type));

	zend_string_free(request_url);

	RETURN_TRUE;
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

	RETURN_STRING(Z_STRVAL_P(zv_message));
}
/* }}} */

/** {{{ proto public ElasticSearchClient::setConnectTimeout(long connect_timeout)
*/
PHP_METHOD(elasticsearch_client, setConnectTimeout) {
    zend_long connect_timeout = NULL;
	
	// parse method args
    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(connect_timeout)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_static_property_long(elasticsearch_client_ce, "connect_timeout", sizeof("connect_timeout") - 1, connect_timeout);

	RETURN_TRUE;
}
/* }}} */

/** {{{ proto public ElasticSearchClient::setRequestTimeout(long request_timeout)
*/
PHP_METHOD(elasticsearch_client, setRequestTimeout) {
    zend_long request_timeout = NULL;
	
	// parse method args
    ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(request_timeout)
	ZEND_PARSE_PARAMETERS_END();

	zend_update_static_property_long(elasticsearch_client_ce, "request_timeout", sizeof("request_timeout") - 1, request_timeout);

	RETURN_TRUE;
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
	zend_declare_property_long(elasticsearch_client_ce, "port", sizeof("port") - 1, 9200L, ZEND_ACC_PRIVATE);
	zend_declare_property_string(elasticsearch_client_ce, "message", sizeof("message") - 1, "", ZEND_ACC_PRIVATE);
	zend_declare_property_long(elasticsearch_client_ce, "connect_timeout", sizeof("connect_timeout") - 1, 1000L, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC);
	zend_declare_property_long(elasticsearch_client_ce, "request_timeout", sizeof("request_timeout") - 1, 2000L, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC);

    return SUCCESS;
}