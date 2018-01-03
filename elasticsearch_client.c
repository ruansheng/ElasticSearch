#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"

#include "php_elasticsearch.h"
#include "elasticsearch_client.h"

zend_class_entry *elasticsearch_client_ce;

/** {{{ pmc_util_methods
*/
zend_function_entry elasticsearch_client_methods[] = {
        PHP_ME(elasticsearch_client, show, NULL, ZEND_ACC_PUBLIC)
        {NULL, NULL, NULL}
};
/* }}} */

/** {{{ proto public PmcUtil::show()
*/
PHP_METHOD(pmc_util, show) {
    php_printf("hello util!");
}
/* }}} */

ELASTICSEARCH_STARTUP_FUNCTION(client){
        zend_class_entry ce;

        INIT_CLASS_ENTRY(ce, "ElasticSearchClient", elasticsearch_client_methods);
        elasticsearch_client_ce = zend_register_internal_class(&ce);

        return SUCCESS;
}