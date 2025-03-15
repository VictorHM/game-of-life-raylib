/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Options Screen Functions Definitions (Init, Update, Draw, Unload)
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
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Needed for calloc(), free()

#define MAX_FILEPATH_RECORDED   4096
#define MAX_FILEPATH_SIZE       2048
//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
int transitionToScreen = 0;

 int filePathCounter = 0;
 char *filePaths[MAX_FILEPATH_RECORDED] = { 0 }; // We will register a maximum of filepaths

// Options Screen Initialization logic
void InitOptionsScreen(void) {
    // Allocate space for the required file path.
     for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
       filePaths[i] = (char *)RL_CALLOC(MAX_FILEPATH_SIZE, 1);
     }
}

void FilterFilesExt(void) {
    const char csv[] = {"csv"};
    // TODO seleccionar archivo desde el directorio.
    DIR *d;
    struct dirent *dir;
    // Open the directory where the system is. Hopefully it would be the one
    // where conmways is running.
    d = opendir(".");
    if (d) {
       while ((dir = readdir(d)) != NULL) {
           TraceLog(LOG_DEBUG, "file name: %s\n", dir->d_name);
           // TODO esto accede a memoria NO asignada. Segfault. Por que?
           char* tmp;
           tmp = dir->d_name;
           printf("%s\n", dir->d_name);
           // TODO creo que esto no es necesario. El while de arriba basta.
               char* token = strtok(tmp, ".");
               TraceLog(LOG_DEBUG, "file name: %s\n", token);
               TraceLog(LOG_DEBUG, "En tmp si hay csv o .csv?: %s\n", tmp);
               int returned_cmp = strcmp(tmp, csv);
               if (returned_cmp == 0) {
                  // Guardar nombre del directorio.
                  filePaths[filePathCounter] = dir->d_name;
                  filePathCounter++;
               }
        }
       TraceLog(LOG_DEBUG, "Cantidad de elementos dentro de path: %d", filePathCounter);
       for(int j = 0; j < filePathCounter; j++) {
          printf("Archivo: %s", filePaths[filePathCounter]);
          TraceLog(LOG_DEBUG, "Nombre de archivo cargado: %s", filePaths[filePathCounter]);
       }
      closedir(d);
    }
}

// Options Screen Update logic
void UpdateOptionsScreen(void)
{
    transitionToScreen = 0;
    if (IsKeyPressed(KEY_ONE))
    {
        // Cargar configuracion desde archivo
        //FilterFilesExt();
        transitionToScreen = 1;
        PlaySound(fxCoin);
    } else if (IsKeyPressed(KEY_TWO)) {
        transitionToScreen = 2;
        PlaySound(fxCoin);
    } else if (IsKeyPressed(KEY_THREE)) {
        // Back to TITLE screen
        transitionToScreen = 3;
        PlaySound(fxCoin);
    }

}

// Update rules screen. For now it does the same as the one
// above, but better to duplicate it for when they change
void UpdateRulesScreen(void)
{
    transitionToScreen = 0;
    if (IsKeyPressed(KEY_ONE))
    {
        // Cargar configuracion desde archivo
        //FilterFilesExt();
        transitionToScreen = 1;
        PlaySound(fxCoin);
    } else if (IsKeyPressed(KEY_TWO)) {
        transitionToScreen = 2;
        PlaySound(fxCoin);
    } else if (IsKeyPressed(KEY_THREE)) {
        // Back to TITLE screen
        transitionToScreen = 3;
        PlaySound(fxCoin);
    }

}

// Options Screen Draw logic
void DrawOptionsScreen(void)
{
    // TODO: Si esta en modo seleccion de .csv, no dibujar esto, sino un
    // recuadro donde cada linea sea uno de los .csv encontrados.
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
    Vector2 pos = { 50, 30 };
    DrawTextEx(font, "CONWAY'S GAME OF LIFE", pos, font.baseSize * 4, 4, DARKGREEN);
    pos.y = pos.y + 150;
    DrawTextEx(font, "1. CARGAR CONFIGURACION DESDE ARCHIVO", pos, font.baseSize * 2, 2, GREEN);
    pos.y = pos.y + 50;
    DrawTextEx(font, "2. ELEGIR SET DE REGLAS", pos, font.baseSize * 2, 2, GREEN);
    pos.y = pos.y + 50;
    DrawTextEx(font, "3. VOLVER AL TITULO", pos, font.baseSize * 2, 2, GREEN);
}

// Selection of Rules
void DrawRulesScreen(void) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
    Vector2 pos = { 50, 30 };
    DrawTextEx(font, "CONWAY'S GAME OF LIFE: Selecciona Reglas", pos, font.baseSize * 4, 4, DARKGREEN);
    pos.y = pos.y + 150;
    DrawTextEx(font, "1. Family Life: B3/S23 (Born if 3, Survive if 2 or 3)", pos, font.baseSize * 2, 2, GREEN);
    pos.y = pos.y + 50;
    DrawTextEx(font, "2. High Life: B36/S23", pos, font.baseSize * 2, 2, GREEN);
    pos.y = pos.y + 50;
    DrawTextEx(font, "3. VOLVER AL TITULO", pos, font.baseSize * 2, 2, GREEN);
}

// Options Screen Unload logic 
void UnloadOptionsScreen(void)
{
    // TODO: Unload OPTIONS screen variables here!
    for (int i = 0; i < MAX_FILEPATH_RECORDED; i++)
    {
        //TraceLog(LOG_DEBUG, "Liberando memoria de paths: %s", filePaths[i]);
        RL_FREE(filePaths[i]); // Free allocated memory for all filepaths
    }
}
// Options Screen should finish?
int FinishOptionsScreen(void)
{
    return transitionToScreen;
}
