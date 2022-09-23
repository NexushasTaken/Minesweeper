#include "button.h"

button initdefault() {
  button out = {
      .text = "Button",
      .x = 0,
      .y = 0,
      .width = 0,
      .height = 0,
      .font_color = BLACK,
      .background_color = (Color){200, 200, 200, 255},
      .border_color = (Color){100, 100, 100, 255},
      .hover_font_color = BLACK,
      .hover_background_color = (Color){220, 220, 220, 255},
      .hover_border_color = (Color){120, 120, 120, 255},
      .clicked_font_color = BLACK,
      .clicked_background_color = (Color){200, 200, 200, 255},
      .clicked_border_color = (Color){100, 100, 100, 255},
      .border_width = 2,
      .font = GetFontDefault(),
      .font_size = 16,
      .font_spacing = 4,
      .debug = 1,
      .state = btnNONE,
  };
  return out;
}

void buttonDraw(button *button) {
  Vector2 font_measure = MeasureTextEx(button->font, button->text,
                                       button->font_size, button->font_spacing);

  float bg_width = button->width - button->border_width * 2;
  float bg_height = button->height - button->border_width * 2;
  float bg_x = button->x + button->border_width;
  float bg_y = button->y + button->border_width;

  if (button->width <= font_measure.x) {
    float size = font_measure.x + 14;
    bg_width = size;
    button->width = size;
  }
  if (button->height <= font_measure.y) {
    float size = font_measure.y + 10;
    bg_height = size;
    button->height = size;
  }

  Vector2 btn_pos = {
      button->x + button->width / 2.0f - (font_measure.x / 2),
      button->y + button->height / 2.0f - (font_measure.y / 2),
  };

  switch (button->state) {
  case btnNONE:
    DrawRectangle(button->x, button->y, button->width, button->height,
                  button->border_color);
    DrawRectangle(bg_x, bg_y, bg_width, bg_height, button->background_color);
    DrawTextEx(button->font, button->text, btn_pos, button->font_size,
               button->font_spacing, button->font_color);
    break;
  case btnHOVER:
    DrawRectangle(button->x, button->y, button->width, button->height,
                  button->hover_border_color);
    DrawRectangle(bg_x, bg_y, bg_width, bg_height,
                  button->hover_background_color);
    DrawTextEx(button->font, button->text, btn_pos, button->font_size,
               button->font_spacing, button->hover_font_color);
    break;
  case btnCLICKED:
    DrawRectangle(button->x, button->y, button->width, button->height,
                  button->clicked_border_color);
    DrawRectangle(bg_x, bg_y, bg_width, bg_height,
                  button->clicked_background_color);
    DrawTextEx(button->font, button->text, btn_pos, button->font_size,
               button->font_spacing, button->clicked_font_color);
    break;
  }
}

void buttonUpdate(button *button) {
  int x = GetMouseX();
  int y = GetMouseY();
  if (button->x <= x && button->y <= y &&
      button->x + button->width >= x &&
      button->y + button->height >= y) {
    button->state = btnHOVER;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      button->state = btnCLICKED;
    }
  }
  else
    button->state = btnNONE;
}
