#include "systems.h"
#include <raylib.h>

void sy_physics(ecs_iter_t *it) {
  co_position *p = ecs_field(it, co_position, 0);
  co_velocity *v = ecs_field(it, co_velocity, 1);

  for (int i = 0; i < it->count; i++) {
    p[i].__pre_x = p[i].x;
    p[i].__pre_y = p[i].y;
    p[i].x += v[i].x * it->delta_time;
    p[i].y += v[i].y * it->delta_time;
  }
}
