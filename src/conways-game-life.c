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
#include "dependencies/log.h"

#include <math.h>
#include <time.h>
#include <stdlib.h>

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define WIDTH 800
#define HEIGHT 500

#define CELL_NUMX 60
#define CELL_NUMY 40
#define CELL_NUM  CELL_NUMX * CELL_NUMY
#define SIZE_X WIDTH/CELL_NUMX
#define SIZE_Y HEIGHT/CELL_NUMY
//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 500;

static bool pause = false;
static bool gameOver = false;

static int xPos;
static int yPos;

static int cicles = 0;
static int delta = 0.1f;

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
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
  log_trace("INITIALIZE Generations");
  bool prevVal = false;
  // Initialize random number generator
  srand(time(NULL));
  // Initialize currGen
  for(int i = 0; i < CELL_NUMX; ++i) {
    for(int j = 0; j < CELL_NUMY; ++j) {
      // generate random number in [0,1]
      double rnd = ((double)rand()/((double)RAND_MAX + 1.0));
      // Do the clipping to the alive/dead state.
      bool val = (rnd <= 0.3) ? true : false;
      currGen[i][j] = val;
      prevGen[i][j] = val;
    }
  }
  log_trace("INITIALIZE Generations: ENDED");
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

// Checks the surrounding cells to determine if they are alive or not.
// Puts the result in a boolean 3x3 matrix. i,j is the cell to check.
void checkCellLife(int cellx, int celly) {
  int countAliveCells = 0;
  // Fill the value from prevGeneration. 
  // Check neightbours of the cell, so i, j go through the values -1, 0, -1
  for(int i = -1; i <= 1; ++i) {
    // Avoid unnecesary calculations that may set wrong values in the gen arrays.
    if (cellx+i >= 0 && (cellx+i) < CELL_NUMX) {
      for (int j = -1; j <= 1; ++j) {
        // Edge cases are taken into account in the conditions
        if ((celly+j >= 0 && (celly+j) < CELL_NUMY) && prevGen[cellx+i][celly+j]) {
          ++countAliveCells;
        }
      }
    }
  }

  // Check if the cell in cellx, celly is alive or dead.
  // Apply rules based on that.
  bool currCellAlive = prevGen[cellx][celly];
  if (currCellAlive) {
    if (countAliveCells < 2 || countAliveCells > 3) {
      currGen[cellx][celly] = false;
    } else {
      currGen[cellx][celly] = true;
    }
  } else {
    // Cell is dead, if exactly 3 cells alive around, reborn.
    if (countAliveCells == 3) {
      currGen[cellx][celly] = true;
    }
  }
  // Copy curr gen to prev gen so it will update every cycle.
  prevGen[cellx][celly] = currGen[cellx][celly];
}

// TODO idea: checkCellLife comprueba cada celula en prevGen.
// Determina cual ha de vivir y cual no.
// \returns Lista de celulas a dibujar: position y size.
// De esa forma, solo pasamos la lista de elementos a pintar, con su posicion
// y tamanos ya definidos.
///////////////////////////////////////////
void evaluateContinuationOfLive(void) {
  if (cicles > 100) {
    for(int i = 0; i < CELL_NUMX; ++i) {
      for(int j = 0; j < CELL_NUMY; ++j) {
        checkCellLife(i, j);
      }
    }
    cicles = 0;
  }
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
    log_trace("Starting the show...");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(30);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        cicles++;
        log_trace("Ciclo: %d", cicles);
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
    xPos = screenWidth/2;
    yPos = screenHeight/2;
    initializeGenerations();
}

// Draw game (one frame)
void DrawGame(void)
{
  BeginDrawing();
      ClearBackground(WHITE);
      if (!gameOver)
      {
				int posX = 0;
        int posY = 0;
        for (int i = 0; i < CELL_NUMX; i++) {
          for (int j = 0; j < CELL_NUMY; j++) {
            // Check if the cell has to be drawn.
            if (currGen[i][j]){
              DrawRectangle(posX, posY, SIZE_X-delta, SIZE_Y-delta, GREEN); // TODO esto es lo que esta mal! Hay que moverse varios pixels y no lo hace bien
              //log_trace("Print X: %d", posX);
              //log_trace("/tPrint Y: %d", posY);
            }
            posY += SIZE_Y;
          }
          posY = 0;
          posX += SIZE_X;
        }            
        // DrawRectangle(xPos, yPos, 2*SIZE_X, 2*SIZE_Y, RED);
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
        if(IsKeyDown(KEY_UP)) {
          --yPos; // TODO check when it is 0 or negative. Howe to wrap up the screen?
        }
        if(IsKeyDown(KEY_DOWN)) {
          ++yPos;
        }
        if(IsKeyDown(KEY_LEFT)) {
          --xPos;
        }
        if(IsKeyDown(KEY_RIGHT)) {
          ++xPos;
        }
        // Precompute a list of Cells ready to paint.
        //PrecomputeSquaresToDraw();
        //if(IsKeyPressed(KEY_L)) {
        //  evaluateContinuationOfLive();          
        //}
        evaluateContinuationOfLive();
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
