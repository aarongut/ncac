#ifndef UI_COMMANDS_H_
#define UI_COMMANDS_H_

#include <stdbool.h>

#include "model.h"

typedef struct ui_command {
  const char *command;
  const char *help_text;
  bool (*handler)(ui_state *);
} ui_command;

#define UI_NUM_COMMANDS 3
extern ui_command ui_commands[];

bool handle_command(ui_state *state);

#endif // UI_COMMANDS_H_
