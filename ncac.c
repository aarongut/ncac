#include "ncac.h"

#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <term.h>

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
        draw_text("Hello, world!", &curs_x, &curs_y);
        break;
      case 'b':
        draw_text("Goodbye, world!", &curs_x, &curs_y);
        break;
      case 'j':
        get_me(&curs_x, &curs_y);
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
  asana_cleanup();
  exit(0);
}

static void setup() {
  char *pat = getenv("ASANA_PAT");
  if (!pat) {
    fprintf(stderr, "ASANA_PAT is not defined\n");
    exit(1);
  }

  // initialize the asana client
  if (!asana_init(pat)) {
    fprintf(stderr, "Unable to initialize the Asana client\n");
    exit(1);
  }

  // initialize ncurses
  if (!initscr()) {
    fprintf(stderr, "Unable to initialize the curses screen.\n");
    exit(1);
  }

  // don't buffer or echo input
  if (cbreak() == ERR) {
    fprintf(stderr, "Unable to initialize.\n");
    exit(1);
  }
  noecho();
  nonl();

  // install handlers
  signal(SIGINT, &finish);
  signal(SIGKILL, &finish);
  signal(SIGTERM, &finish);
}

void get_me(int *curs_x, int *curs_y) {
  Response *response = asana_fetch("users/me");

  *curs_x = 0;
  (*curs_y)++;
  draw_text(response->body, curs_x, curs_y);

  asana_free_response(response);
}
