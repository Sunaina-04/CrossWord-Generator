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