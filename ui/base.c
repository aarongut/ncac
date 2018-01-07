#include "base.h"

#include <curses.h>
#include <string.h>

void draw_text(int *x, int *y, char *text) {
  size_t len = strlen(text);
  if (!len) return;

  mvaddch(*x, *y, text[0]);

  for (size_t i=1; i < strlen(text); i++) {
    addch(text[i]);
  }

  getyx(stdscr, *y, *x);
}
