#ifndef _SYSTEMS_H
#define _SYSTEMS_H

#include "../components/components.h"
#include "../constants.h"
#include <flecs.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sy_camera_init(ecs_iter_t *it);
void sy_collider_check(ecs_iter_t *it);
void sy_draw_begin(ecs_iter_t *it);
void sy_draw_end();
void sy_draw_grid();
void sy_draw_sprites(ecs_iter_t *it);
void sy_on_position_set(ecs_iter_t *it);
void sy_control_camera(ecs_iter_t *it);

void sy_physics(ecs_iter_t *it);
void sy_draw_debug(ecs_iter_t *it);

void sy_draw_2d_begin(ecs_iter_t *it);
void sy_draw_2d_end(ecs_iter_t *it);

void sy_input(ecs_iter_t *it);
void sy_debug(ecs_iter_t *it);
void sy_load_assets(ecs_iter_t *it);

#endif
