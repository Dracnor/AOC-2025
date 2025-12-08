#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>


#define MAX_NB_ROWS 150
#define MAX_NB_COLS 150


void part12(char manifold[MAX_NB_ROWS][MAX_NB_COLS], int nb_rows, int nb_cols,
               int64_t *p1, int64_t* p2) 
{
  int64_t nb_split = 0;
  
  // Array that counts how many times a tachyon reach (i,j)
  int64_t nb_reached[MAX_NB_ROWS][MAX_NB_COLS] = {};
  for (int j = 0; j < nb_cols; j++)
      nb_reached[0][j] = manifold[0][j] == 'S' ? 1 : 0;
  
  // DP to compute number of times reached
  for (int i = 0; i < nb_rows-1; i++)
    for (int j = 0; j < nb_cols; j++)
      if (nb_reached[i][j] > 0) {
        char below = manifold[i+1][j];
        if (below == '^') {
          nb_reached[i+1][j-1] += nb_reached[i][j];
          nb_reached[i+1][j+1] += nb_reached[i][j];
          nb_split++;
        }
        else 
          nb_reached[i+1][j] += nb_reached[i][j];
      }
  
  *p1 = nb_split;

  int64_t nb_paths = 0;
  for (int j = 0; j < nb_cols; j++)
      nb_paths += nb_reached[nb_rows-1][j];
  *p2 = nb_paths;
}


int main(int argc, char const* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage : %s filename\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE* f = fopen(argv[1], "r");
  char manifold[MAX_NB_ROWS][MAX_NB_COLS];
  int nb_rows = 0;

  while (fgets(manifold[nb_rows], MAX_NB_COLS+1, f) != NULL)
    nb_rows++;
  int nb_cols = strlen(manifold[0]);
  fclose(f);

  

  int64_t p1 = 0;
  int64_t p2 = 0;
  part12(manifold, nb_rows, nb_cols, &p1, &p2);
  
  printf("%ld\n%ld\n", p1, p2);
  

  return EXIT_SUCCESS;
}