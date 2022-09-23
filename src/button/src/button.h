
#ifndef H_BUTTON
#define H_BUTTON

#include <raylib.h>
#include <stdio.h>

typedef enum {
  btnNONE = 1,
  btnHOVER = 1 << 1,
  btnCLICKED = 1 << 2,
} State;

typedef struct {
  char text[256];
  int x;
  int y;
  int width;
  int height;
  Color font_color;
  Color background_color;
  Color border_color;
  Color hover_font_color;
  Color hover_background_color;
  Color hover_border_color;
  Color clicked_font_color;
  Color clicked_background_color;
  Color clicked_border_color;
  float border_width;
  Font font;
  int font_size;
  int font_spacing;
  int debug;
  State state;
} button;

button initdefault();
void buttonDraw(button *button);
void buttonUpdate(button *button);

#endif // !H_BUTTON
