#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

#include "php_elasticsearch.h"
#include "elasticsearch_client.h"

zend_class_entry *elasticsearch_client_ce;

/** {{{ elasticsearch_client_methods
*/
zend_function_entry elasticsearch_client_methods[] = {
        PHP_ME(elasticsearch_client, show, NULL, ZEND_ACC_PUBLIC)
        {NULL, NULL, NULL}
};
/* }}} */

/** {{{ proto public ElasticSearchClient::show()
*/
PHP_METHOD(elasticsearch_client, show) {
    php_printf("hello ElasticSearchClient show!");
}
/* }}} */

ELASTICSEARCH_STARTUP_FUNCTION(client){
        zend_class_entry ce;

        INIT_CLASS_ENTRY(ce, "ElasticSearchClient", elasticsearch_client_methods);
        elasticsearch_client_ce = zend_register_internal_class(&ce);

        return SUCCESS;
}