#ifndef _SI_camera_H
#define _SI_camera_H
#include <flecs.h>
#include <raylib.h>

typedef struct si_camera {
  Camera2D main;
  float zoom;
  float zoom_speed;
  float rotate_speed;
} si_camera;

extern ECS_COMPONENT_DECLARE(si_camera);
#endif
