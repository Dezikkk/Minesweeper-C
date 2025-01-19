cmake_minimum_required(VERSION 3.29)
project(Minesweeper-C C)

set(CMAKE_C_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/inc)

set(SOURCES
        src/main.c
        src/board.c
        src/game.c
        src/program.c
        src/menu.c
)

add_executable(Minesweeper-C ${SOURCES})



