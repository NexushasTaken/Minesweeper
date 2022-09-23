#ifndef H_STATUS
#define H_STATUS

#include <raylib.h>
#include <stdbool.h>
#include <sys/time.h>

#define BORDER_SIZE 14
#define TILE_SIZE   18
#define FACE_HEIGHT 30
#define STATUS_BODY_HEIGHT 36

typedef enum {
  // Official difficulties {
  // Begginer is default
  //   Difficulty
  BEGGINER = 0,
  INTERMEDIATE,
  EXPERT,
  // }
  NONE,
} Difficulty;

typedef enum face_status {
  FACE = 0,
  FACE_PRESSED,
  FACE_WIN,
  FACE_LOSE,
  FACE_ACTIVE,
} Face;

typedef struct data {
  Difficulty difficulty;
  Vector2 grid_size;
  Vector2 user_grid_size;
  int user_bomb_percent;

  struct timeval start_time;
  Face face_status;
  bool playing;
  bool bombs_generated;
  int  timer_count;
  int  bomb_count;
  int  flag_count;
  int  tile_left;
  // Best time
  float begginer_time;
  float intermediate_time;
  float expert_time;
} Data;

void M_LoadData();
void M_SaveData();
void M_ResetData();
void M_FreeTiles();
void M_UserNormalize();

#endif // !H_STATUS
