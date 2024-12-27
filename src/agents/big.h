#pragma once

#include "../model/model.h"

direction espace_vital_get_direction(int nb_lignes, int nb_colonnes, int **grid, position **players, direction *directions, int index_bot);
direction immitateur_get_direction(int nb_lignes, int nb_colonnes, int **grid, position **players, direction *directions, int index_bot);