
#include "co_position.h"
#include <flecs.h>
#include <raylib.h>

#define GRID_SIZE 100
#define GRID_WIDTH 6
#define GRID_HEIGHT 6
#define MAX_ENTITIES_PER_CELL 128

typedef struct {
  int entities[MAX_ENTITIES_PER_CELL];
  int count;
} Cell;

typedef struct {
  Cell cells[GRID_WIDTH * GRID_HEIGHT];
} SpatialGrid;

static SpatialGrid grid;

static void grid_clear(SpatialGrid *grid) {
  for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++) {
    grid->cells[i].count = 0;
  }
}

void sy_draw_grid() {
  for (int y = 0; y <= GRID_HEIGHT; y++) {
    int yPos = y * GRID_SIZE;
    DrawLine(0, yPos, GRID_WIDTH * GRID_SIZE, yPos, GRAY);
  }

  for (int x = 0; x <= GRID_WIDTH; x++) {
    int xPos = x * GRID_SIZE;
    DrawLine(xPos, 0, xPos, GRID_HEIGHT * GRID_SIZE, GRAY);
  }
}

void sy_collider_check(ecs_iter_t *it) {
  const co_position *p = ecs_field(it, co_position, 0);
  const Rectangle *r = ecs_field(it, Rectangle, 1);

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
