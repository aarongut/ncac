#include "fetch.h"

#include <curl/curl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CURL *curl;

char asana_auth_header_str[AUTH_HEADER_SIZE];
struct curl_slist *asana_auth_header;

bool asana_init(char *pat) {
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if (!curl) return false;

  snprintf(asana_auth_header_str, AUTH_HEADER_SIZE, "Authorization: Bearer %s",
           pat);
  asana_auth_header = curl_slist_append(NULL, asana_auth_header_str);

  if (!asana_auth_header) return false;

  return true;
}

void asana_cleanup() {
  curl_slist_free_all(asana_auth_header);
  curl_easy_cleanup(curl);
  curl_global_cleanup();
}

size_t asana_write_callback(void *contents, size_t size, size_t nmemb,
                            void *resp) {
  Response *response = (Response *)resp;

  size_t realsize = size * nmemb;

  char *body = realloc(response->body, response->len + realsize + 1);
  response->body = body;
  memcpy(&(response->body[response->len]), contents, realsize);
  response->len += realsize;
  response->body[response->len] = '\0';

  return realsize;
}

Response *asana_fetch(char *path) {
  char url[ASANA_URL_MAX_LENGTH];
  snprintf(url, ASANA_URL_MAX_LENGTH, "https://app.asana.com/api/1.0/%s", path);
  curl_easy_setopt(curl, CURLOPT_URL, url);

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, asana_auth_header);

  Response *response = calloc(sizeof(Response), 1);
  response->body = calloc(sizeof(char), 1);

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, asana_write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);

  CURLcode result;
  result = curl_easy_perform(curl);
  if (result == CURLE_OK) {
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &(response->status));
  } else {
    response->status = 500;
  }

  return response;
}

void asana_free_response(Response *r) {
  free(r->body);
  free(r);
}
