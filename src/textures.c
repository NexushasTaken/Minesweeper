#include "textures.h"

Textures textures = {0};

void M_LoadTextures() {
  textures.tiles      = LoadTexture("assets/tiles.png");
  textures.corners    = LoadTexture("assets/corners.png");
  textures.separator  = LoadTexture("assets/separator.png");
  textures.border_hor = LoadTexture("assets/border_hor.png");
  textures.border_ver = LoadTexture("assets/border_ver.png");
  textures.border2x   = LoadTexture("assets/border2x.png");
  textures.digits_bg  = LoadTexture("assets/nums_bg.png");
  textures.digits     = LoadTexture("assets/digits.png");
  textures.faces      = LoadTexture("assets/faces.png");
}

void M_FreeTextures() {
  UnloadTexture(textures.tiles);
  UnloadTexture(textures.corners);
  UnloadTexture(textures.separator);
  UnloadTexture(textures.border_hor);
  UnloadTexture(textures.border_ver);
  UnloadTexture(textures.border2x);
  UnloadTexture(textures.digits_bg);
  UnloadTexture(textures.digits);
  UnloadTexture(textures.faces);
}
