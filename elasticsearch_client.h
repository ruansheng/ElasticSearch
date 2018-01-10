#ifndef ELASTICSEARCH_UTIL_H
#define ELASTICSEARCH_UTIL_H

#define RS_DEBUG 1

extern zend_class_entry *elasticsearch_client_ce;
ELASTICSEARCH_STARTUP_FUNCTION(client);

PHPAPI void es_client_add_parse(INTERNAL_FUNCTION_PARAMETERS, zend_string **request_url, zval **zv_body);
PHPAPI void es_client_add_request(INTERNAL_FUNCTION_PARAMETERS, zend_string **request_url, zval **zv_body);

static inline void trace(const char *file, int line, const char* function, const char *fmt, ...);
#define TRACE(fmt, ...) do{trace(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__); }while(0)

#endif //ELASTICSEARCH_UTIL_H