#ifndef _CO_rigidbody_H
#define _CO_rigidbody_H
#include <flecs.h>
#include <raylib.h>

typedef struct co_rigidbody {
  Rectangle rect;
  bool solid;
} co_rigidbody;

extern ECS_COMPONENT_DECLARE(co_rigidbody);
#endif
