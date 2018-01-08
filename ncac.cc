#include "ncac.h"

#include <curses.h>
#include <stdlib.h>
#include <term.h>

#include "ui/base.h"


int main(int argc, char **argv) {
  setup();

  int curs_x = 0;
  int curs_y = 0;

  char input;

  while(1) {
    switch (input = getch()) {
      case 'a':
        ui::draw_text("Hello, world!", &curs_x, &curs_y);
        break;
      case 'b':
        ui::draw_text("Goodbye, world!", &curs_x, &curs_y);
        break;
      default:
        finish(SIGTERM);
        break;
    }
  }
  return 0;
}

static void finish(int sig) {
  endwin();
  exit(0);
}

static void setup() {
  // initialize ncurses
  initscr();
  cbreak();

  // don't echo input
  noecho();
  nonl();

  // install handlers
  signal(SIGINT, &finish);
  signal(SIGKILL, &finish);
  signal(SIGTERM, &finish);
}
