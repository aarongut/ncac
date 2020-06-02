#ifndef ASANA_FETCH_H_
#define ASANA_FETCH_H_

#include <stdbool.h>
#include <stdlib.h>

#define AUTH_HEADER_SIZE 1024
#define ASANA_URL_MAX_LENGTH 4096

typedef struct Response {
  int status;
  size_t len;
  char *body;
} Response;


/**
 * initialize libcurl
 */
bool asana_init();

/**
 * cleanup libcurl before program end
 */
void asana_cleanup();

/**
 *
 */
Response *asana_fetch(char *path);

/**
 * Frees memory allocated for a Response
 */
void asana_free_response(Response *r);

#endif // ASANA_FETCH_H_
