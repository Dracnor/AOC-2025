#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>


#define MAX_NB_ROWS 150
#define MAX_NB_COLS 150



bool is_valid_index(int nb_rows, int nb_cols, int i, int j) {
  return 0 <= i && i < nb_rows
      && 0 <= j && j < nb_cols;
}


/* In prevision of p2, we'll mark accessible scrolls with 'A' instead of '@' */
bool is_accessible(char grid[MAX_NB_ROWS][MAX_NB_COLS], int nb_rows, int nb_cols,
                   int x, int y)
{
  if (grid[x][y] != '@' && grid[x][y] != 'A') return false;

  int nb_adjacent = 0;
  for (int i = x-1; i <= x+1; i++)
    for (int j = y-1; j <= y+1; j++)
      if (is_valid_index(nb_rows, nb_cols, i, j) && !(i==x && j==y) 
          && (grid[i][j] == '@' || grid[i][j] == 'A')
        )
        nb_adjacent++;
  return nb_adjacent < 4;
}


int part1(char grid[MAX_NB_ROWS][MAX_NB_COLS], int nb_rows, int nb_cols) {
  int nb_accessible = 0;
  for (int x = 0; x < nb_rows; x++)
    for (int y = 0; y < nb_cols; y++)
      if (is_accessible(grid, nb_rows, nb_cols, x, y)) {
        grid[x][y] = 'A';
        nb_accessible++;
      }
  return nb_accessible;
}


// Part2 : just iterate part1. Stupid, but quick to code.

void delete_scrolls(char grid[MAX_NB_ROWS][MAX_NB_COLS], int nb_rows, int nb_cols) {
  for (int x = 0; x < nb_rows; x++)
    for (int y = 0; y < nb_cols; y++)
      if (grid[x][y] == 'A') grid[x][y] = '.';
}


int part2(char grid[MAX_NB_ROWS][MAX_NB_COLS], int nb_rows, int nb_cols) {
  int nb_accessible = 0;
  int new_accessible = 0;
  while ( (new_accessible = part1(grid, nb_rows, nb_cols)) != 0) {
    nb_accessible += new_accessible;
    delete_scrolls(grid, nb_rows, nb_cols);
  }
  return nb_accessible;
}



int main(int argc, char const* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage : %s filename\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE* f = fopen(argv[1], "r");
  char grid[MAX_NB_ROWS][MAX_NB_COLS] = {};
  int nb_rows = 0;
  while (fscanf(f, " %s", grid[nb_rows]) != EOF) {
    nb_rows++;
  }
  int nb_cols = strlen(grid[0]);
  fclose(f);

  int p1 = part1(grid, nb_rows, nb_cols);
  int p2 = part2(grid, nb_rows, nb_cols);
  printf("%d\n%d\n", p1, p2);


  return EXIT_SUCCESS;
}