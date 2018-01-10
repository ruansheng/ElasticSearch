#ifndef ELASTICSEARCH_UTIL_H
#define ELASTICSEARCH_UTIL_H

extern zend_class_entry *elasticsearch_client_ce;
ELASTICSEARCH_STARTUP_FUNCTION(client);

PHPAPI void es_client_add_parse(INTERNAL_FUNCTION_PARAMETERS, zend_string **request_url, zval **zv_body);
PHPAPI void es_client_add_request(INTERNAL_FUNCTION_PARAMETERS, zend_string **request_url, zval **zv_body);

#endif //ELASTICSEARCH_UTIL_H