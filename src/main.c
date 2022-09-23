#include "board.h"
#include "data.h"
#include "draw.h"
#include "settings_view.h"
#include "textures.h"
#include "utils.h"
#include <bits/types/struct_timeval.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

int debug = 1;
extern Data data;
Vector2 window_size = {190, 240};

int main() {
  struct timeval start, stop;
  gettimeofday(&start, NULL);
  SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN | FLAG_MSAA_4X_HINT);
  InitWindow(window_size.x, window_size.y, "Minesweeper");
  SetWindowMinSize(172, 222);
  //SetTargetFPS(60);

  M_LoadTextures();
  M_LoadData();
  M_InitSettingsView();

  bool game = 1;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground((Color){199, 199, 199, 255});

    if (game) {
      M_UpdateBoard();

      M_DrawBoard();
      M_DrawStatus();
      M_DrawBorders();
    } else {
      M_UpdateSettingsView();
      M_DrawBorders();
      M_DrawSettingsView();
    }
    if (IsKeyPressed(KEY_SPACE)) {
      game = !game;
      if (game)
        M_ResetData();
    }
    EndDrawing();
  }

  M_FreeSettingsView();
  CloseWindow();
  M_SaveData();

  return 0;
}
