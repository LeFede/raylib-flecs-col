#include "co_position.h"
#include "co_rigidbody.h"
#include <flecs.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLLISIONS 1024

typedef struct {
  int entityA;
  int entityB;
} Collision;

typedef struct {
  Collision collisions[MAX_COLLISIONS];
  int count;
} CollisionSet;

static CollisionSet previous_collisions;
static CollisionSet current_collisions;

// Inicializar un conjunto de colisiones
static void collision_set_clear(CollisionSet *set) { set->count = 0; }

// Agregar una colisión al conjunto
static bool collision_set_add(CollisionSet *set, int entityA, int entityB) {
  if (set->count >= MAX_COLLISIONS) {
    fprintf(stderr, "Advertencia: Se alcanzó el límite de colisiones\n");
    return false;
  }

  // Asegurar orden único: siempre entityA < entityB
  if (entityA > entityB) {
    int temp = entityA;
    entityA = entityB;
    entityB = temp;
  }

  // Verificar si ya existe
  for (int i = 0; i < set->count; i++) {
    if (set->collisions[i].entityA == entityA &&
        set->collisions[i].entityB == entityB) {
      return false; // Ya existe
    }
  }

  // Agregar al conjunto
  set->collisions[set->count++] = (Collision){entityA, entityB};
  return true;
}

// Verificar si una colisión está en el conjunto
static bool collision_set_contains(const CollisionSet *set, int entityA,
                                   int entityB) {
  if (entityA > entityB) {
    int temp = entityA;
    entityA = entityB;
    entityB = temp;
  }

  for (int i = 0; i < set->count; i++) {
    if (set->collisions[i].entityA == entityA &&
        set->collisions[i].entityB == entityB) {
      return true;
    }
  }
  return false;
}

#define GRID_SIZE 100
#define GRID_WIDTH 20
#define GRID_HEIGHT 20
#define MAX_ENTITIES_PER_CELL 10

typedef struct {
  int entities[MAX_ENTITIES_PER_CELL];
  int count;
} Cell;

typedef struct {
  Cell cells[GRID_WIDTH * GRID_HEIGHT];
  int non_empty_cells[GRID_WIDTH *
                      GRID_HEIGHT]; // Lista de índices de celdas no vacías
  int non_empty_count;              // Número de celdas no vacías
} SpatialGrid;

static SpatialGrid grid;

// Limpiar el grid (resetea conteos de celdas y lista de celdas no vacías)
static void grid_clear(SpatialGrid *grid) {
  grid->non_empty_count = 0;
  for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++) {
    grid->cells[i].count = 0;
  }
}

// Dibujar el grid en pantalla
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

// Verificar colisiones usando el grid espacial
void sy_collider_check(ecs_iter_t *it) {
  co_position *p = ecs_field(it, co_position, 0);
  co_rigidbody *rigidbody = ecs_field(it, co_rigidbody, 1);

  grid_clear(&grid);
  collision_set_clear(&current_collisions);

  Rectangle abs_boxes[it->count];
  for (int i = 0; i < it->count; i++) {
    abs_boxes[i].x = p[i].x + rigidbody[i].rect.x;
    abs_boxes[i].y = p[i].y + rigidbody[i].rect.y;
    abs_boxes[i].width = rigidbody[i].rect.width;
    abs_boxes[i].height = rigidbody[i].rect.height;
  }

  // Paso 1: Asignar entidades a las celdas del grid
  for (int i = 0; i < it->count; i++) {
    int x = abs_boxes[i].x;
    int y = abs_boxes[i].y;
    int w = abs_boxes[i].width;
    int h = abs_boxes[i].height;

    int start_cell_x = x / GRID_SIZE;
    int start_cell_y = y / GRID_SIZE;
    int end_cell_x = (x + w) / GRID_SIZE;
    int end_cell_y = (y + h) / GRID_SIZE;

    for (int cell_y = start_cell_y; cell_y <= end_cell_y; cell_y++) {
      for (int cell_x = start_cell_x; cell_x <= end_cell_x; cell_x++) {
        if (cell_x < 0 || cell_x >= GRID_WIDTH || cell_y < 0 ||
            cell_y >= GRID_HEIGHT) {
          continue;
        }

        int key = cell_y * GRID_WIDTH + cell_x;
        Cell *cell = &grid.cells[key];
        if (cell->count < MAX_ENTITIES_PER_CELL) {
          if (cell->count == 0) {
            grid.non_empty_cells[grid.non_empty_count++] = key;
          }
          cell->entities[cell->count++] = i; // Guardamos el índice local
        }
      }
    }
  }

  // Paso 2: Detectar y clasificar colisiones
  for (int i = 0; i < grid.non_empty_count; i++) {
    int cell_index = grid.non_empty_cells[i];
    Cell *cell = &grid.cells[cell_index];

    for (int j = 0; j < cell->count; j++) {
      int localA = cell->entities[j];
      int entityA = it->entities[localA]; // ID real de la entidad
      Rectangle boxA = abs_boxes[localA];

      for (int k = j + 1; k < cell->count; k++) {
        int localB = cell->entities[k];
        int entityB = it->entities[localB]; // ID real de la entidad
        Rectangle boxB = abs_boxes[localB];

        if (CheckCollisionRecs(boxA, boxB)) {
          bool is_new =
              !collision_set_contains(&previous_collisions, entityA, entityB);
          collision_set_add(&current_collisions, entityA, entityB);

          // Detectar cuál se está moviendo
          bool entityA_moved = (p[localA].x != p[localA].__pre_x) ||
                               (p[localA].y != p[localA].__pre_y);
          bool entityB_moved = (p[localB].x != p[localB].__pre_x) ||
                               (p[localB].y != p[localB].__pre_y);

          if (entityA_moved && !entityB_moved) {
            printf("Entity %d (moved) collided with %d (static)\n", entityA,
                   entityB);
            // Regresa a entityA a su posición previa
            p[localA].x = p[localA].__pre_x;
            p[localA].y = p[localA].__pre_y;
          } else if (entityB_moved && !entityA_moved) {
            printf("Entity %d (moved) collided with %d (static)\n", entityB,
                   entityA);
            // Regresa a entityB a su posición previa
            p[localB].x = p[localB].__pre_x;
            p[localB].y = p[localB].__pre_y;
          } else {
            // Ambos se movieron (puedes manejar este caso si es necesario)
            printf("Both entities %d and %d are moving\n", entityA, entityB);
            p[localA].x = p[localA].__pre_x;
            p[localA].y = p[localA].__pre_y;
            p[localB].x = p[localB].__pre_x;
            p[localB].y = p[localB].__pre_y;
          }
        }
      }
    }
  }

  // Paso 3: Detectar colisiones resueltas
  for (int i = 0; i < previous_collisions.count; i++) {
    Collision c = previous_collisions.collisions[i];
    if (!collision_set_contains(&current_collisions, c.entityA, c.entityB)) {
      printf("Collision exit %d %d\n", c.entityA, c.entityB);
    }
  }

  // Actualizar el conjunto de colisiones previas
  previous_collisions = current_collisions;
}
