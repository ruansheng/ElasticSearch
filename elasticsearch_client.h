#ifndef ELASTICSEARCH_UTIL_H
#define ELASTICSEARCH_UTIL_H

extern zend_class_entry *elasticsearch_client_ce;
ELASTICSEARCH_STARTUP_FUNCTION(client);

PHP_METHOD(elasticsearch_client, show);

#endif //ELASTICSEARCH_UTIL_H