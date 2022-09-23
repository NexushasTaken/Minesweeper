#include "settings_view.h"
#include "button/src/button.h"
#include "string.h"
#include <raylib.h>
#include <stdio.h>
#include "data.h"
#include "limits.h"
#include "utils.h"

static button difficulty = {0};
static button col_inc = {0};
static button col_dec = {0};

static button row_inc = {0};
static button row_dec = {0};

static button bomb_inc = {0};
static button bomb_dec = {0};

static bool diff_clicked = false;
static bool row_clicked  = false;
static bool col_clicked  = false;
static bool bomb_clicked = false;
static char row_text[10];
static char col_text[10];
static char bomb_text[10];
static char statistic_text[256];

static Font hack_font;
static int font_size = 14;
static int gap   = 2;

extern int grid_limit_x;
extern int grid_limit_y;

extern Rectangle body_b;
extern Rectangle board_b;
extern Data data;

const char *diffs[5] = {
  "Begginer",
  "Intermediate",
  "Expert",
  "Custom",
};
const char *diff = NULL;

static button *buttons[3][2] = {
  {&row_dec, &row_inc},
  {&col_dec, &col_inc},
  {&bomb_dec, &bomb_inc},
};

static void update_buttons(){
  buttonUpdate(&difficulty);
  buttonUpdate(&col_inc);
  buttonUpdate(&col_dec);
  buttonUpdate(&row_inc);
  buttonUpdate(&row_dec);
  buttonUpdate(&bomb_inc);
  buttonUpdate(&bomb_dec);
}
static void update_pos() {
  update_buttons();

  float size_w = 21;
  float size_h = 24;
  int start_x = difficulty.x;
  int start_y = difficulty.y + size_h + gap;

  printf("diff %d\n", difficulty.height);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
      button *btn = buttons[i][j];

      btn->font_size = font_size;
      btn->font = hack_font;
      btn->font_spacing = 1;
      btn->width = size_w;
      btn->height = size_h;
      if(j)
        btn->x = start_x + difficulty.width - btn->width;
      else
        btn->x = start_x;
      btn->y = start_y;
    }
    start_y += size_h + gap;
  }
  update_buttons();
}

void M_InitSettingsView(){
  difficulty = initdefault();
  col_inc = initdefault();
  col_dec = initdefault();
  row_inc = initdefault();
  row_dec = initdefault();
  bomb_inc = initdefault();
  bomb_dec = initdefault();

  hack_font = LoadFont("./assets/hack.ttf");

  difficulty.font = hack_font;
  difficulty.font_spacing = 0;
  difficulty.font_size = font_size;
  difficulty.width = 98;
  difficulty.x = board_b.x + BORDER_SIZE * 4;
  difficulty.y = board_b.y;

  strcpy(col_inc.text, ">");
  strcpy(col_dec.text, "<");
  strcpy(row_inc.text, ">");
  strcpy(row_dec.text, "<");
  strcpy(bomb_inc.text, ">");
  strcpy(bomb_dec.text, "<");
  update_pos();
}

void M_UpdateSettingsView(){
  update_buttons();

  strcpy(difficulty.text, diffs[data.difficulty]);

  if(difficulty.state == btnCLICKED && !diff_clicked) {
    data.difficulty += 1;
    data.difficulty %= NONE;
    diff_clicked = !IsKeyDown(KEY_LEFT_SHIFT);
  }

  if(bomb_inc.state == btnCLICKED ||
     bomb_dec.state == btnCLICKED ||
     row_inc.state == btnCLICKED ||
     row_dec.state == btnCLICKED ||
     col_inc.state == btnCLICKED ||
     col_dec.state == btnCLICKED) {
    data.difficulty = NONE;

    if(row_inc.state == btnCLICKED && !row_clicked) {
      data.user_grid_size.x++;

      row_clicked = !IsKeyDown(KEY_LEFT_SHIFT);
    }
    if(row_dec.state == btnCLICKED && !row_clicked) {
      data.user_grid_size.x--;
      row_clicked = !IsKeyDown(KEY_LEFT_SHIFT);
    }

    if(col_inc.state == btnCLICKED && !col_clicked) {
      data.user_grid_size.y++;
      col_clicked = !IsKeyDown(KEY_LEFT_SHIFT);
    }
    if(col_dec.state == btnCLICKED && !col_clicked) {
      data.user_grid_size.y--;
      col_clicked = !IsKeyDown(KEY_LEFT_SHIFT);
    }

    if(bomb_inc.state == btnCLICKED && !bomb_clicked) {
      data.user_bomb_percent++;
      bomb_clicked = !IsKeyDown(KEY_LEFT_SHIFT);
    }
    if(bomb_dec.state == btnCLICKED && !bomb_clicked) {
      data.user_bomb_percent--;
      bomb_clicked = !IsKeyDown(KEY_LEFT_SHIFT);
    }

    M_UserNormalize();
  }

  if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    diff_clicked = false;
    row_clicked  = false;
    col_clicked  = false;
    bomb_clicked = false;
  }
  SetWindowSize(300, 400);
}

void M_DrawSettingsView() {
  buttonDraw(&difficulty);
  buttonDraw(&col_inc);
  buttonDraw(&col_dec);
  buttonDraw(&row_inc);
  buttonDraw(&row_dec);
  buttonDraw(&bomb_inc);
  buttonDraw(&bomb_dec);

  DrawTextEx(hack_font, "Diff.",
              (Vector2){board_b.x + gap, difficulty.y + gap * 2 + 1},
              font_size, 1, BLACK);
  char *text;
  for (int i = 0; i < 3; i++) {
    button *btn = buttons[i][0];
    switch (i) {
      case 0:
        text = "Rows ";
        break;
      case 1:
        text = "Cols ";
        break;
      case 2:
        text = "Bombs";
        break;
    }
    DrawTextEx(hack_font, text,
              (Vector2){board_b.x + gap, btn->y + gap * 2 + 1},
              font_size, 1, BLACK);
  }

  sprintf(row_text, "%d", (int)data.user_grid_size.x);
  DrawTextEx(hack_font, row_text,
            (Vector2){row_dec.x + row_dec.width + gap * 2, row_dec.y + gap * 2 + 1},
            font_size, 1, BLACK);

  sprintf(col_text, "%d", (int)data.user_grid_size.y);
  DrawTextEx(hack_font, col_text,
            (Vector2){col_dec.x + col_dec.width + gap * 2, col_dec.y + gap * 2 + 1},
            font_size, 1, BLACK);

  sprintf(bomb_text, "%d%%", (int)data.user_bomb_percent);
  DrawTextEx(hack_font, bomb_text,
            (Vector2){bomb_dec.x + bomb_dec.width + gap * 2, bomb_dec.y + gap * 2},
            font_size + 1, 1, BLACK);

  DrawTextEx(hack_font, "Click and Hold shift to\n  increase/decrease the value faster",
            (Vector2){gap + BORDER_SIZE, bomb_dec.y + gap * 2 + bomb_dec.height},
            font_size, 0, BLACK);

  sprintf(statistic_text,
"     Statistics\n"
"Diff.         Time\n"
"Begginer      %.2fs\n"
"Intermediate  %.2fs\n"
"Expert        %.2fs\n"
"-1s means you did not play and\n"
" solve that difficulty yet",
  data.begginer_time, data.intermediate_time, data.expert_time);
  DrawTextEx(hack_font, statistic_text,
            (Vector2){gap + BORDER_SIZE, bomb_dec.y + bomb_dec.height * 3},
            font_size, 0, BLACK);
}

void M_FreeSettingsView() {
  UnloadFont(hack_font);
}
