#include "systems.h"

void sy_debug(ecs_iter_t *it) {
  // co_Position *p = ecs_field(it, co_Position, 0);
  // for (int i = 0; i < it->count; i++) {
  //   DrawRectangle(p[i].x, p[i].y, 1, 1, (Color){255, 0, 0, 255});
  //   DrawRectangle(p[i].x, p[i].y, 11, 11, (Color){255, 0, 0, 255});
  // }
  const si_camera *c = ecs_singleton_get(it->world, si_camera);
  const si_input *input = ecs_singleton_get(it->world, si_input);
  const int fps_number = GetFPS();

  DrawRectangle(0, 0, 250, 600, (Color){20, 20, 20, 50});
  DrawText(TextFormat("Delta: %f\n", it->delta_time), 20, 20, 25,
           (Color){255, 255, 255, 255});
  DrawText(TextFormat("Fps: %d\n", fps_number), 20, 45, 25,
           (Color){255, 255, 255, 255});
  DrawText(TextFormat("Zoom: %.2f\n", c->main.zoom), 20, 80, 25,
           (Color){255, 255, 255, 255});
  DrawText(TextFormat("Rotation: %.2f\n", c->main.rotation), 20, 105, 25,
           (Color){255, 255, 255, 255});
  DrawText(
      TextFormat("Q:%d\nE:%d\nW:%d\nA:%d\nS:%d\nD:%d\nCTRL:%d\nSPACE:%"
                 "d\nENTER:%d\nALT:%d\nUP:%d\nLEFT:%d\nDOWN:%d\nRIGHT:%d\n",
                 input->q, input->e, input->w, input->a, input->s, input->d,
                 input->left_ctrl, input->space, input->enter, input->alt,
                 input->up, input->left, input->down, input->right),
      20, 160, 25, (Color){255, 255, 255, 255});
  DrawLine(0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT / 2,
           (Color){0, 255, 0, 100});
  DrawLine(WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT,
           (Color){0, 255, 0, 100});
}
