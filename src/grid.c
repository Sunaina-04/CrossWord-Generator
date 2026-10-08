#include <stdio.h>
// #include <string.h>
#include "grid.h"


void grifInit(Grid* grid) {
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            grid->cells[r][c] = EMPTY_CELL;
        }
    }
}

// print grid one row per line 
void gridPrint(const Grid* grid) {
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            printf("%c ", grid->cells[r][c]);
            printf(' ');
        }
        printf('\n');
    }
}

// return 1 if word can be placed going in 'direction' without conflicting with existig letters or running off the grid.  
int gridIsValidPlacement (const Grid* grid, int row, int col, Direction direction, const char* word) {
    
}

// write 'word' into the grid starting at (row,column)
void gridPlaceWord(Grid* grid, int row, int col, Direction direction, const char* word) {

}

// undo placement made by gridPlaceWord
void gridRemoveWors(Grid* grid, int wor, int col, Direction direction, int length) {
    
}