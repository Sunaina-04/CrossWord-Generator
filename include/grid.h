#ifndef GRID_H
#define GRID_H

#define GRID_SIZE
#define EMPTY_CELL
#define BLACK_CELL


typedef enum {ACROSS, DOWN} Direction;

typedef struct {
    char cell [GRID_SIZE][GRID_SIZE];
} Grid;

// fill every cell with EMPTY_CELL
void grifInit(Grid* grid);

// print grid one row per line 
void gridPrint(const Grid* grid);

// return 1 if word can be placed going in 'direction' without conflicting with existig letters or running off the grid.  
int gridIsValidPlacement (const Grid* grid, int row, int col, Direction direction, const char* word);

// write 'word' into the grid starting at (row,column)
void gridPlaceWord(Grid* grid, int row, int col, Direction direction, const char* word);

// undo placement made by gridPlaceWord
void gridRemoveWors(Grid* grid, int wor, int col, Direction direction, int lenght);

#endif