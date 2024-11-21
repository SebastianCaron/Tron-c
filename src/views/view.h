#pragma once

#include "model/model.h"

typedef struct v{
    void (*update_screen)(int **grid);
    void (*update_change_screen)(int **grid);

    direction (*get_direction)();


    void (*destroy_self)(struct v *);
} view;

