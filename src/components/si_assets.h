#ifndef _SI_ASSETS_
#define _SI_ASSETS_

#include <flecs.h>
#include <raylib.h>

typedef struct si_assets {
  Texture2D textures[10];
} si_assets;

extern ECS_COMPONENT_DECLARE(si_assets);
#endif
