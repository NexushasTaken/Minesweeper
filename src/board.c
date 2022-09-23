#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdbool.h>
#include "draw.h"
#include "board.h"
#include "data.h"
#include "textures.h"
#include "utils.h"

// Bounds {
extern const Rectangle body_b;
extern const Rectangle face_b;
extern const Rectangle timer_b;
extern const Rectangle bomb_b;
extern const Rectangle digit_b;
extern const Rectangle board_b;
// }

static struct timeval end_time;
extern const Vector2 offset;
extern Textures textures;
extern Data data;
Tile **tiles;


// I love macros hahaha
#define VISIT_NEIGHBORS(INDEX_X, INDEX_Y, BLOCK)\
  for  (int i = INDEX_Y - 1; i < INDEX_Y + 2; i++) {\
    for(int j = INDEX_X - 1; j < INDEX_X + 2; j++) {\
      if(j >= 0 && i >= 0 &&\
         i < data.grid_size.y && j < data.grid_size.x) BLOCK }}
#define ITERATE_TILES(BLOCK)\
  for  (int i = 0; i < data.grid_size.y; i++) {\
    for(int j = 0; j < data.grid_size.x; j++) BLOCK }


static bool safe_bounds(int x, int y) {
  return x >= 0 && x < data.grid_size.x && 
         y >= 0 && y < data.grid_size.y;
}

static int count_bombs_neighbors(int index_x, int index_y) {
  int bomb_count = 0;
  VISIT_NEIGHBORS(index_x, index_y, {
    if(tiles[i][j].bomb)
      bomb_count++;
  })
  return bomb_count;
}

static void place_numbers() {
  ITERATE_TILES({
    if(!tiles[i][j].bomb)
      tiles[i][j].bomb_count = count_bombs_neighbors(j, i);
  })
}

// This will only generate once when bomb generated
void M_GenerateBombs(int cursor_x, int cursor_y) {
  if (!data.bombs_generated) {
    // The time check is in draw.c:M_DrawStatus
    gettimeofday(&data.start_time, NULL);

    int bomb_placed = 0;
    int start_x = cursor_x - 1;
    int start_y = cursor_y - 1;
    while(bomb_placed != data.bomb_count) {
      int pos_x, pos_y;
      // Thanks me later
    gen:
      pos_x = GetRandomValue(0, (int)data.grid_size.x - 1);
      pos_y = GetRandomValue(0, (int)data.grid_size.y - 1);
      if((pos_x == cursor_x && pos_y == cursor_y) || tiles[pos_y][pos_x].bomb) {
        goto gen;
      }
      VISIT_NEIGHBORS(cursor_x, cursor_y, {
        if(i == pos_y && j == pos_x)
          goto gen;
      })
      tiles[pos_y][pos_x].bomb = true;
      bomb_placed++;
    }
    data.bombs_generated = true;
    place_numbers();
  }
}

void win(){
  // Check the board if solve/win
  if(data.tile_left == 0) {
    data.face_status = FACE_WIN;
    data.playing = false;
    data.bomb_count = 0;
    ITERATE_TILES({
      if(tiles[i][j].bomb) {
        tiles[i][j].open = false;
        tiles[i][j].flag = true;
      }
      if(!tiles[i][j].open && !tiles[i][j].bomb) {
        tiles[i][j].open = true;
      }
    })
    float time_finished =
      ((end_time.tv_sec - data.start_time.tv_sec)   * 1000000 +
      (end_time.tv_usec - data.start_time.tv_usec)) * 0.000001; 
    switch (data.difficulty) {
      case BEGGINER:
        if(data.begginer_time < time_finished)
          data.begginer_time = time_finished;
        break;
      case INTERMEDIATE:
        if(data.begginer_time < time_finished)
          data.intermediate_time = time_finished;
        break;
      case EXPERT:
        if(data.begginer_time < time_finished)
          data.expert_time = time_finished;
        break;
      default:
        break;
    }
  }
}

static void debug() {
//  bool is_window_resized = IsWindowResized();
  bool print = 0;
//  if (IsWindowResized() && !bin_resize) {
//    M_CalculateBounds();
//    int x = GetScreenWidth() - BORDER_SIZE * 2;
//    int y = GetScreenHeight() - (BORDER_SIZE * 3 + STATUS_BODY_HEIGHT);
//    data.difficulty = NONE;
//    data.user_grid_size.x = (int)(x / TILE_SIZE);
//    data.user_grid_size.y = (int)(y / TILE_SIZE);
//    M_ResetData();
//    print = 1;
//    bin_resize = false;
//  }

  // Others {
  // Change difficulty when w is pressed
  if(IsKeyPressed(KEY_F)) {
    data.difficulty += 1;
    data.difficulty %= NONE;
    M_ResetData();
    print = 1;
  }

  if(IsKeyPressed(KEY_W)) {
    data.user_grid_size.y++;
    M_ResetData();
    print = 1;
  }
  else if(IsKeyPressed(KEY_S)) {
    data.difficulty = NONE;
    data.user_grid_size.y--;
    M_ResetData();
    print = 1;
  }
  else if(IsKeyPressed(KEY_D)) {
    data.difficulty = NONE;
    data.user_grid_size.x++;
    M_ResetData();
    print = 1;
  }
  else if(IsKeyPressed(KEY_A)) {
    data.difficulty = NONE;
    data.user_grid_size.x--;
    M_ResetData();
    print = 1;
  }
  else if(IsKeyPressed(KEY_Z)) {
    ITERATE_TILES({
      tiles[i][j].open = true;
    })
    print = 1;
  }
  else if(IsKeyPressed(KEY_X)) {
    ITERATE_TILES({
      tiles[i][j].open = false;
    })
    print = 1;
  }
  else if(IsKeyPressed(KEY_X)) {
          
  }
  if(print) {
  }
  // }
}

// Beautiful solution
// Check the tile in specified index (This function can visit neighbors or just one tile),
//   if the tiles is bomb then gameover
static bool check_tile(int index_x, int index_y, bool neighbors) {
  int s_x = index_x   -   neighbors,
      e_x = index_x + 1 * neighbors + 1,
      s_y = index_y   -   neighbors,
      e_y = index_y + 1 * neighbors + 1;

  Tile *bomb_tiles[8]  = {0};
  int   bomb_tiles_len = 0;
  for  (int i = s_y; i < e_y; i++) {
    for(int j = s_x; j < e_x; j++) {
      if(j >= 0 &&
         i >= 0 &&
         i < data.grid_size.y &&
         j < data.grid_size.x && 
         tiles[i][j].bomb) {
        bomb_tiles[bomb_tiles_len++] = &tiles[i][j];
      }
    }
  }
  if(bomb_tiles_len) {
    data.face_status = FACE_LOSE;
    data.playing = false;
    ITERATE_TILES({
      if(tiles[i][j].flag) {
        tiles[i][j].flag = 2;
      } if(tiles[i][j].bomb) { tiles[i][j].open = true;
      }
    })
    for(int i = 0; i < bomb_tiles_len; i++) {
      bomb_tiles[i]->open = true;
      bomb_tiles[i]->bomb = 2;
    }
  }
  return bomb_tiles_len > 0;
}

static void M_RevealTile(int index_x, int index_y) {
  // Generate bombs if theres no bombs in grid, and generate bombs only when mouse is release
  M_GenerateBombs(index_x, index_y);
  Tile *ct = &tiles[index_y][index_x];
  if(check_tile(index_x, index_y, false) || ct->open) {
    return;
  }
  else {
    ct->open = true;
    data.tile_left--;
    if(ct->bomb_count) {
      return;
    }
  }

  VISIT_NEIGHBORS(index_x, index_y, {
    if(!tiles[i][j].open) {
      M_RevealTile(j, i);
    }
  })
}

void M_UpdateBoard() {
  int mouse_pos_x = GetMouseX();
  int mouse_pos_y = GetMouseY();
  int mouse_left  = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
  int mouse_right = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
  int mouse_left_d  = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
  int mouse_right_d = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
  int mouse_left_r  = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
  int mouse_right_r = IsMouseButtonReleased(MOUSE_BUTTON_RIGHT);

  ITERATE_TILES({
    tiles[i][j].visited = false;
    tiles[i][j].just_open = false;
  })

  if (IsWindowResized() && false) {
    M_CalculateBounds();
    int x = GetScreenWidth() - BORDER_SIZE * 2;
    int y = GetScreenHeight() - (BORDER_SIZE * 3 + STATUS_BODY_HEIGHT);
    data.difficulty = NONE;
    data.user_grid_size.x = (int)(x / TILE_SIZE);
    data.user_grid_size.y = (int)(y / TILE_SIZE);
    M_ResetData();
  }

  debug();

  if(mouse_left || mouse_right) {
    M_CalculateBounds();
  }
  
  // Check if body_b/status is pressed
  if(mouse_pos_x >= body_b.x && mouse_pos_y >= body_b.y &&
     mouse_pos_x <= body_b.x + body_b.width &&
     mouse_pos_y <= body_b.y + body_b.height && mouse_left_d) {
    data.face_status = FACE_PRESSED;
    M_ResetData();
    return;
  }
  else {
    // FACE_LOSE and FACE_WIN should not be change. only if body_b/status is clicked
    if(data.face_status != FACE_LOSE && data.face_status != FACE_WIN)
      data.face_status = FACE;
  }

  if (data.playing) {
    gettimeofday(&end_time, NULL);
    data.timer_count = end_time.tv_sec - data.start_time.tv_sec;

    if(data.timer_count > 999)
      data.timer_count = 0;

    int index_y = -1;
    int index_x = -1;
    int pos_x = mouse_pos_x - offset.x;
    int pos_y = mouse_pos_y - offset.y;
  
    // Getting the index base on mouse pos, stop the function if mouse is not clica
    index_y = pos_y / TILE_SIZE;
    index_x = pos_x / TILE_SIZE;

    // Check if index_x/y is inside of grid
    if(index_x >= 0 && index_y >= 0 &&
       index_y < data.grid_size.y && index_x < data.grid_size.x &&
       pos_x >= 0 && pos_y >= 0) {
      Tile *ct = &tiles[index_y][index_x];
      
      if(mouse_left_d) {
        ct->just_open = true;
        if(ct->open && ct->bomb_count > 0) {
          VISIT_NEIGHBORS(index_x, index_y, {
            if(!tiles[i][j].flag) {
              tiles[i][j].just_open = true;
            }
          })
        }
        data.face_status = FACE_ACTIVE;
      }
      // If Left mouse button is clicked? and check if the tile is not flagged
      if(mouse_left_r && !ct->flag && !ct->open) {
        M_RevealTile(index_x, index_y);
      }
      if(mouse_left_r && ct->open && ct->bomb_count > 0){
        // Reveal all neighbors
        __uint16_t indexes[8] = {0};
        Tile *tiles_captured[8] = {0};
        int len = 0;
        int flag_count = 0;

        VISIT_NEIGHBORS(index_x, index_y, {
          Tile *curr_t = &tiles[i][j];
          if(curr_t->flag > 0) {
            ++flag_count;
          }
          if(!curr_t->open && !curr_t->flag) {
            __uint8_t x = j;
            __uint8_t y = i;
            indexes[len] = (x << (sizeof(__uint8_t)*8)) + i;
            tiles_captured[len++] = curr_t;
          }
        })
        if(ct->bomb_count == flag_count) {
          for (int i = 0; i < len; i++) {
            __uint8_t x = indexes[i] >> (sizeof(__uint8_t) * 8);
            __uint8_t y = indexes[i];
            M_RevealTile(x, y);
          }            
        }
      }

      if(mouse_right && !ct->open){
        // Add flag if tile is not opened and not flagged
        ct->flag = !ct->flag;
        data.bomb_count += ct->flag ? -1 : 1;
      }
      win();
    }
  }
}

