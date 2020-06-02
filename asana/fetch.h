#ifndef ASANA_FETCH_H_
#define ASANA_FETCH_H_

#include <stdbool.h>

/**
 * initialize libcurl
 */
bool asana_init();

/**
 * cleanup libcurl before program end
 */
void asana_cleanup();

#endif // ASANA_FETCH_H_
