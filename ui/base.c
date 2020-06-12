#include "base.h"

#include <curses.h>
#include <stdbool.h>
#include <string.h>

#include "commands.h"
#include "model.h"

void draw_status_line(ui_state *state) {
  int old_x = state->curs_x;
  int old_y = state->curs_y;

  state->curs_y = LINES - 1;
  state->curs_x = 0;

  if (state->mode == NORMAL) {
    draw_text("NORMAL", state);
  } else {
    draw_text(":", state);
    draw_text(state->input_buffer, state);
  }
  clrtoeol();

  state->curs_x = old_x;
  state->curs_y = old_y;
}

void draw_text(const char *text, ui_state *state) {
  if (!text || strnlen(text, 1) == 0) return;

  mvaddch(state->curs_y, state->curs_x, *text);
  ++text;

  for (; *text != '\0'; ++text) {
    addch(*text);
  }

  getyx(stdscr, state->curs_y, state->curs_x);
}
