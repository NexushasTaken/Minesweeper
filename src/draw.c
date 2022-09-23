#include <raylib.h>
#include <stdio.h>
#include "draw.h"

// The time header is include in this header
#include "data.h"
#include "textures.h"
#include "board.h"
#include "utils.h"

extern const Textures textures;
extern const Data data;
extern const Tile **tiles;
extern const Vector2 offset;
extern Vector2 window_size;

const Vector2 offset = {
  BORDER_SIZE,
  BORDER_SIZE * 2 + STATUS_BODY_HEIGHT,
};
Rectangle body_b;
Rectangle face_b;
Rectangle timer_b;
Rectangle bomb_b;
Rectangle digit_b;
Rectangle board_b;

void M_DrawStatus() {
  // Draw face    
  DrawTexturePro(textures.faces,
                (Rectangle){data.face_status * textures.faces.height, 0,
                            textures.faces.height, textures.faces.height},
                face_b,
                (Vector2){0}, 0, WHITE);
  // Draw bomb counter
  DrawTexturePro(textures.digits_bg,
                (Rectangle){0, 0, textures.digits_bg.width, textures.digits_bg.height},
                bomb_b,
                (Vector2){0, 0}, 0, WHITE);
  // Draw timer
  DrawTexturePro(textures.digits_bg,
                (Rectangle){0, 0, textures.digits_bg.width, textures.digits_bg.height},
                timer_b,
                (Vector2){timer_b.width, 0}, 0, WHITE);

  // Drawing numbers {
  char bomb_count [3] = {0};
  char timer_count[3] = {0};
  int  gap = 1;
  float digit_width = textures.digits.width / 10.f;

  int bombs = data.bomb_count - data.flag_count;

  num_to_3chars(bomb_count, bombs >= 0 ? bombs : 0);
  num_to_3chars(timer_count, (int)data.timer_count);

  // Draw bomb digits
  digit_b.x = bomb_b.x + 1;
  digit_b.y = bomb_b.y + 1;

  for(int i = 0; i < 3; i++){
    DrawTexturePro(textures.digits,
                  (Rectangle){digit_width * bomb_count[i], 0,
                     digit_width, textures.digits.height},
                  digit_b,
                  (Vector2){0}, 0, WHITE);
    digit_b.x += digit_b.width + gap;
  }

  // Draw timer digits
  digit_b.x = (timer_b.x - timer_b.width) + 1;
  digit_b.y = (timer_b.y) + 1;

  for(int i = 0; i < 3; i++){
    DrawTexturePro(textures.digits,
                  (Rectangle){digit_width * timer_count[i], 0,
                     digit_width, textures.digits.height},
                  digit_b,
                  (Vector2){0}, 0, WHITE);
    digit_b.x += digit_b.width + gap;
  }
  // }
}

void M_DrawBoard() {
  const Texture2D *tile_sprite = &textures.tiles;
  float tile_size = tile_sprite->width / 2.f;
  for(int y = 0; y < data.grid_size.y; y++) {
    for(int x = 0; x < data.grid_size.x; x++) {
      const Tile *tile = &tiles[y][x];
      Rectangle destination = {
        x * TILE_SIZE + BORDER_SIZE, (y * TILE_SIZE) + offset.y,
        TILE_SIZE, TILE_SIZE,
      };
      Rectangle source = {
        tile_size, 0,
        tile_size, tile_size
      };

      if(tile->open) {
        source.x = tile_size;
        source.y = 0;
        if(tile->bomb) {
          source.x = tile->bomb == 2 ? tile_size : 0;
          source.y = 10 * tile_size;
        }
        else if(tile->bomb_count >= 1 && tile->bomb_count <= 8) {
          source.x = 0;
          source.y = tile->bomb_count * tile_size;
        }
      }
      else {
        source.x = 0;
        source.y = 0;
        if(tile->flag) {
          source.x = tile->flag == 2 ? tile_size : 0;
          source.y = 9 * tile_size;
        }
        else if(tile->just_open) {
          source.x = tile_size;
          source.y = 0;
        }
      }
      DrawTexturePro(*tile_sprite, source, destination, (Vector2){0}, 0, WHITE);
    }
  }
}

void M_DrawBorders() {
  // Draw edges {
  // Top
  DrawTexturePro(textures.border_ver,
                (Rectangle){0,0, textures.border_ver.width, textures.border_ver.height},
                (Rectangle){0,0, GetScreenWidth(), BORDER_SIZE},
                (Vector2){0}, 0, WHITE);
  // Middle
  DrawTexturePro(textures.border_ver,
                (Rectangle){0,0, textures.border_ver.width, textures.border_ver.height},
                (Rectangle){0,STATUS_BODY_HEIGHT + BORDER_SIZE, GetScreenWidth(), BORDER_SIZE},
                (Vector2){0}, 0, WHITE);
  // Bottom
  DrawTexturePro(textures.border_ver,
                (Rectangle){0,0, textures.border_ver.width, textures.border_ver.height},
                (Rectangle){0,GetScreenHeight() - BORDER_SIZE, GetScreenWidth(), BORDER_SIZE},
                (Vector2){0}, 0, WHITE);
  // Left
  DrawTexturePro(textures.border_hor,
                (Rectangle){0,0, textures.border_hor.width, textures.border_hor.height},
                (Rectangle){0,0, BORDER_SIZE, GetScreenHeight() - BORDER_SIZE},
                (Vector2){0}, 0, WHITE);
  // Right
  DrawTexturePro(textures.border_hor,
                (Rectangle){0,0, textures.border_hor.width, textures.border_hor.height},
                (Rectangle){GetScreenWidth() - BORDER_SIZE,0, BORDER_SIZE, GetScreenHeight() - BORDER_SIZE},
                (Vector2){0}, 0, WHITE);
  // }

  // Draw corners {
  float corner_size = textures.corners.height/2.f;
  // Upper left
  DrawTexturePro(textures.corners,
                (Rectangle){0,0, corner_size, corner_size},
                (Rectangle){0,0, BORDER_SIZE, BORDER_SIZE}, (Vector2){0}, 0, WHITE);
  // Upper right
  DrawTexturePro(textures.corners,
                (Rectangle){corner_size, 0, corner_size, corner_size},
                (Rectangle){GetScreenWidth() - BORDER_SIZE, 0, BORDER_SIZE, BORDER_SIZE},
                (Vector2){0}, 0, WHITE);

  // Middle left
  DrawTexturePro(textures.separator,
                (Rectangle){0, 0, corner_size, corner_size},
                (Rectangle){0, BORDER_SIZE + STATUS_BODY_HEIGHT, BORDER_SIZE, BORDER_SIZE},
                (Vector2){0}, 0, WHITE);
  // Middle right
  DrawTexturePro(textures.separator,
                (Rectangle){corner_size, 0, corner_size, corner_size},
                (Rectangle){GetScreenWidth() - BORDER_SIZE, BORDER_SIZE + STATUS_BODY_HEIGHT,
                   BORDER_SIZE, BORDER_SIZE},
                (Vector2){0}, 0, WHITE);

  // Bottom left
  DrawTexturePro(textures.corners,
                (Rectangle){0,corner_size, corner_size, corner_size},
                (Rectangle){0,GetScreenHeight() - BORDER_SIZE, BORDER_SIZE, BORDER_SIZE},
                (Vector2){0}, 0, WHITE);
  // Bottom right
  DrawTexturePro(textures.corners,
                (Rectangle){corner_size, corner_size, corner_size, corner_size},
                (Rectangle){GetScreenWidth() - BORDER_SIZE, GetScreenHeight() - BORDER_SIZE,
                   BORDER_SIZE, BORDER_SIZE},
                (Vector2){0}, 0, WHITE);
  // }
}

void M_CalculateBounds() {
  window_size.x =  BORDER_SIZE * 2 + data.grid_size.x * TILE_SIZE;
  window_size.y = (BORDER_SIZE * 3) + STATUS_BODY_HEIGHT + (data.grid_size.y * TILE_SIZE);

  SetWindowSize(window_size.x, window_size.y);

  float scale = 0.28; 
  Vector2 digit_bg = {
    textures.digits_bg.width * scale,
    textures.digits_bg.height * scale,
  };

  float offset_bg = (STATUS_BODY_HEIGHT - digit_bg.y) / 2;
  float offset_face = (STATUS_BODY_HEIGHT - FACE_HEIGHT) / 2.f;

  body_b = (Rectangle){
    BORDER_SIZE, BORDER_SIZE,
    window_size.x - (BORDER_SIZE * 2), STATUS_BODY_HEIGHT,
  };
  face_b = (Rectangle){
    window_size.x / 2.f - (FACE_HEIGHT / 2.f), BORDER_SIZE + offset_face,
    FACE_HEIGHT, FACE_HEIGHT,
  };
  bomb_b = (Rectangle){
    BORDER_SIZE + offset_bg, BORDER_SIZE + offset_bg,
    digit_bg.x, digit_bg.y,
  };
  timer_b = (Rectangle){
    window_size.x - BORDER_SIZE - offset_bg, BORDER_SIZE + offset_bg,
    digit_bg.x, digit_bg.y,
  };
  digit_b = (Rectangle){
    0,0,
    textures.digits.width / 10.f * scale, textures.digits.height * scale,
  };
  digit_b = (Rectangle){
    0,0,
    textures.digits.width / 10.f * scale, textures.digits.height * scale,
  };
  board_b = (Rectangle){
    BORDER_SIZE, BORDER_SIZE * 2 + STATUS_BODY_HEIGHT,
    data.grid_size.x * TILE_SIZE, data.grid_size.y * TILE_SIZE
  };
}
