#pragma once

// #include <SDL2/SDL.h>
#include "../model/model.h"
#include "../views/view_sdl.h"
#include "../views/view_ncurse.h"
#include "../views/view.h"

void init_controller(view *);

void controllerPlay(model *, view *);

void destroy_controller(model *, view *);



