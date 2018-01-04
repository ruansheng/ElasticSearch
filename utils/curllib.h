#ifndef PHP7_CURLLIB_H_H
#define PHP7_CURLLIB_H_H

#include <stdbool.h>
#include <curl/curl.h>

typedef struct MemoryStruct {
    char *memory;
    size_t size;
} chunk;

size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
bool libcurlGet(const char *url, chunk *ret, long connect_timeout, long request_timeout);
bool libcurlPost(const char *url, const char *postdata, chunk *ret, long connect_timeout, long request_timeout);
bool libcurlDelete(const char *url, chunk *ret, long connect_timeout, long request_timeout);

#endif //PHP7_CURLLIB_H_H