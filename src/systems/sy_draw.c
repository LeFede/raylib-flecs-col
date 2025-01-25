#include "systems.h"
#include <raylib.h>

void sy_draw_begin(ecs_iter_t *it) {
  BeginDrawing();
  ClearBackground(WINDOW_BACKGROUND);
}

void sy_draw_2d_begin(ecs_iter_t *it) {
  const si_camera *c = ecs_singleton_get(it->world, si_camera);
  BeginMode2D(c->main);
}

void sy_draw_end() { EndDrawing(); }
void sy_draw_2d_end(ecs_iter_t *it) { EndMode2D(); }

void sy_draw_sprites(ecs_iter_t *it) {

  const si_assets *a = ecs_singleton_get(it->world, si_assets);

  co_position *p = ecs_field(it, co_position, 0);
  co_sprite *s = ecs_field(it, co_sprite, 1);
  for (int i = 0; i < it->count; i++) {
    DrawTexture(a->textures[s[i].idx], p[i].x - s[i].origin.x,
                p[i].y - s[i].origin.y, s[i].color);
  }
}

void sy_draw_debug(ecs_iter_t *it) {
  co_position *p = ecs_field(it, co_position, 0);
  co_rigidbody *r = ecs_field(it, co_rigidbody, 1);

  for (int i = 0; i < it->count; i++) {
    DrawRectangle(p[i].x + r[i].rect.x, p[i].y + r[i].rect.y, r[i].rect.width, r[i].rect.height,
                  (Color){0, 200, 30, 100});
    DrawRectangle(p[i].x, p[i].y, 1, 1, RED);
  }
}

// void sy_draw_sprites(ecs_iter_t *it) {
//   const co_position *p = ecs_field(it, co_position, 0);
//
//   for (int i = 0; i < it->count; i++) {
//     Color c;
//     switch (i) {
//     case 0:
//       c = (Color){255, 0, 0, 255};
//       break;
//
//     case 1:
//       c = (Color){0, 255, 0, 255};
//       break;
//
//     case 2:
//       c = (Color){0, 0, 255, 255};
//       break;
//
//     default:
//       c = (Color){255, 0, 255, 255};
//       break;
//     }
//     DrawRectangle(p[i].x, p[i].y, 20, 20, c);
//   }
// }
