#include <stdio.h>
#include <stdlib.h>
#include "slot.h"

SlotList* slotListCreate(int capacity) {
    SlotList* list = (SlotList*)malloc(sizeof(SlotList));
    if (!list) {
        fprintf(stderr, "slotListCreate: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    list->slots = (Slot*)malloc(sizeof(Slot) * capacity);
    if (!list->slots) {
        fprintf(stderr, "slotListCreate: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    list->count = 0;
    list->capacity = capacity;
    return list;
}

void slotListAdd(SlotList* list, Slot slot) {
    if (list->count == list->capacity) {
        int newCapacity = list->capacity == 0 ? 4 : list->capacity * 2;
        Slot* resized = (Slot*)realloc(list->slots, sizeof(Slot) * newCapacity);
        if (!resized) {
            fprintf(stderr, "slotListAdd: realloc failed\n");
            exit(EXIT_FAILURE);
        }
        list->slots = resized;
        list->capacity = newCapacity;
    }
    list->slots[list->count++] = slot;
}

void slotListFree(SlotList* list) {
    if (!list) return;
    free(list->slots);
    free(list);
}

static void scanLine(const Grid* grid, SlotList* list, int line,
                      Direction direction) {
    int runStart = -1;
 
    for (int pos = 0; pos <= GRID_SIZE; pos++) {
        int open = 0;
        if (pos < GRID_SIZE) {
            char cell = (direction == ACROSS) ? grid->cells[line][pos]
                                              : grid->cells[pos][line];
            open = (cell != BLACK_CELL);
        }
        /* pos == GRID_SIZE counts as "not open", ending the last run. */
 
        if (open) {
            if (runStart < 0) runStart = pos;
        } else if (runStart >= 0) {
            int length = pos - runStart;
            if (length >= MIN_SLOT_LENGTH) {
                Slot slot;
                slot.row = (direction == ACROSS) ? line : runStart;
                slot.col = (direction == ACROSS) ? runStart : line;
                slot.length = length;
                slot.direction = direction;
                slotListAdd(list, slot);
            }
            runStart = -1;
        }
    }
}
 
SlotList* slotListFromGrid(const Grid* grid) {
    SlotList* list = slotListCreate(16);
    for (int line = 0; line < GRID_SIZE; line++) {
        scanLine(grid, list, line, ACROSS);
    }
    for (int line = 0; line < GRID_SIZE; line++) {
        scanLine(grid, list, line, DOWN);
    }
    return list;
}