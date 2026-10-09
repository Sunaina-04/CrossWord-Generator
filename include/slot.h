#ifndef SLOT_H
#define SLOT_H

#include "grid.h"

// One fillable word position in the grid. 
typedef struct {
    int row;
    int col;
    int length;
    Direction direction;
} Slot;

// A growable list of slots.
typedef struct {
    Slot* slots;
    int count;
    int capacity;
} SlotList;

// Allocate a slot list with room for `capacity` entries. 
SlotList* slotListCreate(int capacity);

// Append `slot` to the list, growing it if needed.
void slotListAdd(SlotList* list, Slot slot);

// Free the list's internal array and the list struct itself.
void slotListFree(SlotList* list);

SlotList* slotListFromGrid(const Grid* grid);

#endif