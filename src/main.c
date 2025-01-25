#include "components/co_rigidbody.h"
#include "components/co_sprite.h"
#include "components/co_velocity.h"
#include "components/components.h"
#include "constants.h"
#include "systems/systems.h"
#include <flecs.h>
#include <flecs/addons/flecs_c.h>
#include <flecs/private/addons.h>
#include <raylib.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
  /* ══════════════════════════ Setup ═══════════════════════════════════════ */
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(60);
  ecs_world_t *world = ecs_init();

  /* ══════════════════════════ Components ══════════════════════════════════ */
  ECS_COMPONENT_DEFINE(world, co_rigidbody);
  ECS_COMPONENT_DEFINE(world, co_position);
  ECS_COMPONENT_DEFINE(world, co_sprite);
  ECS_COMPONENT_DEFINE(world, co_velocity);
  ECS_COMPONENT_DEFINE(world, si_camera);
  ECS_COMPONENT_DEFINE(world, si_input);
  ECS_COMPONENT_DEFINE(world, si_assets);

  /* ══════════════════════════ Singletons ══════════════════════════════════ */
  ecs_singleton_add(world, si_camera);
  ecs_singleton_add(world, si_input);
  ecs_singleton_add(world, si_assets);

  /* ══════════════════════════ Prefabs ═════════════════════════════════════ */
  ECS_PREFAB(world, pf_entity_1, co_position, co_rigidbody, co_velocity);
  ecs_set(world, pf_entity_1, co_rigidbody, {-7, -15, 16, 16});
  ecs_set(world, pf_entity_1, co_position, {0, 30});
  ecs_set(world, pf_entity_1, co_velocity, {0, 0});
  ecs_set(
      world, pf_entity_1, co_sprite,
      {.idx = t_ASEPRITE, .origin = {7, 15}, .color = {255, 255, 255, 255}});

  // co_Sprite s = {.idx = t_GRID,
  //                .origin = (Vector2){0, 0},
  //                .color = (Color){255, 255, 255, 50}};

  /* ══════════════════════════ Observers ═══════════════════════════════════ */
  ECS_OBSERVER(world, sy_on_position_set, EcsOnSet, co_position);

  /* ══════════════════════════ Start ═══════════════════════════════════════ */
  ECS_SYSTEM(world, sy_camera_init, EcsOnStart, si_camera);
  ECS_SYSTEM(world, sy_load_assets, EcsOnStart, si_assets);
  /****************************************************************************/
  /****************************************************************************/
  /****************************************************************************/
  /****************************************************************************/
  /****************************************************************************/
  /****************************************************************************/

  /* ══════════════════════════ Early Update ════════════════════════════════ */
  ECS_SYSTEM(world, sy_control_camera, EcsOnUpdate, si_camera);
  ECS_SYSTEM(world, sy_input, EcsOnUpdate, si_input);

  /* ══════════════════════════ Update ══════════════════════════════════════ */
  ECS_SYSTEM(world, sy_physics, EcsOnUpdate, co_position, co_velocity);

  /* ══════════════════════════ Late Update ═════════════════════════════════ */
  ECS_SYSTEM(world, sy_collider_check, EcsOnUpdate, co_position, co_rigidbody);
  /****************************************************************************/
  /****************************************************************************/
  /****************************************************************************/
  /****************************************************************************/
  /****************************************************************************/
  /****************************************************************************/

  /* ═════════════════════════ Draw Begin ═══════════════════════════════════ */
  ECS_SYSTEM(world, sy_draw_begin, EcsOnUpdate);
  /* ═════════════════════════ 2D Mode ══════════════════════════════════════ */
  ECS_SYSTEM(world, sy_draw_2d_begin, EcsOnUpdate);
  ECS_SYSTEM(world, sy_draw_grid, EcsOnUpdate);
  ECS_SYSTEM(world, sy_draw_sprites, EcsOnUpdate, co_position, co_sprite);
  ECS_SYSTEM(world, sy_draw_debug, EcsOnUpdate, co_position, co_rigidbody);
  ECS_SYSTEM(world, sy_draw_2d_end, EcsOnUpdate);
  /* ═════════════════════════ UI ═══════════════════════════════════════════ */
  ECS_SYSTEM(world, sy_debug, EcsOnUpdate);
  ECS_SYSTEM(world, sy_draw_end, EcsOnUpdate);
  /****************************************************************************/
  /****************************************************************************/
  /****************************************************************************/
  /****************************************************************************/
  /****************************************************************************/
  /****************************************************************************/

  /* ═════════════════════════ Test ═════════════════════════════════════════ */
  ecs_entity_t entity_1 = ecs_new_w_pair(world, EcsIsA, pf_entity_1);
  ecs_entity_t entity_3 = ecs_new_w_pair(world, EcsIsA, pf_entity_1);
  ecs_set(world, entity_1, co_position, {0, 0});
  ecs_set(world, entity_1, co_velocity, {100, 0});
  ecs_set(world, entity_3, co_position, {130, 0});
  ecs_set(world, entity_3, co_velocity, {-100, 0});
  co_rigidbody *rb = ecs_get_mut(world, entity_3, co_rigidbody);
  if (rb) {
    rb->is_solid = true;
  }

  // ecs_set(world, r*b, {});
  // ecs_set(world, entity_3, co_rigidbody,
  //         {.rect = {-7, -15, 16, 16}, .is_solid = true});
  /****************************************************************************/
  /****************************************************************************/
  /****************************************************************************/
  /****************************************************************************/
  /****************************************************************************/
  /****************************************************************************/

  /* ══════════════════════════ Main Loop ═══════════════════════════════════ */
  float delta = 0.0f;
  while (!WindowShouldClose()) {
    delta = GetFrameTime();
    ecs_progress(world, delta);
  }

  /* ══════════════════════════ CleanUp ═════════════════════════════════════ */
  ecs_fini(world);
  CloseWindow();
  return 0;
}
