#ifndef _CO_position_H
#define _CO_position_H
#include <flecs.h>

typedef struct co_position {
  int x, y, __pre_x, __pre_y;
} co_position;

extern ECS_COMPONENT_DECLARE(co_position);
#endif
