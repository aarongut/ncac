#ifndef UI_BASE_H_
#define UI_BASE_H_

#include <stdbool.h>

#include "model.h"

/**
 * Prints the string text starting at position *x, *y. x and y are updated to
 * the position after the last character
 */
void draw_text(const char *text, ui_state *state);

/**
 * Render the input status bar
 */
void draw_status_line(ui_state *state);

#endif // UI_BASE_H_
