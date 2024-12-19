#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#define PI 3.14159265358979323846

#include "model.h"
#include "../utils/hashmap.h"
#include "../utils/QStack.h"

model *init_game(int nb_player, int nb_lignes_grid, int nb_colonnes_grid, int **grid){
    model *game = (model *) malloc(sizeof(model));
    if(!game){
        perror("[MODEL] erreur allocation de la structure model.");
        exit(EXIT_FAILURE);
    }

    game->players = (position**) malloc(sizeof(position*) * nb_player);
    if(game->players == NULL){
        perror("[MODEL] erreur allocation des positions pour les joueurs.");
        exit(EXIT_FAILURE);
    }

    game->directions = (direction *) calloc(nb_player, sizeof(direction));
    if(game->directions == NULL){
        perror("[MODEL] erreur allocation des directions pour les joueurs.");
        exit(EXIT_FAILURE);
    }

    game->dead = (char *) calloc(nb_player, sizeof(char));
    if(game->dead == NULL){
        perror("[MODEL] erreur allocation des etats de vie des joueurs.");
        exit(EXIT_FAILURE);
    }

    game->n_player= nb_player;
    game->n_player_alive = nb_player;

    game->grid = grid;
    game->nb_lignes_grid = nb_lignes_grid;
    game->nb_colonnes_grid = nb_colonnes_grid;

    game->scores = (int *) calloc(nb_player, sizeof(int));
    if(!game->scores){
        perror("[MODEL] erreur allocation d'une colonne de la grille de jeu.");
        exit(EXIT_FAILURE);
    }

    init_positions(game);
    init_directions(game);

    return game;
}

void destroy_model(model *m){
    if(!m) return;

    free(m->dead);
    for(int i = 0; i < m->n_player; i++){
        free(m->players[i]);
    }
    free(m->players);
    free(m->scores);
    free(m->directions);

    for(int i = 0; i < m->nb_lignes_grid; i++){
        free(m->grid[i]);
    }
    free(m->grid);

    free(m);
}

int move_player(model *m, int player, direction new_direction){
    if(!m){
        perror("[MODEL] deplacement joueur sur model NULL");
        exit(EXIT_FAILURE);
    }   
    if(player > m->n_player){
        perror("[MODEL] deplacement joueur d'un joueur inexistant.");
        exit(EXIT_FAILURE);
    }

    if(m->dead[player] != 0) return 0;  // Le joueur est mort, on ne le déplace pas.

    position *player_position = m->players[player];
    m->grid[player_position->y][player_position->x] = -(player+1); // Mur representé par -(joueur+1) 

    if((new_direction != NODIRECTION) && ((m->directions[player] & 1) != (new_direction & 1))){
        // printf("JOUEUR %d : DIRECTION : %d : NEW_DIR : %d\n", player, m->directions[player], new_direction);
        m->directions[player] = new_direction; // Change la direction si n'est pas opposé à la précèdente

    }

    switch (m->directions[player])
    {
        case UP:
            m->players[player]->y--;
            break;
        case DOWN:
            m->players[player]->y++;
            break;
        case LEFT:
            m->players[player]->x--;
            break;
        case RIGHT:
            m->players[player]->x++;
            break;
        default:
            break;
    }

    if(player_position->y >= 0 && player_position->y < m->nb_lignes_grid
        && player_position->x >= 0 && player_position->x < m->nb_colonnes_grid
        && m->grid[player_position->y][player_position->x] == EMPTY){

        m->grid[player_position->y][player_position->x] = (player+1); // Deplace le joueur si la case ou il doit être déplacé est vide
        m->scores[player] += SCORE_INCREMENT;
    }


    return 1;
}

// AJOUTER UN TEST AVEC LA COLLISION SUR LES BORDS DE LA FENETRE IE : y >= 0 && y < nb_lignes && x >= 0 && x < nb_colonnes
int collision_player(model *m, int indexPlayer){
    if(!m){
        perror("[MODEL] collision joueur sur model NULL");
        exit(EXIT_FAILURE);
    }  
    if(indexPlayer > m->n_player){
        perror("[MODEL] collision joueur d'un joueur inexistant.");
        exit(EXIT_FAILURE);
    }

    if(!m->dead[indexPlayer]){
        position *player = m->players[indexPlayer];
        if(player->y < 0 || player->y >= m->nb_lignes_grid || player->x < 0 || player->x >= m->nb_colonnes_grid || m->grid[player->y][player->x] != (indexPlayer+1)){
            // printf("PLAYER %d est moooort ! \n", indexPlayer);
            m->dead[indexPlayer] = 1;
            m->n_player_alive--;
            return 1;
        }
    }
    return 0;
}

int est_fini(model *m){
    if(!m){
        perror("[MODEL] est_fini sur model NULL");
        exit(EXIT_FAILURE);
    }  
    return (m->n_player_alive <= 1) ? 1 : 0;
}

int get_winner(model *m){
    for(int i =0; i<m->n_player;i++){
        if(!m->dead[i]) return i;
    }
    return -1;
}

position *create_position(int x, int y){
    position *p = calloc(1, sizeof(position));
    if(p == NULL){
        perror("[MODEL] ERREUR ALLOC Position");
        exit(EXIT_FAILURE);
    }
    p->x = x;
    p->y = y;
    return p;
}

int position_hash(void *v){
    position *p = v;
    return p->x * 100 + p->y;
}
int position_equal(void *a, void *b){
    position *v1 = a;
    position *v2 = b;

    return (v1->x == v2->x && v1->y == v2->y) ? 1 : 0;
}

position *get_nearest_point_available(model *m, int x, int y){
    position *pos_depart = create_position(x, y);
    // TODO
    hashmap *visited = init_hashmap(-1, position_equal, position_hash);
    queue *q = QS_init();
    while(q->size > 0){
        position *pos = QS_get_first(q);

        if(m->grid[pos->y][pos->x] == EMPTY){
            destroy_hashmap(visited);
            QS_destroy(q);
            return pos;
        }
        position *north, *south, *east, *west;
        north = create_position(pos->x, pos->y-1);
        south = create_position(pos->x, pos->y+1);
        east = create_position(pos->x+1, pos->y);
        west = create_position(pos->x-1, pos->y);
        if(!hashmap_is_in(visited, north)){
            hashmap_add(visited, north);
            QS_add(q, north);
        }
        if(!hashmap_is_in(visited, south)){
            hashmap_add(visited, south);
            QS_add(q, south);
        }
        if(!hashmap_is_in(visited, east)){
            hashmap_add(visited, east);
            QS_add(q, east);
        }
        if(!hashmap_is_in(visited, west)){
            hashmap_add(visited, west);
            QS_add(q, west);
        }

        free(pos);
    }
    destroy_hashmap(visited);
    QS_destroy(q);
    return pos_depart;
}

void init_positions(model *m){

    float marge = 5.0f;

    float centre_x = ((float) m->nb_colonnes_grid) / 2.0f;
    float centre_y = ((float) m->nb_lignes_grid) / 2.0f;

    float tmp_a = (((float) m->nb_lignes_grid) - 2 * marge) / 2.0f;
    float tmp_b = (((float) m->nb_colonnes_grid) - 2 * marge) / 2.0f;
    float radius = tmp_a;

    if(tmp_a > tmp_b) radius = tmp_b;

    float angle = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    for(int k = 0; k < m->n_player; k++){
        angle = 2 * PI * k / ((float)m->n_player);
        x = centre_x + radius * cos(angle);
        y = centre_y + radius * sin(angle);

        m->players[k] = get_nearest_point_available(m, round(x), round(y));
    }
}

void init_directions(model *m){
    for(int i = 0; i < m->n_player; i++){
        int d_x = (m->nb_colonnes_grid / 2) - m->players[i]->x;
        int d_y = (m->nb_lignes_grid / 2) - m->players[i]->y;
        if((d_x  < 0 ? -d_x : d_x) > (d_y  < 0 ? -d_y : d_y)){
            if(d_x > 0){
                m->directions[i] = RIGHT;
            }else{
                m->directions[i] = LEFT;
            }
        }else{
            if(d_y > 0){
                m->directions[i] = DOWN;
            }else{
                m->directions[i] = UP;
            }
        }
    }
}