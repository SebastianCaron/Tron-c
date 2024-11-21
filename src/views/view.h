#pragma once

#include "model/model.h"

typedef struct{
    void (*update_screen)(int **grid);
    void (*update_change_screen)(int **grid);

    direction (*get_direction)();
} view;

