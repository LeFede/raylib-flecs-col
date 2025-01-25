#include "systems.h"

void sy_camera_init(ecs_iter_t *it) {
  si_camera *c = ecs_field(it, si_camera, 0);
  c->main.rotation = 0.0f;
  c->main.zoom = WINDOW_ZOOM;
  c->main.offset = (Vector2){WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f};
  c->main.target = (Vector2){0.0f, 0.0f};
  c->zoom_speed = 4.0f;
  c->rotate_speed = 40.0f;
}
