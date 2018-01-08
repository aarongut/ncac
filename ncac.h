#ifndef _NCAC_H_
#define _NCAC_H_

/**
 * Cleans up before exit. Installed on SIGINT
 */
static void finish(int sig);

/**
 * Sets up ncurses and internal data structures
 */
static void setup();
#endif // _NCAC_H_
