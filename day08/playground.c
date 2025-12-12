#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>


#define min(a,b) ({\
  __typeof__ (a) _a = (a);\
  __typeof__ (b) _b = (b);\
  _a < _b ? _a : _b;\
})


#define MAX_NB_POINTS 1001

typedef struct {int x; int y; int z;} position;
// The array of junctions positions. Global for conveniance.
position positions[MAX_NB_POINTS];



/**************/
/* Union-find */
/**************/

/* I'll union-find with union by rank */
typedef struct {
  int size;
  int parent;
} uf_data;


void initialize(uf_data uf[MAX_NB_POINTS], int nb_points) {
  for (int i = 0; i < nb_points; i++)
    uf[i] = (uf_data) {.size = 1, .parent = i};
}


/** Returns the canonical representant of the circuit of i.
  Compresses paths along the way. */
int uf_find(uf_data uf[MAX_NB_POINTS], int i) {
  if (uf[i].parent == i)
    return i;
  else {
    int repr = uf_find(uf, uf[i].parent);
    uf[i].parent = repr;
    return repr;
  }
}


/** Unites the circuits of i and j.
  Returns true if a union had to been done, false else (if
  i and j already were in the same set) */
bool uf_union(uf_data uf[MAX_NB_POINTS], int i, int j) {
  int repr_i = uf_find(uf, i);
  int repr_j = uf_find(uf, j);
  if (repr_i == repr_j) 
    return false;

  // branch the smallest below the biggest
  if (uf[repr_i].size <= uf[repr_j].size) {
    uf[repr_i].parent = repr_j;
    uf[repr_j].size += uf[repr_i].size;
  }
  else {
    uf[repr_j].parent = repr_i;
    uf[repr_i].size += uf[repr_j].size;
  }
  return true;
}



/*********/
/* Parts */
/*********/


/* I'll generate the array of (index of) positions to (index of) position 
  edges and sort it by distance */
typedef struct {int pos0; int pos1;} edge;

int64_t dist(edge e) {
  int64_t dx = positions[e.pos0].x - positions[e.pos1].x;
  int64_t dy = positions[e.pos0].y - positions[e.pos1].y;
  int64_t dz = positions[e.pos0].z - positions[e.pos1].z;
  return dx*dx + dy*dy + dz*dz;
}


int compare(void const* edge0, void const* edge1) {
  int64_t d0 = dist(* (edge*) edge0);
  int64_t d1 = dist(* (edge*) edge1);
  return (d1 < d0) - (d0 < d1); 
}


int64_t product_three_greatest(uf_data uf[MAX_NB_POINTS], int nb_points) {
  // maxi0 <= maxi1 <= maxi2
  int64_t maxi0 = 0;
  int64_t maxi1 = 0;
  int64_t maxi2 = 0;

  for (int i = 0; i < nb_points; i++) {
    // only consider the representant of each set
    if (uf[i].parent != i) continue;

    // printf("a set of size %d\n", uf[i].size);

    if (uf[i].size > maxi0) {
      maxi2 = maxi1;
      maxi1 = maxi0;
      maxi0 = uf[i].size;
    } 
    else if (uf[i].size > maxi1) {
      maxi2 = maxi1;
      maxi1 = uf[i].size;
    }
    else if (uf[i].size > maxi2)
      maxi2 = uf[i].size;
  }

  return maxi0 * maxi1 * maxi2;
}


void kruskall(int nb_points, int64_t *p1, int64_t *p2) {
  /* Generate and sort the array of edges */
  edge edges[MAX_NB_POINTS * MAX_NB_POINTS / 2];
  int nb_edges = 0;
  for (int pos0 = 0; pos0 < nb_points; pos0++)
    for (int pos1 = pos0+1; pos1 <nb_points; pos1++) {
      edges[nb_edges] = (edge) {.pos0 = pos0, .pos1 = pos1};
      nb_edges++;
    }
  qsort(edges, nb_edges, sizeof(edge), compare);

  /* Now do a nice kruskall algorithm ! */
  uf_data uf[MAX_NB_POINTS];
  initialize(uf, nb_points);
  int x0_last;
  int x1_last;

  // First the first 1000 merges

  int nb_merge = 0;
  int i_edge = 0;
  for (; i_edge < min(1000, nb_edges); i_edge++) {
    if (uf_union(uf, edges[i_edge].pos0, edges[i_edge].pos1)) {
      nb_merge++;
      x0_last = positions[edges[i_edge].pos0].x;
      x1_last = positions[edges[i_edge].pos1].x;
    }
  }
  *p1 = product_three_greatest(uf, nb_points);

  // Then the others ones
  for (; i_edge < nb_edges; i_edge++) {
    if (uf_union(uf, edges[i_edge].pos0, edges[i_edge].pos1)) {
      nb_merge++;
      x0_last = positions[edges[i_edge].pos0].x;
      x1_last = positions[edges[i_edge].pos1].x;
    }
  }
  *p2 = ((int64_t) x0_last) * ((int64_t) x1_last);
}



int main(int argc, char const* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage : %s filename\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE* f = fopen(argv[1], "r");
  // int positions[MAX_NB_POINTS][3]; -> became a global variable for comfort
  int nb_points = 0;

  while (fscanf(f, " %d,%d,%d", &positions[nb_points].x, 
                            &positions[nb_points].y, &positions[nb_points].z) 
        != EOF
  )
    nb_points++;
  
  fclose(f);


  
  int64_t p1 = 0;
  int64_t p2 = 0;
  kruskall(nb_points, &p1, &p2);
  
  printf("%ld\n%ld\n", p1, p2);
  

  return EXIT_SUCCESS;
}