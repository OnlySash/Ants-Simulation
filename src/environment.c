#include "../include/environment.h"

#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

#define WALL '#'
#define FLOOR ' '
#define FOOD '*'
#define ANT '&'

// "Private methods" for terrain generation
void field_build(environment_t *env);
void obstacle_add(environment_t *env);

void maze_build(environment_t *env);
void path_build(environment_t *env, int tile[2]);
int is_tile_available(environment_t *env, int tile[2]);

void environment_partial_dest(environment_t *env, int size);

// Initializes an environment with the given size and a randomly generated
// terrain
void environment_init(environment_t *env, int size, int type) {
  if (type < 0 || type >= ENVIRONMENT_COUNT) {
    printf("Invalid environment type selected."
           "Stopping environment generation\n");
    return;
  }

  // Set size and allocate first memory for terrain
  env->size = size;
  env->terrain = malloc(size * sizeof(char *));

  // Destroy if unable to allocate memory
  if (!env->terrain) {
    environment_partial_dest(env, 0);
    return;
  }

  // Allocate rest of memory for terrain
  for (int i = 0; i < size; ++i) {
    env->terrain[i] = malloc(size * sizeof(char));

    // Destroy if unable to allocate memory
    if (!env->terrain[i]) {
      environment_partial_dest(env, i);
      return;
    }
  }

  // Run a different generation algorithm according to the given type
  switch (type) {
  case ENVIRONMENT_FIELD:
    field_build(env);
    break;

  case ENVIRONMENT_MAZE:
    maze_build(env);
    break;
  }
}

// Print the environment
void environment_show(environment_t *env) {
  for (int i = 0; i < env->size; ++i) {
    for (int j = 0; j < env->size; ++j) {
      printf("%c%c", env->terrain[i][j], j < env->size - 1 ? ' ' : '\n');
    }
  }
}

int find_free_tile(environment_t *env, int pos[2]){
  // Busca espacio vacìo para colocar hormiga
  if (!env || !env->terrain) return 0;

  int attempts = 0;
  int max_attempts = env->size * env->size;

  do {
    pos[0] = rand() % env->size;
    pos[1] = rand() % env->size;
    attempts++;

    if (env->terrain[pos[0]][pos[1]] == FLOOR) {
      return 1; // encontrado
    }

  } while (attempts < max_attempts);

  return 0;
}

// Destroy the environment
void environment_dest(environment_t *env) {
  // Free terrain rows
  for (int i = 0; i < env->size; ++i) {
    free(env->terrain[i]);
  }

  free(env->terrain);
  env->size = 0;
}

// Build a mostly empty and open space with only a few obstacles
void field_build(environment_t *env) {
  // Set terrain borders
  for (int i = 0; i < env->size; ++i) {
    for (int j = 0; j < env->size; ++j) {
      int is_inner_edge = !(i && j);
      int is_outer_edge = !((i - env->size + 1) && (j - env->size + 1));
      int is_wall = is_inner_edge || is_outer_edge;

      env->terrain[i][j] = is_wall ? WALL : FLOOR;
    }
  }

  // Generate a random number of obstacles in the terrain
  srand(time(NULL));
  int num_obstacles = rand() % 4 + 2;

  for (int i = 0; i < num_obstacles; ++i) {
    obstacle_add(env);
  }
}

// Adds an obstacle of random size at a random location within the terrain
// Obstacles are circles filled with walls
// Obstacle radius is no smaller than size/12 and no larger than size/6
void obstacle_add(environment_t *env) {

  // Randomly select origin coordinates and radius of obstacle
  int o[2] = {rand() % env->size, rand() % env->size};
  int r = rand() % (env->size / 6) + (env->size / 12);

  // Fill a circle of the given radius at the given origin with walls
  for (int x = -r; x < r; ++x) {

    // If coordinate out of terrain bounds, continue
    if (o[0] + x < 0 || o[0] + x >= env->size) {
      continue;
    }

    for (int y = -r; y < r; ++y) {

      // If coordinate out of terrin bounds, continue
      if (o[1] + y < 0 || o[1] + y >= env->size) {
        continue;
      }

      // If coordinate within circle bounds, make wall
      if (x * x + y * y < r * r) {
        int p[2] = {o[0] + x, o[1] + y};
        env->terrain[p[0]][p[1]] = WALL;
      }
    }
  }
}

// Build a maze using recursive backtracking
void maze_build(environment_t *env) {
  // Fill terrain with walls
  for (int i = 0; i < env->size; ++i) {
    for (int j = 0; j < env->size; ++j) {
      env->terrain[i][j] = WALL;
    }
  }

  // Set the random seed, start in the middle of the terrain
  srand(time(NULL));
  int origin[2] = {env->size / 2, env->size / 2};

  path_build(env, origin);

  return;
}

// Convert the selected (wall) tile to floor and move to a random unoccupied
// neighbor
// If all neighbors are occupied, backtrack to the nearest tile with available
// neighbors
// The tiles are actually traversed in sets of two to allow for the permanence
// of walls
void path_build(environment_t *env, int tile[2]) {
  env->terrain[tile[0]][tile[1]] = FLOOR; // Convert tile

  // Run four times (one for each neighbor, in the worst case)
  for (int n = 0; n < 4; ++n) {
    int next[2] = {env->size / 2, env->size / 2}; // Known occupied tile
    int middle[2] = {0, 0};     // The middle coordinate between two tiles
    int axis = rand() % 2;      // Used as index for an axis
    int direction = rand() % 2; // Later to be converted to 1 or -1

    // If the selected tile is not available, try another one
    // First, the opposite one on the same axis, then the ones on the unselected
    // axis
    for (int i = 0; !is_tile_available(env, next) && i < 4; ++i) {
      (i) ? (i % 2) ? (axis = !axis) : (direction = !direction) : 0;
      next[axis] = tile[axis] + (4 * direction) - 2;
      next[!axis] = tile[!axis];
    }

    // If the tile is available, convert the middle one to floor and recursively
    // call this very function to convert the next tile and keep the search
    // going
    if (is_tile_available(env, next)) {
      middle[axis] = next[axis] - (2 * direction) + 1;
      middle[!axis] = next[!axis];

      env->terrain[middle[0]][middle[1]] = FLOOR;
      path_build(env, next);
    }
  }
}

// Makes sure the selected tile is inside the terrain boundaries and is not
// occupied already
int is_tile_available(environment_t *env, int tile[2]) {
  int is_in_lower_bound = tile[0] > 0 && tile[1] > 0;
  int is_in_upper_bound = tile[0] < env->size - 1 && tile[1] < env->size - 1;
  int is_in_bounds = is_in_lower_bound && is_in_upper_bound;

  if (!is_in_bounds) {
    return 0;
  }

  int is_free = env->terrain[tile[0]][tile[1]] != FLOOR;

  return is_free;
}

// Used when allocation of memory for terrain fails
// Frees any memory that was able to be allocated
void environment_partial_dest(environment_t *env, int size) {
  printf("Unable to fully allocate memory for terrain."
         "Stopping environment generation\n");

  for (int i = 0; i < size; ++i) {
    free(env->terrain[i]);
  }

  free(env->terrain);
  env->size = 0;
}
