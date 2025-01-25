#include "systems.h"

#define FOLDER_ASSETS "assets/"

void sy_load_assets(ecs_iter_t *it) {
  si_assets *a = ecs_field(it, si_assets, 0);

  a->textures[t_GRID] = LoadTexture(FOLDER_ASSETS "grid.png");
  a->textures[t_ASEPRITE] = LoadTexture(FOLDER_ASSETS "aseprite.png");
}
