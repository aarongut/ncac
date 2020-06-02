#include "base.h"

#include <curses.h>
#include <string.h>

void draw_text(char *text, int *x, int *y) {
  if (!text || strnlen(text, 1) == 0) return;

  mvaddch(*y, *x, *text);
  ++text;

  for (; *text != '\0'; ++text) {
    addch(*text);
  }

  getyx(stdscr, *y, *x);
}
