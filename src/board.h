#ifndef H_BOARD
#define H_BOARD

#include "raylib.h"

typedef struct tile {
  short bomb_count;
  short open;
  short flag;
  short bomb;
  bool  visited;
  bool  just_open;
} Tile;

void M_UpdateBoard();
void M_GenerateBombs();

#endif // !H_BOARD
