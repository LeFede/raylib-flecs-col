#ifndef _SI_INPUT_
#define _SI_INPUT_

#include "../constants.h"
#include <flecs.h>
#include <raylib.h>

typedef struct si_input {
  u32 a : 1, s : 1, d : 1, w : 1, space : 1, left_ctrl : 1, enter : 1, alt : 1,
      q : 1, e : 1, up : 1, down : 1, left : 1, right : 1;
} si_input;

extern ECS_COMPONENT_DECLARE(si_input);
#endif
