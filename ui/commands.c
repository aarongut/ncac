#include "commands.h"

#include <curses.h>
#include <stdbool.h>

#include "../asana/asana.h"
#include "base.h"

bool ui_quit() { return true; }

bool ui_help(ui_state *state) {
  erase();
  state->curs_x = 0;
  state->curs_y = 0;

  draw_text("Commands:", state);

  state->curs_x = 0;
  state->curs_y += 2;

  for (size_t i = 0; i < UI_NUM_COMMANDS; i++) {
    draw_text("\t", state);
    draw_text(ui_commands[i].command, state);
    draw_text("\t", state);
    draw_text(ui_commands[i].help_text, state);

    state->curs_x = 0;
    state->curs_y++;
  }

  return false;
}

bool ui_mytasks(ui_state *state) {
  state->curs_x = 0;
  state->curs_y = 0;
  draw_text("Fetching user...", state);
  clrtobot();
  refresh();

  /*
  User me;
  user_info(&me);

  if (me.workspaces == NULL || me.workspaces_len == 0) {
    fprintf(stderr, "Unable to get workspaces.\n");
    return true;
  }
  */

  state->curs_x = 0;
  state->curs_y = 0;
  draw_text("Fetching user task list ID...", state);
  clrtobot();
  refresh();

  char gid[64];
  gid[0] = '\0';
  if (user_task_list_gid(
          /*me.workspaces[me.workspaces_len-1].gid*/ "15793206719", gid) !=
      ASANA_ERR_OK) {
    fprintf(stderr, "Unable to get task list ID. %s\n", gid);
    return true;
  }

  state->curs_x = 0;
  state->curs_y = 0;
  draw_text("Fetching user task list", state);
  clrtobot();
  refresh();

  Project my_tasks;
  if (user_task_list(gid, &my_tasks) != ASANA_ERR_OK) {
    fprintf(stderr, "Unable to get task list.\n");
    return true;
  }

  state->curs_x = 0;
  state->curs_y = 0;
  draw_text("My Tasks", state);
  clrtoeol();

  for (size_t i = 0; i < my_tasks.tasks_len; i++) {
    state->curs_y++;
    state->curs_x = 0;
    draw_text(my_tasks.tasks[i].name, state);
    clrtoeol();
  }

  clrtobot();

  return false;
}

ui_command ui_commands[UI_NUM_COMMANDS] = {
    {"help", "print this help message", ui_help},
    {"q", "quit the application", ui_quit},
    {"mytasks", "fetch and display your task list", ui_mytasks}};
