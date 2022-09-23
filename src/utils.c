#include "utils.h"
#include <stdio.h>

extern int debug;

void printVector2V(float x, float y) {
  if (debug)
    printf("[%.2f, %.2f]\n", x, y);
}

void printVector2(Vector2 *vec) {
  if (debug)
    printf("[%.2f, %.2f]\n", vec->x, vec->y);
}

void printRectangle(Rectangle *rect) {
  if (debug)
    printf("[(%-4.2f,%-4.2f)(%-4.2f,%-4.2f)]",
           rect->width, rect->height, rect->x, rect->y);
}

void num_to_3chars(char *s, int value) {
  if (value > 999)
    return;
  int i = 2;
  while (value > 0) {
    s[i--] = value % 10;
    value /= 10;
  }
}
