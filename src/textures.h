#ifndef H_TEXTURES
#define H_TEXTURES

#include <raylib.h>

typedef struct textures {
  Texture2D tiles;
  Texture2D corners;
  Texture2D separator;
  Texture2D border_hor;
  Texture2D border_ver;
  Texture2D border2x;
  Texture2D digits_bg;
  Texture2D digits;
  Texture2D faces;
} Textures;

void M_LoadTextures();
void M_FreeTextures();

#endif
