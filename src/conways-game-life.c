/*******************************************************************************************
*
*   raylib - Conway's Game of Life
*
*   Implementation of Conway's Game of Life in Raylib.
*
*   The goal is to use the project as a way to learn Raylib.
*
*   This game has been created using raylib v1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include <math.h>
#include <time.h>
#include <stdlib.h>

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define WIDTH 800
#define HEIGHT 500

#define CELL_NUMX 80
#define CELL_NUMY 50
#define CELL_NUM  CELL_NUMX * CELL_NUMY
#define SIZE_X 80/WIDTH
#define SIZE_Y 50/HEIGHT
//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 500;

static bool pause = false;
static bool gameOver = false;

// Numero de celulas en eje X y eje Y.
//const int cellsX = CELL_NUM/screenWidth;
//const int cellsY = CELL_NUM/screenHeight;
//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct Cell {
  bool alive;
  Vector2 position;  // Position relative to the cell grid.
} Cell;

// Necesito estructura para almacenar cada celda existente en el tablero, muerta
// o no. Y con eso tal vez TODO deprecar los dos arrays de bool. Porque Cell ya
// tiene dentro un bool para alive.
Vector2 totalCells[CELL_NUM] = {0};
bool currGen [CELL_NUMX][CELL_NUMY] = {0};
bool prevGen [CELL_NUMX][CELL_NUMY] = {0};

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateDrawGame(void);       // Update game (one frame)
static void UnloadGame(void);       // Unload game
//------------------------------------------------------------------------------------
// Auxiliary functions
//------------------------------------------------------------------------------------
// It knows the size of array. No need to pass it.
void initializeGenerations(void) {
  // Initialize random number generator
  srand(time(NULL));
  // Initialize currGen
  for(int i = 0; i < CELL_NUMX; ++i) {
    for(int j = 0; j < CELL_NUMY; ++j) {
      // TODO generate random number in [0,1]
      double rnd = ((double)rand()/((double)RAND_MAX + 1.0));
      // Do the clipping.
      bool val = (rnd <= 0.5) ? true : false;
      currGen[i][j] = val;
      prevGen[i][j] = val;
    }
  }
}

void initPositionCells (struct Cell* cell) {
}
//
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

// Checks the surrounding cells to determine if they are alive or not.
// Puts the result in a boolean 3x3 matrix. i=j is the cell to check.
void checkCellLife(int posx, int posy) {
  int countAliveCells = 0;
  // Fill the value from prevGeneration.
  for(int i = -1; i <= 1; ++i) {
      if ((posx+i >= 0) && prevGen[posx+i]) ++countAliveCells;
  }

  // Check if the cell in posx, posy is alive or dead.
  // Apply rules based on that.
  bool currCellAlive = prevGen[posx][posy];
  if (currCellAlive) {
    if (countAliveCells < 2 || countAliveCells > 3) {
      currGen[posx][posy] = false;
    } else {
      currGen[posx][posy] = true;
    }
  } else {
    // Cell is dead, if exactly 3 cells alive around, reborn.
    if (countAliveCells == 3) {
      currGen[posx][posy] = true;
    }
  }
}

void checkCellsLife(void){

}
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Conwan\'s Game of Life");

    InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawGame();
        //----------------------------------------------------------------------------------
    }
#endif
    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadGame();         // Unload loaded data (textures, sounds, models...)

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Initialize game variables and initial seed setup for Conwans.
void InitGame(void) {
    gameOver = false;
    pause = false;
    initializeGenerations();
}


// Draw game (one frame)
void DrawGame(void)
{
    BeginDrawing();
        ClearBackground(RAYWHITE);
        if (!gameOver)
        {
          // Draw each square of current cells
          // Go through each element of currGen[i][j] and its value determines
          // if it needs to be drawn.
          // TODO this is horrible code. I should write more clear one.
          for(int cl = 0; cl < CELL_NUM; ++cl) {
            for (int x = 0; x < CELL_NUMX; x++) {
              for (int y = 0; y < CELL_NUMY; y++) {
                // DrawRectangle(Vector2 position, Vector2 size, Color color)
                if(currGen[x][y]) {
                  DrawRectangle(totalCells[cl].x, totalCells[cl].y, x*SIZE_X, y*SIZE_Y, GREEN);
                }
              }
            }
          }
        }
    EndDrawing();
}

// Update the game, one frame at a time
void UpdateGame(void) {
    if(!gameOver) {
      if (IsKeyPressed('P')) pause = !pause;
      if (IsKeyPressed(KEY_ESCAPE)) gameOver = true;

      if (!pause) {
        // Cell life logic: checks every cell and decide if they born, live,
        // die.
        checkCellsLife();
        DrawGame();
      }
    }
}

void UpdateDrawGame(void) {
  UpdateGame();
}

void UnloadGame(void) {
  // TODO
}
