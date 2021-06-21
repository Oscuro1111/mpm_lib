#include<stdint.h>
#include<unistd.h>

//Convert initeger upto 16 decimal places to string
void int_to_str(char buf[16], int value) {

  int8_t i = 0;

  char buffer[16];
  uint8_t sign = value < 0 ? 1 : 0;

  uint8_t y = sign ? 1 : 0;

  uint8_t temp;

  while (value > 0) {
    temp = value % 10;
    buffer[i++] = 48 + temp;
    value /= 10;
  }

  for (int8_t x = i - 1; x >= 0; x--) {
    buf[y++] = buffer[x];
  }

  if (sign) {
    buf[0] = '-';
  }

  buf[y] = '\0';
}
