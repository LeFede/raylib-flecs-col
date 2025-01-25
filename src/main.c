#include "co_position.h"
#include "co_rigidbody.h"
#include "sy_collider_check.h"
#include <flecs.h>
#include <flecs/addons/flecs_c.h>
#include <flecs/private/addons.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

ECS_COMPONENT_DECLARE(co_position);
ECS_COMPONENT_DECLARE(co_rigidbody);

typedef struct Collider {
  int x, y, w, h;
} Collider;

Camera2D camera = {0};

void sy_draw_begin() {
  printf("draw begin ==================================\n");
  BeginDrawing();
  BeginMode2D(camera);
  ClearBackground((Color){60, 60, 100, 255});
}

void sy_draw_end() {
  printf("draw end ==================================\n");
  EndDrawing();
}

void sy_draw_sprites(ecs_iter_t *it) {
  const co_position *p = ecs_field(it, co_position, 0);

  for (int i = 0; i < it->count; i++) {
    Color c;
    switch (i) {
    case 0:
      c = (Color){255, 0, 0, 255};
      break;

    case 1:
      c = (Color){0, 255, 0, 255};
      break;

    case 2:
      c = (Color){0, 0, 255, 255};
      break;

    default:
      c = (Color){255, 0, 255, 255};
      break;
    }
    DrawRectangle(p[i].x, p[i].y, 20, 20, c);
  }
  printf("dibujo\n");
}

void sy_move_right(ecs_iter_t *it) {
  co_position *p = ecs_field(it, co_position, 0);
  for (int i = 0; i < it->count; i++) {
    if (i == 1)
      continue;
    if (i == 2) {
      p[i].__pre_x = p[i].x;
      p[i].__pre_y = p[i].y;
      p[i].x -= 1;
      continue;
    }
    p[i].__pre_x = p[i].x;
    p[i].__pre_y = p[i].y;
    p[i].x += 1;
  }
}

void sy_on_position_set(ecs_iter_t *it) {
  co_position *p = ecs_field(it, co_position, 0);

  for (int i = 0; i < it->count; i++) {
    p[i].__pre_x = p[i].x;
    p[i].__pre_y = p[i].y;
  }
}

int main(int argc, char *argv[]) {
  InitWindow(1600, 900, "Hola");
  SetTargetFPS(60);
  camera.zoom = 8;

  ecs_world_t *world = ecs_init();

  ECS_COMPONENT(world, Collider);
  ECS_COMPONENT(world, co_rigidbody);
  ECS_COMPONENT(world, co_position);
  ECS_OBSERVER(world, sy_on_position_set, EcsOnSet, co_position);
  ECS_SYSTEM(world, sy_move_right, EcsOnUpdate, co_position);
  ECS_SYSTEM(world, sy_collider_check, EcsOnUpdate, co_position, co_rigidbody);
  ECS_SYSTEM(world, sy_draw_begin, EcsOnUpdate);
  ECS_SYSTEM(world, sy_draw_grid, EcsOnUpdate);
  ECS_SYSTEM(world, sy_draw_sprites, EcsOnUpdate, co_position);
  ECS_SYSTEM(world, sy_draw_end, EcsOnUpdate);

  ECS_PREFAB(world, pf_entity_1, co_position, co_rigidbody);
  ecs_set(world, pf_entity_1, co_rigidbody, {0, 0, 20, 20});
  ecs_set(world, pf_entity_1, co_position, {0, 0});

  ecs_entity_t entity_1 = ecs_new_w_pair(world, EcsIsA, pf_entity_1);
  ecs_entity_t entity_2 = ecs_new_w_pair(world, EcsIsA, pf_entity_1);
  ecs_entity_t entity_3 = ecs_new_w_pair(world, EcsIsA, pf_entity_1);
  // ecs_entity_t entity_3 = ecs_new_w_pair(world, EcsIsA, pf_entity_1);
  // ecs_entity_t entity_4 = ecs_new_w_pair(world, EcsIsA, pf_entity_1);
  ecs_set(world, entity_1, co_position, {75, 30});
  // ecs_set(world, entity_2, co_position, {100, 30});
  ecs_set(world, entity_3, co_position, {150, 30});
  // ecs_set(world, entity_3, co_position, {40, 30});
  // ecs_set(world, entity_4, co_position, {0, 30});

  float delta = 0.0f;
  while (!WindowShouldClose()) {
    delta = GetFPS();
    ecs_progress(world, delta);
  }

  ecs_fini(world);
  CloseWindow();
  return 0;
}
