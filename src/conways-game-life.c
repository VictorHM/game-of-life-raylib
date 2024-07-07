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

// Numero de celulas en eje X y eje Y.
//const int cellsX = CELL_NUM/screenWidth;
//const int cellsY = CELL_NUM/screenHeight;
//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct Cell {
  bool alive;
  Rectangle cellbody;  // 4 float values [16 bytes]. Each Rectangle is Rectangle(posx, posy, posx+SIZE_X, posy+SIZE_Y)
} Cell;

// Necesito estructura para almacenar cada celda viva en el tablero.
// Solo tengo que usar un algoritmo para calcular la posicion en x e y a partir 
// de una celda.
// Este vector contendra solo las celdas vivas, en concreto, sus posiciones.
Cell totalCells[CELL_NUM] = {0};
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
      bool val = (rnd <= 0.5) ? true : false;
      currGen[i][j] = val;
      prevGen[i][j] = val;
      totalCells[i+j].alive = val;
      totalCells[i+j].cellbody.x = i*10;
      totalCells[i+j].cellbody.y = j*10;
      totalCells[i+j].cellbody.height = SIZE_Y - delta;
      totalCells[i+j].cellbody.width = SIZE_X - delta;
    }
  }
  log_trace("INITIALIZE Generations: ENDED");
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

// Checks the surrounding cells to determine if they are alive or not.
// Puts the result in a boolean 3x3 matrix. i=j is the cell to check.
bool checkCellLife(int posx, int posy) {
  bool isAlive = false;
  int countAliveCells = 0;
  // Fill the value from prevGeneration. 
  
  // TODO Aclarar pantalla y dibujar cada celda segun se comprueba. Asi puedo pdeterminar si la logica esta bien

  // Check neightbours of the cell, so i, j go through the values -1, 0, -1
  for(int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      // TODO edge cases like the cell at the right border.
      if ((posx+i*10 >= 0 && (posx+i) < WIDTH) && (posy+j*10 >= 0 && (posy+j) < HEIGHT) 
          && prevGen[posx+i][posy+j]) {
        ++countAliveCells;
      }
    }
  }

  // Check if the cell in posx, posy is alive or dead.
  // Apply rules based on that.
  bool currCellAlive = prevGen[posx][posy];
  if (currCellAlive) {
    if (countAliveCells < 2 || countAliveCells > 3) {
      currGen[posx][posy] = false;
      isAlive = false;
    } else {
      currGen[posx][posy] = true;
      isAlive = true;
    }
  } else {
    // Cell is dead, if exactly 3 cells alive around, reborn.
    if (countAliveCells == 3) {
      currGen[posx][posy] = true;
      isAlive = true;
    }
  }
  // Copy curr gen to prev gen so it will update every cycle.
  prevGen[posx][posy] = currGen[posx][posy];
  return isAlive;
}

// TODO idea: checkCellLife comprueba cada celula en prevGen.
// Determina cual ha de vivir y cual no.
// \returns Lista de celulas a dibujar: position y size.
// De esa forma, solo pasamos la lista de elementos a pintar, con su posicion
// y tamanos ya definidos.
///////////////////////////////////////////
void evaluateContinuationOfLive(void) {
  for(int i = 0; i < CELL_NUMX; ++i) {
    for(int j = 0; j < CELL_NUMY; ++j) {
      checkCellLife(i, j);
    }
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
    SetTargetFPS(60);
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
    // TODO save the results to a text file to check if there is anything alive.

}

// Draw game (one frame)
void DrawGame(void)
{
    BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!gameOver)
        {
					int posX = 0;
          int posY = 0;
          for (int i = 0; i < CELL_NUMX; i++) {
            for (int j = 0; j < CELL_NUMY; j++) {
              // TODO this is the part where it checks if draw needs to be done.
              if (currGen[i][j]){
                DrawRectangle(posX, posY, SIZE_X-delta, SIZE_Y-delta, GREEN); // TODO esto es lo que esta mal! Hay que moverse varios pixels y no lo hace bien
                log_trace("Print X: %d", posX);
                log_trace("/tPrint Y: %d", posY);
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
        if(IsKeyPressed(KEY_L)) {
          evaluateContinuationOfLive();
        }
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
