#include "systems.h"

void sy_control_camera(ecs_iter_t *it) {
  si_camera *c = ecs_field(it, si_camera, 0);
  const si_input *input = ecs_singleton_get(it->world, si_input);

  c->main.zoom += c->zoom_speed * GetFrameTime() * (input->w - input->s);
  c->main.rotation += c->rotate_speed * GetFrameTime() * (input->a - input->d);
}
