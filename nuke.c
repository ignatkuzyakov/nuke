#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS 10000
#define MAX_X 100
#define MAX_Y 100

struct coordinate {
  int x;
  int y;
};

struct coordinates {
  struct coordinate *cds;
  int size;
};

int is_target_in_radius(struct coordinate center, struct coordinate target,
                        int radius) {
  return ((target.x >= (center.x - radius)) &&
          (target.x <= (center.x + radius)) &&
          (target.y <= (center.y + radius)) &&
          (target.y >= (center.y - radius)))
             ? 1
             : 0;
}

int targets_in_radius(struct coordinate center, struct coordinates *coords,
                      int radius) {
  int i, targets = 0;
  for (i = 0; i < coords->size; i += 1)
    if (is_target_in_radius(center, coords->cds[i], radius))
      targets += 1;
  return targets;
}

struct coordinate find_optimal_coordinate(struct coordinates *coords,
                                          int radius) {
  int x, y, i, targets = 0, max_targets = 0;
  struct coordinate final_coordinate, tmp_c;
  for (x = 0; x < MAX_X; x += 1) {
    for (y = 0; y < MAX_Y; y += 1) {
      tmp_c.y = y;
      tmp_c.x = x;
      targets = targets_in_radius(tmp_c, coords, radius);
      if (targets > max_targets) {
        max_targets = targets;
        final_coordinate = tmp_c;
      }
      targets = 0;
    }
  }
  return final_coordinate;
}

struct coordinates *read_coordinates_from_file(const char *name) {
  FILE *f;
  int i;

  struct coordinates *coords =
      (struct coordinates *)calloc(1, sizeof(struct coordinates));
  if (!coords) {
    fprintf(stderr, "calloc error");
    abort();
  }

  coords->cds =
      (struct coordinate *)malloc(MAX_POINTS * sizeof(struct coordinate));
  if (!coords->cds) {
    fprintf(stderr, "malloc error");
    abort();
  }

  f = fopen(name, "r");
  if (!f) {
    perror("error opening file\n");
    abort();
  }

  for (i = 0;; i += 1) {
    int res;
    res = fscanf(f, "%d,%d", &(coords->cds[i].x), &(coords->cds[i].y));
    if (res == EOF)
      break;
    if (res != 2) {
      fprintf(stderr, "file have unknown symbol, example: 1,2\n");
      abort();
    }
  }
  coords->size = i;
  fclose(f);

  return coords;
}

int main(int argc, char **argv) {
  int radius, targets;
  char *endptr;
  struct coordinates *coords;
  struct coordinate res;

  if (argc < 3) {
    fprintf(stderr, "usage: %s <coords_file> <radius>\n", argv[0]);
    return 1;
  }

  radius = strtol(argv[2], &endptr, 10);
  if (endptr == argv[2]) {
    fprintf(stderr, "<%s> cannot be converted to integer\n", argv[2]);
    return 1;
  }

  coords = read_coordinates_from_file(argv[1]);

  res = find_optimal_coordinate(coords, radius);
  targets = targets_in_radius(res, coords, radius);

  printf("%d, %d\n%d\n", res.x, res.y, targets);

  free(coords->cds);
  free(coords);

  return EXIT_SUCCESS;
}
