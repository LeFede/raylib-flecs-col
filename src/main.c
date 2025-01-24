#include <flecs.h>
#include <flecs/addons/flecs_c.h>
#include <flecs/private/addons.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Collider {
  int x, y, w, h;
} Collider;

typedef struct Position {
  int x, y;
} Position;

#define GRID_SIZE 50              // Tamaño de cada celda
#define GRID_WIDTH 50             // Número de celdas horizontales
#define GRID_HEIGHT 50            // Número de celdas verticales
#define MAX_ENTITIES_PER_CELL 128 // Máximo de entidades por celda
//
typedef struct {
  int entities[MAX_ENTITIES_PER_CELL]; // Índices de las entidades en esta celda
  int count;                           // Número de entidades en esta celda
} Cell;

typedef struct {
  Cell cells[GRID_WIDTH * GRID_HEIGHT]; // El grid como un array de celdas
} SpatialGrid;

static SpatialGrid grid;

void grid_clear(SpatialGrid *grid) {
  for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++) {
    grid->cells[i].count = 0;
  }
}

void sy_collider_check(ecs_iter_t *it) {
  const Position *p = ecs_field(it, Position, 0);
  const Rectangle *r = ecs_field(it, Rectangle, 1);

  // Limpiar el grid antes de asignar entidades
  grid_clear(&grid);

  // Paso 1: Asignar cada entidad a las celdas del grid
  for (int i = 0; i < it->count; i++) {
    int x = p[i].x + r[i].x;
    int y = p[i].y + r[i].y;
    int w = r[i].width;
    int h = r[i].height;

    // Calcular el rango de celdas afectadas
    int start_cell_x = x / GRID_SIZE;
    int start_cell_y = y / GRID_SIZE;
    int end_cell_x = (x + w - 1) / GRID_SIZE;
    int end_cell_y = (y + h - 1) / GRID_SIZE;

    // Iterar sobre las celdas afectadas y añadir la entidad
    for (int cell_y = start_cell_y; cell_y <= end_cell_y; cell_y++) {
      for (int cell_x = start_cell_x; cell_x <= end_cell_x; cell_x++) {
        int key = cell_y * GRID_WIDTH + cell_x;

        // Verificar que la celda esté dentro del rango válido
        if (cell_x < 0 || cell_x >= GRID_WIDTH || cell_y < 0 ||
            cell_y >= GRID_HEIGHT) {
          continue;
        }

        Cell *cell = &grid.cells[key];
        if (cell->count < MAX_ENTITIES_PER_CELL) {
          cell->entities[cell->count++] = i;
        } else {
          fprintf(stderr,
                  "Advertencia: Celda %d llena, entidad %d no agregada\n", key,
                  i);
        }
      }
    }
  }

  // Paso 2: Imprimir celdas con al menos una entidad
  printf("Celdas con entidades:\n");
  for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++) {
    Cell *cell = &grid.cells[i];
    if (cell->count > 0) {
      printf("Celda %d: %d entidad(es)\n", i, cell->count);
    }
  }

  // Paso 3: Verificar colisiones entre entidades en cada celda
  for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++) {
    Cell *cell = &grid.cells[i];

    for (int j = 0; j < cell->count; j++) {
      int entityA = cell->entities[j];
      int xA = p[entityA].x + r[entityA].x;
      int yA = p[entityA].y + r[entityA].y;
      int wA = r[entityA].width;
      int hA = r[entityA].height;
      Rectangle boxA = {xA, yA, wA, hA};

      for (int k = j + 1; k < cell->count; k++) {
        int entityB = cell->entities[k];
        int xB = p[entityB].x + r[entityB].x;
        int yB = p[entityB].y + r[entityB].y;
        int wB = r[entityB].width;
        int hB = r[entityB].height;
        Rectangle boxB = {xB, yB, wB, hB};

        if (CheckCollisionRecs(boxA, boxB)) {
          printf("Collision detected between entity %d and entity %d\n",
                 entityA, entityB);
        }
      }
    }
  }
}

void sy_draw_grid() {
  for (int y = 0; y <= GRID_HEIGHT; y++) {
    int yPos = y * GRID_SIZE;
    DrawLine(0, yPos, GRID_WIDTH * GRID_SIZE, yPos,
             GRAY); // Líneas horizontales
  }

  for (int x = 0; x <= GRID_WIDTH; x++) {
    int xPos = x * GRID_SIZE;
    DrawLine(xPos, 0, xPos, GRID_HEIGHT * GRID_SIZE, GRAY); // Líneas verticales
  }
}

void sy_draw_begin() {
  BeginDrawing();
  ClearBackground((Color){60, 60, 100, 255});
}
void sy_draw_end() { EndDrawing(); }
void sy_draw_sprites(ecs_iter_t *it) {
  const Position *p = ecs_field(it, Position, 0);

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
}

void sy_move_right(ecs_iter_t *it) {
  Position *p = ecs_field(it, Position, 0);
  for (int i = 0; i < it->count; i++) {
    if (i == 1)
      continue;
    p[i].x += 1;
  }
}

int main(int argc, char *argv[]) {
  InitWindow(1280, 720, "Hola");
  SetTargetFPS(60);

  ecs_world_t *world = ecs_init();

  ECS_COMPONENT(world, Collider);
  ECS_COMPONENT(world, Rectangle);
  ECS_COMPONENT(world, Position);
  ECS_PREFAB(world, pf_entity_1, Position, Rectangle);
  ECS_SYSTEM(world, sy_move_right, EcsOnUpdate, Position);
  ECS_SYSTEM(world, sy_draw_begin, EcsOnUpdate);
  ECS_SYSTEM(world, sy_draw_grid, EcsOnUpdate);
  ECS_SYSTEM(world, sy_draw_sprites, EcsOnUpdate, Position);
  ECS_SYSTEM(world, sy_collider_check, EcsOnUpdate, Position, Rectangle);
  ECS_SYSTEM(world, sy_draw_end, EcsOnUpdate);
  ecs_set(world, pf_entity_1, Rectangle, {0, 0, 20, 20});
  ecs_set(world, pf_entity_1, Position, {});

  ecs_entity_t entity_1 = ecs_new_w_pair(world, EcsIsA, pf_entity_1);
  ecs_entity_t entity_2 = ecs_new_w_pair(world, EcsIsA, pf_entity_1);
  ecs_entity_t entity_3 = ecs_new_w_pair(world, EcsIsA, pf_entity_1);
  ecs_entity_t entity_4 = ecs_new_w_pair(world, EcsIsA, pf_entity_1);
  ecs_set(world, entity_1, Position, {100, 30});
  ecs_set(world, entity_2, Position, {500, 30});
  ecs_set(world, entity_3, Position, {200, 30});
  ecs_set(world, entity_4, Position, {300, 30});

  float delta = 0.0f;
  while (!WindowShouldClose()) {
    delta = GetFPS();
    ecs_progress(world, delta);
  }

  ecs_fini(world);
  CloseWindow();
  return 0;
}
