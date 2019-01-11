#include "ncac.h"

#include <curses.h>
#include <stdlib.h>
#include <term.h>

#include <iostream>

#include "ui/base.h"
#include "asana/fetch.h"


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
      case 'q':
        finish(SIGTERM);
        break;
    }
  }
  return 0;
}

static void finish(int sig) {
  endwin();
  asana::deinit();
  exit(0);
}

static void setup() {
  // initialize the asana client
  if (!asana::init()) {
    std::cerr << "Unable to initialize the Asana client\n" << EOF;
    exit(1);
  }

  // initialize ncurses
  if (!initscr()) {
    std::cerr << "Unable to initialize the curses screen.\n" << EOF;
    exit(1);
  }

  // don't buffer or echo input
  if (cbreak() == ERR) {
    std::cerr << "Unable to initialize.\n" << EOF;
    exit(1);
  }
  noecho();
  nonl();

  // install handlers
  signal(SIGINT, &finish);
  signal(SIGKILL, &finish);
  signal(SIGTERM, &finish);
}
