#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>


#define MAX_BANK_LEN 101
#define MAX_NB_BANK 200

#define max(a,b) ({\
  __typeof__ (a) _a = (a);\
  __typeof__ (b) _b = (b);\
  _a > _b ? _a : _b;\
})


/* We'll compute the voltage of a bank as follow :
  read the bank from left to right. As we do, we memorize :
  - the biggest digit we've seen
  - the biggest voltage we've been able to create
*/

int part1(int8_t banks[MAX_NB_BANK][MAX_BANK_LEN], int nb_bank, int bank_len) {
  int sum = 0;

  for (int i = 0; i < nb_bank; i++) {
    int biggest_digit = 0;
    int biggest_voltage = 0;
    for (int j = 0; j < bank_len; j++) {
      biggest_voltage = max(biggest_voltage, 10*biggest_digit + banks[i][j]);
      biggest_digit = max(biggest_digit, banks[i][j]);
    } 

    sum += biggest_voltage;
  }

  return sum;
}


/* I'll try something that shouldn't work. 
  I'll read each bank from left to right and memorize the biggest voltage. 
  Update it with each battery (digit) as follow :
  - either don't use this new digit
  - either use it as any digit. All the digits to the right of it will be discarded and set to 0.

  Ofc, near the end of the bank remember that you need 12 digits in the final
  voltage and hence can't discard too many digits.
*/


int64_t power(int64_t x, int n) {
  int64_t p = 1;
  for (int i = 0; i < n; i++) p *= x;
  return p;
}


/** Updates as explained. 
  @remaining_digits is the number of available digits remaining (j included) */
int64_t update(int64_t biggest_voltage, int8_t new_digit, int remaining_digits) {
  int64_t new_biggest = biggest_voltage;

  for (int n = 0; n < 12 && n < remaining_digits; n++) {
    int64_t ten_n = power(10, n);
    int64_t at_pos_n = (biggest_voltage / (10*ten_n)) * (10*ten_n) + new_digit * ten_n;
    new_biggest = max(new_biggest, at_pos_n);
  }

  return new_biggest;
}


int64_t part2(int8_t banks[MAX_NB_BANK][MAX_BANK_LEN], int nb_bank, int bank_len) {
  int64_t sum = 0;

  for (int i = 0; i < nb_bank; i++) {
    int64_t biggest_voltage = 0;
    for (int j = 0; j < bank_len; j++) {
      biggest_voltage = update(biggest_voltage, banks[i][j], bank_len - j);
    }

    sum += biggest_voltage;
  }

  return sum;
}


int main(int argc, char const* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage : %s input_file", argv[0]);
    return EXIT_FAILURE;
  }

  FILE* f = fopen(argv[1], "r");
  int8_t banks[MAX_NB_BANK][MAX_BANK_LEN] = {};
  int nb_bank = 0;
  while (fscanf(f, "%s", banks[nb_bank]) != EOF)
    nb_bank++;
  fclose(f);
  int bank_len = strlen((char*) banks[0]);

  // Sub '0' to all char to manipulate integers directly
  for (int i = 0; i < nb_bank; i++)
    for (int j = 0; j < bank_len; j++)
      banks[i][j] -= '0';
  
  
  printf("%d\n%lu\n", part1(banks, nb_bank, bank_len), part2(banks, nb_bank, bank_len));

  return EXIT_SUCCESS;
}