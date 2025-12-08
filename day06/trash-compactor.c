#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>


/* Due to part 2, we'll store the input as a char matrix */
#define MAX_NB_COLS 4000
#define MAX_NB_ROWS 5


int64_t part1(char input[MAX_NB_ROWS][MAX_NB_COLS], int nb_rows, int separators[MAX_NB_COLS], int nb_problems) {
  // Sum the solution of all problems.
  int64_t grand_total = 0;
  for (int n = 0; n < nb_problems; n++) {
    int jmin = separators[n];

    char op = input[nb_rows-1][jmin];
    assert(op == '+' || op == '*'); // to debug
    int64_t answer = op == '+' ? 0 : 1;

    // Read all the numbers of the n-th list
    for (int i = 0; i < nb_rows-1; i++) {
      int64_t number = -1; 
      sscanf(&(input[i][jmin]), " %ld", & number);
      if (op == '+') answer += number;
      else answer *= number;
    }

    grand_total += answer;
  }

  return grand_total;
}


/** Sets *number to the number written in column j.
  Returns true if a number has been read, false if column was empty. */
bool get_column_integer(char input[MAX_NB_ROWS][MAX_NB_COLS], int nb_rows, int j, int64_t *number) {
  bool nonempty = false;
  *number = 0;
  for (int i = 0; i < nb_rows-1; i++) {
    if (input[i][j] == ' ') continue;
    else {
      nonempty = true;
      *number = *number * 10 + (input[i][j] - '0');
    }
  }
  return nonempty;
}


int64_t part2(char input[MAX_NB_ROWS][MAX_NB_COLS], int nb_rows, int separators[MAX_NB_COLS], int nb_problems) {
  // Sum the solution of all problems.
  int64_t grand_total = 0;
  for (int n = 0; n < nb_problems; n++) {
    int jmin = separators[n];
    int jmax = separators[n+1];

    char op = input[nb_rows-1][jmin];
    assert(op == '+' || op == '*'); // to debug
    int64_t answer = op == '+' ? 0 : 1;

    // Read all the numbers of the n-th list/
    // This time, written in column !
    for (int j = jmin; j < jmax; j++) {
      int64_t number = -1;
      if (get_column_integer(input, nb_rows, j, &number)) {
        if (op == '+') answer += number;
        else answer *= number;
      }
    }

    // printf("%dth problem has answer %ld\n", n, answer);
    grand_total += answer;
  }

  return grand_total;
}


int main(int argc, char const* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage : %s filename\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE* f = fopen(argv[1], "r");
  char input[MAX_NB_ROWS][MAX_NB_COLS];
  int nb_rows = 0;

  while (fgets(input[nb_rows], MAX_NB_COLS, f) != NULL)
    nb_rows++;
  fclose(f);

  // The ith problem goes from column sep[i] to sep[i+1] excluded
  int separators[MAX_NB_COLS];
  int nb_problems = 0;
  int j;
  for (j = 0; input[nb_rows-1][j] != '\0'; j++)
    if (input[nb_rows-1][j] != ' ') {
      separators[nb_problems] = j;
      nb_problems++;
    }
  separators[nb_problems] = j;
  
  
  printf("%ld\n%ld\n", part1(input, nb_rows, separators, nb_problems), part2(input, nb_rows, separators, nb_problems));
  

  return EXIT_SUCCESS;
}