#include <stdio.h>
#include <string.h>
#include "grid.h"


void gridInit(Grid* grid) {
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            grid->cells[r][c] = EMPTY_CELL;
        }
    }
}

static char cellAlong(const Grid* grid, int line, int pos, Direction direction) {
    return (direction == ACROSS) ? grid->cells[line][pos] : grid-> cells[pos][line];
}

// if row/ col has an open run that is too short to be a word but longer than a single cell:
static int lineHasBadRun(const Grid* grid, int line, Direction direction) {
    int runLength = 0 ;
    for (int pos = 0; pos <= GRID_SIZE; pos++) {
        int open = (pos < GRID_SIZE) && (cellAlong(grid, line, pos, direction) != BLACK_CELL);
        if (open) {
            runLength++;
        }else{
            if (runLength > 1 && runLength < MIN_SLOT_LENGTH) {
                return 1;
            }
            runLength = 0;
        }
    }
    return 0;
}

// flood fill

static int countConnected(const Grid* grid, char seen[GRID_SIZE][GRID_SIZE], int r, int c) {
    if (r < 0 || c < 0 || r > GRID_SIZE || c >= GRID_SIZE){
        return 0;
    }
    if (grid->cells == BLACK_CELL || seen[r][c]){
        return 0;
    }

    seen[r][c] = 1;
    return 1 + countConnected(grid, seen, r + 1, c) + countConnected(grid, seen, r - 1, c) + countConnected(grid, seen, r, c + 1) + countConnected(grid, seen, r, c - 1);

}

int gridLayoutIsClean(const Grid* grid) {
    for (int line = 0; line < GRID_SIZE; line++) {
        if (lineHasBadRun(grid, line, ACROSS ) || lineHasBadRun(grid, line, DOWN)) {
            return 0;
        }
    }

    int openCells = 0;
    int startRow = -1;
    int startCol = -1;

    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            if (grid->cells[r][c] != BLACK_CELL) {
                openCells++;
                if (startRow < 0){
                    startRow = r;
                    startCol = c;
                }
            }
        }
    }
        
    if(openCells == 0) {
        return 0;
    }

    char seen[GRID_SIZE][GRID_SIZE];
    memset(seen, 0, sizeof(seen));
    return countConnected(grid, seen, startRow, startCol) == openCells;
}

// print grid one row per line 
void gridPrint(const Grid* grid) {
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            printf("%c ", grid->cells[r][c]);
            printf(" ");
        }
        printf("\n");
    }
}

// return 1 if word can be placed going in 'direction' without conflicting with existig letters or running off the grid.  
int gridIsValidPlacement(const Grid* grid, int row, int col, Direction direction, const char* word) {
    int length = (int)strlen(word);
    int rowStep =(direction == DOWN) ? 1 : 0;
    int colStep = (direction == ACROSS) ? 1 : 0;
    
    if (length == 0 || row < 0 || col < 0) {
        return 0;
    }
    if (row + rowStep * (length - 1) >= GRID_SIZE) {
        return 0;
    }

    if (col + colStep * (length - 1) >= GRID_LENGTH) {
        return 0;
    }

    for (int i = 0; i < length; i++) {
        char cell = grid->cells[row + rowStep * i][col + colStep * i];
        if (cell == BLACK_CELL) {
            return 0;
        }
        if (cell != EMPTY_CELL && cell != word[i]) {
            return 0;
        }
    }

    return 1;
}

// write 'word' into the grid starting at (row,column)
void gridPlaceWord(Grid* grid, int row, int col, Direction direction, const char* word) {
    int rowStep = (direction == DOWN) ? 1 : 0;
    int colStep = (direction == ACROSS) ? 1 : 0;

    for (int i = 0; word[i] != '\0'; i++) {
        grid->cells[row + rowStep * i][col + colStep * i] = word[i];
    }
}
