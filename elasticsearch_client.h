#ifndef ELASTICSEARCH_UTIL_H
#define ELASTICSEARCH_UTIL_H

extern zend_class_entry *elasticsearch_client_ce;
ELASTICSEARCH_STARTUP_FUNCTION(client);

// PHP_METHOD(elasticsearch_client, __construct);
// PHP_METHOD(elasticsearch_client, add);
// PHP_METHOD(elasticsearch_client, remove);
// PHP_METHOD(elasticsearch_client, update);
// PHP_METHOD(elasticsearch_client, get);
// PHP_METHOD(elasticsearch_client, search);
// PHP_METHOD(elasticsearch_client, getMessage);
// PHP_METHOD(elasticsearch_client, setConnectTimeout);
// PHP_METHOD(elasticsearch_client, setRequestTimeout);

#endif //ELASTICSEARCH_UTIL_H