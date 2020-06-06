#include "ncac.h"

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <term.h>

#include "asana/asana.h"
#include "asana/fetch.h"
#include "ui/base.h"

int main(/*int argc, char **argv*/) {
  setup();

  int curs_x = 0;
  int curs_y = 0;

  char input;

  while (1) {
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

static void finish(/*int sig*/) {
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
  User me;
  user_info(&me);

  if (me.workspaces == NULL || me.workspaces_len == 0) {
    fprintf(stderr, "Unable to get workspaces.\n");
    return;
  }

  char gid[64];
  gid[0] = '\0';

  user_task_list_gid(me.workspaces[0].gid, gid);
  fprintf(stderr, "Got a task list? %s\n", gid);

  *curs_x = 0;
  (*curs_y)++;
  draw_text(gid, curs_x, curs_y);
}
