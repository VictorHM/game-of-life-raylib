/* Loadinour configurations from files to start the game*/

// TODO funciones: abrir archivo
//leer filas
#include <stdio.h>
#include <stdlib.h>
// TODO this is C style. We can try C++ style later

// TODO Review the signature of the function to return possible
// errors or conditions to inform the main program.
void readConfig(bool* initCellsMatrix, size_t cellsX, size_t cellsY){
  FILE* stream = fopen("initconfig.csv", "r");
  if (stream == NULL){
    return;
  }
  // it reads botht he values for cellX and the separators.
  // So we multiply by two.
  char* line = NULL;
  size_t read;
  ssize_t len = 0;
  int row = 0;
  while((read = getline(&line, &len, stream)) != -1){
    printf("Retrieved line of length %zu:\n", read);
    //printf("%s", line);
    for(int i = 0; i < cellsX*2; ++i) {
      if (line[i] == '0'){
        initCellsMatrix[i][row] = false;
      } else if(line[i] == '1'){
        initCellsMatrix[i][row] = true;
      }
    }
    // Increment row number.
    row++;
  }

  fclose(stream);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
};

