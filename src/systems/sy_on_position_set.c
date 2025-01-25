#include "systems.h"

void sy_on_position_set(ecs_iter_t *it) {
  co_position *p = ecs_field(it, co_position, 0);

  for (int i = 0; i < it->count; i++) {
    p[i].__pre_x = p[i].x;
    p[i].__pre_y = p[i].y;
  }
}
