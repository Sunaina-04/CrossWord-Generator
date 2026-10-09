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

SlotList* slotListFromGrid(const Grid* grid) {
    if (!grid) return NULL;

    SlotList* list = slotListCreate(16);
    if (!list) return NULL;

    // 1. Scan Horizontal (ACROSS)
    for (int r = 0; r < GRID_SIZE; r++) {
        int c = 0;
        while (c < GRID_SIZE) {
            if (grid->cells[r][c] == BLACK_CELL) {
                c++;
                continue;
            }

            int startCol = c;
            int length = 0;
            while (c < GRID_SIZE && grid->cells[r][c] != BLACK_CELL) {
                length++;
                c++;
            }

            if (length >= 2) {
                Slot slot = { r, startCol, length, ACROSS };
                slotListAdd(list, slot);
            }
        }
    }

    // 2. Scan Vertical (DOWN)
    for (int c = 0; c < GRID_SIZE; c++) {
        int r = 0;
        while (r < GRID_SIZE) {
            if (grid->cells[r][c] == BLACK_CELL) {
                r++;
                continue;
            }

            int startRow = r;
            int length = 0;
            while (r < GRID_SIZE && grid->cells[r][c] != BLACK_CELL) {
                length++;
                r++;
            }

            if (length >= 2) {
                Slot slot = { startRow, c, length, DOWN };
                slotListAdd(list, slot);
            }
        }
    }

    return list;
}

void slotListFree(SlotList* list) {
    if (!list) return;
    free(list->slots);
    free(list);
}