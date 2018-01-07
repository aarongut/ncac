#ifndef _NCAC_H_
#define _NCAC_H_

/**
 * Cleans up before exit. Installed on SIGINT
 */
void finish();

/**
 * Sets up ncurses and internal data structures
 */
void setup();


#endif // _NCAC_H_
