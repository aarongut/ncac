#include "fetch.h"

#include <stdbool.h>
#include <curl/curl.h>

  CURL *curl;

bool asana_init() {
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if (!curl) return false;

  return true;
}

void asana_cleanup() {
  curl_global_cleanup();
}

