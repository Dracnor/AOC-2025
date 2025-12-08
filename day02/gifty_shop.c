#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


#define MAX_LEN 40 // I have < ranges in my input 

#define min(a,b) ({\
  __typeof__ (a) _a = (a);\
  __typeof__ (b) _b = (b);\
  _a < _b ? _a : _b;\
})

typedef struct digits_s {
  int digits[64];
  int nb_digits;
} digits;


/* The smart solution would be to observe that 
  all squares are of the form k*100...01;
  then conclude with Gauss integer sum.
  But today, I'm not smart.
*/

digits digits_of_int64(int64_t id) {
  digits d = {};
  while (id != 0) {
    d.digits[d.nb_digits] = id % 10;
    id /= 10;
    d.nb_digits++;
  }
  return d;
}


/** Checks if id is a decimal square */
bool is_dec_square(int64_t id) {
  digits d = digits_of_int64(id);
  if (d.nb_digits % 2 != 0)
    return false;

  for (int i = 0; i < d.nb_digits/2 ; i++)
    if (d.digits[i] != d.digits[d.nb_digits/2 + i])
      return false;
  return true;
}


int64_t part1(int64_t ranges[MAX_LEN][2], int len) {
  int64_t sum = 0;

  // For each range, check all ids and sum invalid ones 
  for (int i = 0; i < len; i++)
    for (int64_t id = ranges[i][0]; id <= ranges[i][1]; id++)
      if (is_dec_square(id))
        sum += id;

  return sum;
}


/** Checks if the decimal writing of is p-periodic */
bool is_p_periodic(digits d, int p) {
  if (d.nb_digits % p != 0)
    return false;

  for (int i = 0; i+p < d.nb_digits; i++)
    if (d.digits[i] != d.digits[i+p])
      return false;
  return true;
}


/** Checks if id is p-periodic for some p */
bool is_dec_periodic(int64_t id) {
  digits d = digits_of_int64(id);

  // For each period, check if p-periodic
  for (int p = 1; p <= d.nb_digits /2; p++)
    if (is_p_periodic(d, p)) 
      return true;
  return false;
}


int64_t part2(int64_t ranges[MAX_LEN][2], int len) {
  int64_t sum = 0;

  for (int i = 0; i < len; i++)
    for (int64_t id = ranges[i][0]; id <= ranges[i][1]; id++)
      if (is_dec_periodic(id))
        sum += id;

  return sum;
}


int main(int argc, char const* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage : %s filename\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE* f = fopen(argv[1], "r");
  int64_t ranges[MAX_LEN][2] = {};
  int len = 0;
  do {
    fscanf(f, "%ld-%ld", &(ranges[len][0]), &(ranges[len][1]));
    len++;
  } while (fscanf(f, ",") != EOF);
  fclose(f);

  printf("%ld\n%ld\n", part1(ranges, len), part2(ranges, len));

  return EXIT_SUCCESS;
}