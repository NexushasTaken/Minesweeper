cmake_minimum_required(VERSION 3.23.1)
project(Minesweeper)

add_subdirectory(src/button)
find_package(raylib 4.2.0)

add_compile_options(-Wall -Wextra -g)

add_executable(${PROJECT_NAME}
  src/main.c
  src/textures.c
  src/data.c
  src/board.c
  src/utils.c
  src/draw.c
  src/settings_view.c)

target_link_libraries(${PROJECT_NAME} button)
target_link_libraries(${PROJECT_NAME} raylib)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 17)
