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
#include "includes/life_rules.h"
#include <time.h>
#include <stdlib.h>


//#define TESTING
//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;

static int xPos = 0;
static int yPos = 0;

static int cicles = 0;
static int delta = 0.1f;

static bool currGen [MAX_CELL_X][MAX_CELL_Y] = {0};
static bool prevGen [MAX_CELL_X][MAX_CELL_Y] = {0};
//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    framesCounter = 0;
    finishScreen = 0;
    TraceLog(LOG_DEBUG, "INITIALIZE Generations");
#ifdef TESTING
  for(int i = 0; i < MAX_CELL_X; ++i) {
      for(int j = 0; j < MAX_CELL_Y; ++j) {
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
    for(int i = 0; i < MAX_CELL_X; ++i) {
      for(int j = 0; j < MAX_CELL_Y; ++j) {
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
          if (x >= 0 && x < MAX_CELL_X && y >= 0 && y < MAX_CELL_Y) {
              if (prevGen[x][y]) countAliveCells++;
          }
      }
  }

  bool currCellAlive = prevGen[cellx][celly];
  if (currCellAlive) {
      if (countAliveCells < minSurvive || countAliveCells > maxSurvive) {
          currGen[cellx][celly] = 0;
      } else {
          currGen[cellx][celly] = 1;
      }
  } else {
      if (countAliveCells >= minBorn && countAliveCells <= maxBorn) {
          currGen[cellx][celly] = 1;
      }
  }
}

void UpdateGeneration() {
  for (int x = 0; x < MAX_CELL_X; x++) {
      for (int y = 0; y < MAX_CELL_Y; y++) {
          CheckCellLife(x, y);
      }
  }

  for (int x = 0; x < MAX_CELL_X; x++) {
      for (int y = 0; y < MAX_CELL_Y; y++) {
          prevGen[x][y] = currGen[x][y];
      }
  }
}

// Update current generation life state from prev.
int copyCurrGenToPrev() {
  int aliveCells = 0;
  for (int i = 0; i < MAX_CELL_X; ++i) {
    for (int j = 0; j < MAX_CELL_Y; ++j) {
      if (currGen[i][j]) ++aliveCells;
      prevGen[i][j] = currGen[i][j];
    }
  }
  return aliveCells;
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // Update sizes and positions based on window

    // TODO move this to init or some better place.
    // TODO Algorithm to try to estimate the right size for the drawing cells

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        PlaySound(fxCoin);
    }
    // TODO for red square. Consider using MouseKeyPress and related funtions
    // to destroy the cell at that point, instead of using the red square.
    if (IsKeyDown(KEY_LEFT))
    {
        xPos -= CELL_WIDTH;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        xPos += CELL_WIDTH;
    }
    if (IsKeyDown(KEY_UP))
    {
        yPos -= CELL_HEIGHT;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        yPos += CELL_HEIGHT;
    }
    cicles++;

    // Pressing SPACE advance 1 cicle of cells
    // TODO for gameplay.
    // User can press SPACE or wait 150 cicles to see the next generation.
    if (cicles >= 100 || IsKeyPressed(KEY_SPACE)) {
      UpdateGeneration();
      cicles = 0;
    }
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    int posX = 0;
    int posY = 0;
    for (int i = 0; i < MAX_CELL_X; i++) {
      for (int j = 0; j < MAX_CELL_Y; j++) {
        // Check if the cell has to be drawn.
        if (currGen[i][j]){
          DrawRectangle(posX, posY, CELL_WIDTH-delta, CELL_HEIGHT-delta, GREEN);
        }
        DrawRectangleLines(posX, posY, CELL_WIDTH, CELL_HEIGHT, RED);
        posY += CELL_HEIGHT;
      }
      posY = 0;
      posX += CELL_WIDTH;
    }            
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
