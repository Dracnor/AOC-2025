#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#define MAX_NB_RANGES 200
#define MAX_NB_IDS 1001

#define max(a,b) ({\
  __typeof__ (a) _a = (a);\
  __typeof__ (b) _b = (b);\
  _a > _b ? _a : _b;\
})


typedef struct range_s {int64_t a; int64_t b;} range;

int lexico(const void* p0, const void* p1) {
  range r0 = * (range*) p0;
  range r1 = * (range*) p1;

  int cmp_a = (r0.a > r1.a) - (r0.a < r1.a);
  int cmp_b = (r0.b > r1.b) - (r0.b < r1.b);
  if (cmp_a != 0) return cmp_a;
  else return cmp_b;
}


int64_t part1(range ranges[MAX_NB_RANGES], int nb_ranges, int64_t ids[MAX_NB_IDS], int nb_ids) {
  int64_t nb_fresh = 0;

  for (int i = 0; i < nb_ids; i++) {
    int64_t id = ids[i];
    bool is_fresh = false;

    for (int j = 0; j < nb_ranges; j++) {
      if (ranges[j].a <= id && id <= ranges[j].b) {
        is_fresh = true;
        break;
      }
    }

    if (is_fresh) nb_fresh++;
  }

  return nb_fresh;
}


/** Part2 : merge the intervals !
  I'll sort them by lexicographic order to fasten the merging */
int64_t part2(range ranges[MAX_NB_RANGES], int nb_ranges) {
  // First, build rmerged the list of ranges merged
  range rmerged[MAX_NB_RANGES];
  int nb_merged = 0;
  qsort(ranges, nb_ranges, sizeof(range), lexico);

  rmerged[0] = ranges[0];
  nb_merged++;

  for (int j = 1; j < nb_ranges; j++) {
    // if the jth interval can be merged with the last one : do it
    if (ranges[j].a <= rmerged[nb_merged-1].b) {
      rmerged[nb_merged-1].b = max(ranges[j].b, rmerged[nb_merged-1].b);
    }

    else {
      rmerged[nb_merged] = ranges[j];
      nb_merged++;
    }
  }

  // Now, just sum over all the merged ranges
  int64_t nb_fresh = 0;
  for (int j = 0; j < nb_merged; j++)
    nb_fresh += rmerged[j].b - rmerged[j].a +1;

  return nb_fresh;
}


int main(int argc, char const* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage : %s filename\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE* f = fopen(argv[1], "r");
  
  // First read the ranges
  range ranges[MAX_NB_RANGES];
  int nb_ranges = 0;

  // First loop to read the ranges
  while(true) {
    // Get the line
    char* line = NULL;
    size_t line_length = 0;
    getline(&line, &line_length, f);

    // Detect if the line is the end of ranges
    if (strcmp(line, "\n") == 0) {
      free(line);
      break;
    }

    // else parse the line
    else {
      sscanf(line, "%ld-%ld", &ranges[nb_ranges].a, &ranges[nb_ranges].b);
      free(line);
      nb_ranges++;
    }
  }

  // Now read the IDS
  int64_t ids[MAX_NB_IDS];
  int nb_ids = 0;
  while(fscanf(f, " %ld", &ids[nb_ids]) != EOF) {
    nb_ids++;
  }

  fclose(f);


  // Sort the ranges : it will be usefull for part2

  int64_t p1 = part1(ranges, nb_ranges, ids, nb_ids);
  int64_t p2 = part2(ranges, nb_ranges);
  printf("%ld\n%ld\n", p1, p2);

  return EXIT_SUCCESS;
}