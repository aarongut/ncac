#include "base.h"

#include <curses.h>
#include <string.h>

#include <string>

namespace ui {
  void draw_text(const std::string text, int *x, int *y) {
    if (!text.length()) return;

    auto iter = text.begin();

    mvaddch(*y, *x, *iter);
    ++iter;

    for (; iter != text.end(); ++iter) {
      addch(*iter);
    }

    getyx(stdscr, *y, *x);
  }
}
