/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"
#include <time.h>
#include <stdlib.h>


//#define TESTING
//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

static int xPos;
static int yPos;

static int cicles = 0;
static int delta = 0.1f;

static bool currGen [CELL_NUMX][CELL_NUMY] = {0};
static bool prevGen [CELL_NUMX][CELL_NUMY] = {0};
//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    //if (isLoaded) {
    //  // TODO load the csv data here.
    //}
    framesCounter = 0;
    finishScreen = 0;
    TraceLog(LOG_DEBUG, "INITIALIZE Generations");
    bool prevVal = false;
#ifdef TESTING
  for(int i = 0; i < CELL_NUMX; ++i) {
      for(int j = 0; j < CELL_NUMY; ++j) {
        currGen[i][j] = 0;
        prevGen[i][j] = 0;
      }
  }
  // Now we set a glider that should survive and move
  currGen[24][14] = 1;
  currGen[25][15] = 1;
  currGen[26][15] = 1;
  currGen[24][16] = 1;
  currGen[25][16] = 1;

  prevGen[24][14] = 1;
  prevGen[25][15] = 1;
  prevGen[26][15] = 1;
  prevGen[24][16] = 1;
  prevGen[25][16] = 1;

#else
    // Initialize random number generator
    srand(time(NULL));
    // Initialize currGen
    for(int i = 0; i < CELL_NUMX; ++i) {
      for(int j = 0; j < CELL_NUMY; ++j) {
        // generate random number in [0,1]
        currGen[i][j] = rand() % 2;
        prevGen[i][j] = rand() % 2;
        TraceLog(LOG_DEBUG, "Value: %d, X: %d, Y: %d", currGen[i][j], i, j);
      }
    }
#endif
    TraceLog(LOG_DEBUG, "INITIALIZE Generations: ENDED");
}

void CheckCellLife(int cellx, int celly) {
  int countAliveCells = 0;
  for (int j = -1; j <= 1; j++) {
      for (int i = -1; i <= 1; i++) {
          if (i == 0 && j == 0) continue;
          int x = cellx + i;
          int y = celly + j;
          if (x >= 0 && x < CELL_NUMX && y >= 0 && y < CELL_NUMY) {
              if (prevGen[x][y]) countAliveCells++;
          }
      }
  }

  bool currCellAlive = prevGen[cellx][celly];
  if (currCellAlive) {
      if (countAliveCells < 2 || countAliveCells > 3) {
          currGen[cellx][celly] = 0;
      } else {
          currGen[cellx][celly] = 1;
      }
  } else {
      if (countAliveCells == 3) {
          currGen[cellx][celly] = 1;
      }
  }
}

void UpdateGeneration() {
  for (int x = 0; x < CELL_NUMX; x++) {
      for (int y = 0; y < CELL_NUMY; y++) {
          CheckCellLife(x, y);
      }
  }

  for (int x = 0; x < CELL_NUMX; x++) {
      for (int y = 0; y < CELL_NUMY; y++) {
          prevGen[x][y] = currGen[x][y];
      }
  }
}

// Update current generation life state from prev.
int copyCurrGenToPrev() {
  // Copy arrays to keep updated. Doing it in checkLifeCells breaks logic.
  // TODO contar celulas vivas en cada iteraccion
  int aliveCells = 0;
  for (int i = 0; i < CELL_NUMX; ++i) {
    for (int j = 0; j < CELL_NUMY; ++j) {
      if (currGen[i][j]) ++aliveCells;
      prevGen[i][j] = currGen[i][j];
    }
  }
  return aliveCells;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
    //cicles++;

    // TODO anadir lectura de input key para avanzar un ciclo, y asi poder ver que falla en la logica.
    if (cicles >= 50 || IsKeyPressed(KEY_SPACE)) {
      TraceLog(LOG_DEBUG, "Nuevo Ciclo.");
      UpdateGeneration();
      TraceLog(LOG_DEBUG, "Nuevo Ciclo: %d", cicles);
      cicles = 0;
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    int posX = 0;
    int posY = 0;
    // TODO this could be split in several parallel processing. Each row,
    // each cell, is independent of any other at this point (the state has
    // been already calculated).
    for (int i = 0; i < CELL_NUMX; i++) {
      for (int j = 0; j < CELL_NUMY; j++) {
        // Check if the cell has to be drawn.
        if (currGen[i][j]){
          DrawRectangle(posX, posY, SIZE_X-delta, SIZE_Y-delta, GREEN); // TODO esto es lo que esta mal! Hay que moverse varios pixels y no lo hace bien
        }
        DrawRectangleLines(posX, posY, SIZE_X, SIZE_Y, RED);
        posY += SIZE_Y;
      }
      posY = 0;
      posX += SIZE_X;
    }            
    DrawRectangle(xPos, yPos, 2*SIZE_X, 2*SIZE_Y, RED);
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}
