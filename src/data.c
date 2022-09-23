#include "data.h"
#include "textures.h"
#include "utils.h"
#include "board.h"
#include "stdio.h"
#include "draw.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

extern Rectangle body_b;
extern Rectangle face_b;
extern Rectangle timer_b;
extern Rectangle bomb_b;
extern Rectangle digit_b;
extern Rectangle board_b;

extern Textures textures;
extern Vector2 window_size;
extern Tile **tiles;
int grid_limit_x;
int grid_limit_y;
Data data = {0};

void M_FreeTiles() {
  if(tiles == NULL)
    return;

  for(int y = 0; y < data.grid_size.y; y++) {
    free(tiles[y]);
    tiles[y] = NULL;
  }
  free(tiles);
  tiles = NULL;
}

void M_ResetData(){
  // Reset {
  data.bombs_generated = 0;
  memset(&data.start_time, 0, sizeof(struct timeval));
  data.timer_count     = 0;
  data.flag_count      = 0;
  data.tile_left       = 0;
  data.playing         = true;
  // }

  M_FreeTiles();

  switch (data.difficulty) {
    case BEGGINER:
      data.grid_size.x = 8;
      data.grid_size.y = 8;
      data.bomb_count = 10;
      break;
    case INTERMEDIATE:
      data.grid_size.x = 16;
      data.grid_size.y = 16;
      data.bomb_count = 40;
      break;
    case EXPERT:
      data.grid_size.x = 30;
      data.grid_size.y = 16;
      data.bomb_count = 99;
      break;
    default:
      M_UserNormalize();
      data.grid_size = data.user_grid_size;
      data.bomb_count = (int)(data.grid_size.x * data.grid_size.y * data.user_bomb_percent * 0.01);
      break;
  }

  data.tile_left = data.grid_size.x * data.grid_size.y - data.bomb_count;

  tiles = calloc(data.grid_size.y, sizeof(Tile*));
  for(int y = 0; y < data.grid_size.y; y++) {
    tiles[y] = calloc(data.grid_size.x, sizeof(Tile));
  }

  M_CalculateBounds();
}

void M_UserNormalize() {
  if(data.user_grid_size.x < 8)
    data.user_grid_size.x  = 8;
  if(data.user_grid_size.y < 8)
    data.user_grid_size.y  = 8;

  if(data.user_grid_size.x > grid_limit_x)
    data.user_grid_size.x  = grid_limit_x;
  if(data.user_grid_size.y > grid_limit_y)
    data.user_grid_size.y  = grid_limit_y;

  if(data.user_bomb_percent < 5)
    data.user_bomb_percent = 9;
  if(data.user_bomb_percent > 90)
    data.user_bomb_percent = 1;
}

void M_LoadData() {
  int min_x = INT_MAX;
  int min_y = INT_MAX;
  for(int i = 0; i < GetMonitorCount(); i++) {
    int c_x = GetMonitorWidth(i);
    int c_y = GetMonitorHeight(i);
    if(c_x < min_x)
      min_x = c_x;
    if(c_y < min_y)
      min_y = c_y;
  }
  grid_limit_x = (int)((min_x - BORDER_SIZE * 2) / TILE_SIZE);
  grid_limit_y = (int)((min_y - BORDER_SIZE * 2) / TILE_SIZE) - 6;

  data.difficulty = BEGGINER;
  data.user_grid_size.x = 8;
  data.user_grid_size.y = 8;
  data.user_bomb_percent = 30;

  data.begginer_time = -1;
  data.intermediate_time = -1;
  data.expert_time = -1;

  FILE *file = fopen("./data", "r");
  if(file != NULL) {
    fscanf(file, "%d", &data.difficulty);
    fscanf(file, "%f", &data.user_grid_size.x);
    fscanf(file, "%f", &data.user_grid_size.y);
    fscanf(file, "%d", &data.user_bomb_percent);
    fscanf(file, "%f", &data.begginer_time);
    fscanf(file, "%f", &data.intermediate_time);
    fscanf(file, "%f", &data.expert_time);
    fclose(file);
  }
  M_UserNormalize();

  M_ResetData();
}

void M_SaveData() {
  FILE *file = fopen("./data", "w");
  fprintf(file, "%d ", data.difficulty);
  fprintf(file, "%.0f ", data.user_grid_size.x);
  fprintf(file, "%.0f ", data.user_grid_size.y);
  fprintf(file, "%d ", data.user_bomb_percent);
  fprintf(file, "%.2f ", data.begginer_time);
  fprintf(file, "%.2f ", data.intermediate_time);
  fprintf(file, "%.2f\n", data.expert_time);
  fprintf(file, "NOTE: Pls don't edit the first line!.\n");
  fprintf(file, "\tThis file where youre Minesweeper data are stored\n");
  fprintf(file, "\tYou can look at the source code\n");
  fprintf(file, "\t\tdata.c:M_LoadData\n");
  fprintf(file, "\t\tdata.c:M_LoadData\n");
  fclose(file);
}
