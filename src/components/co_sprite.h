
#ifndef _CO_SPRITE_
#define _CO_SPRITE_
#include <flecs.h>
#include <raylib.h>

typedef struct co_sprite {
  int idx;
  Vector2 origin;
  Color color;
} co_sprite;

extern ECS_COMPONENT_DECLARE(co_sprite);
#endif
