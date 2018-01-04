
#include "utils/curllib.h"

size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        /* out of memory! */
        //printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

/**
 * libcurlGet
 */
bool libcurlGet(const char *url, chunk *ret, long connect_timeout, long request_timeout)
{
    // curl request
    CURL * curl_handle = curl_easy_init();
    struct curl_slist *headers = NULL;

    headers = curl_slist_append(NULL, "Accept-Charset: utf-8");
    curl_slist_append(headers, "Cache-Control: no-cache");
    curl_slist_append(headers, "Keep-Alive: 300");
    curl_slist_append(headers, "Connection: keep-alive");
    curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded;charset=UTF-8");

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, connect_timeout);
    curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, request_timeout);

    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)ret);

    CURLcode info_status;
    info_status = curl_easy_perform(curl_handle);

    curl_easy_cleanup(curl_handle);

    if(CURLE_OK == info_status) {
        return true;
    } else {
        return false;
    }
}

/**
 * libcurlPost
 */
bool libcurlPost(const char *url, const char *postdata, chunk *ret, long connect_timeout, long request_timeout)
{
    // curl request
    CURL * curl_handle = curl_easy_init();
    struct curl_slist *headers = NULL;

    // curl request url
    headers = curl_slist_append(NULL, "Accept-Charset: utf-8");
    curl_slist_append(headers, "Cache-Control: no-cache");
    curl_slist_append(headers, "Keep-Alive: 300");
    curl_slist_append(headers, "Connection: keep-alive");
    curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded;charset=UTF-8");

    curl_easy_setopt(curl_handle, CURLOPT_POST, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, connect_timeout);
    curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, request_timeout);

    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, postdata);

    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)ret);

    CURLcode info_status;
    info_status = curl_easy_perform(curl_handle);

    curl_easy_cleanup(curl_handle);

    if(CURLE_OK == info_status) {
        return true;
    } else {
        return false;
    }
}

/**
 * libcurlDelete
 */
bool libcurlDelete(const char *url, chunk *ret, long connect_timeout, long request_timeout)
{
    // curl request
    CURL * curl_handle = curl_easy_init();
    struct curl_slist *headers = NULL;

    headers = curl_slist_append(NULL, "Accept-Charset: utf-8");
    curl_slist_append(headers, "Cache-Control: no-cache");
    curl_slist_append(headers, "Keep-Alive: 300");
    curl_slist_append(headers, "Connection: keep-alive");
    curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded;charset=UTF-8");

    curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, connect_timeout);
    curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT, request_timeout);

    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)ret);

    CURLcode info_status;
    info_status = curl_easy_perform(curl_handle);

    curl_easy_cleanup(curl_handle);

    if(CURLE_OK == info_status) {
        return true;
    } else {
        return false;
    }
}