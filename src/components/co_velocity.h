#ifndef _CO_velocity_H
#define _CO_velocity_H
#include <flecs.h>

typedef struct co_velocity {
  int x, y;
} co_velocity;

extern ECS_COMPONENT_DECLARE(co_velocity);
#endif
