#include "ncac.h"

#include <curses.h>
#include <stdlib.h>
#include <term.h>

#include "ui/base.h"


int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  setup();

  char input;

  int curs_x = 0;
  int curs_y = 0;

  while(1) {
    switch (input = getch()) {
      case 'a':
        draw_text(&curs_x, &curs_y, "Hello, world!");
        break;
      default:
        finish(0);
        break;
    }
  }
  return 0;
}

void finish(int sig) {
  (void)sig;
  endwin();
  exit(0);
}

void setup() {
  // initialize ncurses
  initscr();

  cbreak();



  // don't echo input
  noecho();

  nonl();

  // install handlers
  signal(SIGINT, &finish);
}
