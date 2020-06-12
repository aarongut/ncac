#ifndef UI_MODEL_H_
#define UI_MODEL_H_

#include <stdlib.h>

enum UI_MODE {
  NORMAL,
  COMMAND,
};

#define UI_INPUT_BUFFER_SIZE 4096

typedef struct ui_state {
  int curs_x;
  int curs_y;
  enum UI_MODE mode;
  size_t buffer_size;
  char input_buffer[UI_INPUT_BUFFER_SIZE];
} ui_state;

#endif // UI_MODEL_H_
