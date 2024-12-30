#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "../model/model.h"
#include "q.h"


float Q[MAX_STATES][3];  // PAS BOUGER, ALLER A DROITE ET GAUCHE en relatif

int dx[] = {0, 0, -1, 1};  // LEFT, RIGHT
int dy[] = {-1, 1, 0, 0};  // UP, DOWN

float epsilon = EPSILON;
int episode_count = 0;

void init_Q() {
    for (int i = 0; i < MAX_STATES; i++) {
        for (int j = 0; j < 3; j++) {
            Q[i][j] = 0.0f;
        }
    }
}

void save_Q_table(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(Q, sizeof(Q), 1, f);
        fclose(f);
    }
}

int load_Q_table(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (f) {
        fread(Q, sizeof(Q), 1, f);
        fclose(f);
        return 1;
    }
    return 0;
}

void reset_bot_q(){
    epsilon = EPSILON;
    episode_count = 0;
}

int get_state_index(int nb_lignes, int nb_colonnes, int **grid, position bot_position) {
    if (bot_position.x < 0 || bot_position.x >= nb_colonnes || 
        bot_position.y < 0 || bot_position.y >= nb_lignes) {
        return -1; 
    }

    int index = bot_position.y * nb_colonnes + bot_position.x;

    int v[4] = {
        (bot_position.y > 0) ? grid[bot_position.y - 1][bot_position.x] : WALL,
        (bot_position.y < nb_lignes - 1) ? grid[bot_position.y + 1][bot_position.x] : WALL,
        (bot_position.x > 0) ? grid[bot_position.y][bot_position.x - 1] : WALL,
        (bot_position.x < nb_colonnes - 1) ? grid[bot_position.y][bot_position.x + 1] : WALL,
    };

    for (int i = 0; i < 4; i++) {
        index = index * 10 + v[i];
    }

    // if(index > MAX_STATES) printf("INDEX %d out : %d / %d \n", index, index, MAX_STATES);
    return index % MAX_STATES;
}

direction get_new_direction(direction current_direction, int relative_action) {
    switch (current_direction) {
        case UP:
            return (relative_action == 0) ? UP : (relative_action == 1 ? LEFT : RIGHT);
        case DOWN:
            return (relative_action == 0) ? DOWN : (relative_action == 1 ? RIGHT : LEFT);
        case LEFT:
            return (relative_action == 0) ? LEFT : (relative_action == 1 ? DOWN : UP);
        case RIGHT:
            return (relative_action == 0) ? RIGHT : (relative_action == 1 ? UP : DOWN);
        default:
            return NODIRECTION;
    }
}

int choose_action(int state, int episode_count) {
    float effective_epsilon = epsilon / (1 + episode_count / 1000);
    if ((float)rand() / RAND_MAX < effective_epsilon) {
        return rand() % 3;
    } else {
        int best_action = 0;
        float max_value = Q[state][0];
        for (int a = 1; a < 3; a++) {
            if (Q[state][a] > max_value) {
                max_value = Q[state][a];
                best_action = a;
            }
        }
        return best_action;
    }
}

int calculate_reward(int nb_lignes, int nb_colonnes, int **grid, int x, int y) {
    if (x < 0 || x >= nb_colonnes || y < 0 || y >= nb_lignes || grid[y][x] != EMPTY) {
        return PENALTY_COLLISION;
    }

    int free_neighbors = 0;
    if (y > 0 && grid[y - 1][x] == EMPTY) free_neighbors++;
    if (y < nb_lignes - 1 && grid[y + 1][x] == EMPTY) free_neighbors++;
    if (x > 0 && grid[y][x - 1] == EMPTY) free_neighbors++;
    if (x < nb_colonnes - 1 && grid[y][x + 1] == EMPTY) free_neighbors++;

    return SCORE_INCREMENT + free_neighbors * 2;
}

// https://fr.wikipedia.org/wiki/Q-learning
void update_Q(int state, int action, int reward, int next_state) {
    float max_next_q = Q[next_state][0];
    for (int a = 1; a < 3; a++) {
        if (Q[next_state][a] > max_next_q) {
            max_next_q = Q[next_state][a];
        }
    }

    float update = ALPHA * (reward + GAMMA * max_next_q - Q[state][action]);
    Q[state][action] += update;
    // Q[state][action] = (1 - ALPHA) * Q[state][action] + ALPHA * (reward + GAMMA * max_next_q);

    if (Q[state][action] > 1e6) Q[state][action] = 1e6;
    if (Q[state][action] < -1e6) Q[state][action] = -1e6;
}


// float update_epsilon(int episode, int max_episodes) {
//     float initial_epsilon = 0.8;  // Valeur initiale élevée pour favoriser l'exploration
//     float final_epsilon = 0.4;  // Valeur minimale
//     epsilon = final_epsilon + (initial_epsilon - final_epsilon) * exp(-5.0 * episode / max_episodes);
//     // return final_epsilon + (initial_epsilon - final_epsilon) * exp(-10.0 * episode / max_episodes);
//     return epsilon;
// }

direction q_learning_bot(int nb_lignes, int nb_colonnes, int **grid, position **players, direction *directions, int index_bot) { 
    position bot_position = *(players[index_bot]);
    direction current_direction = directions[index_bot];
    int current_state = get_state_index(nb_lignes, nb_colonnes, grid, bot_position);

    if (current_state < 0) return NODIRECTION;

    int relative_action = choose_action(current_state, episode_count);
    direction new_direction = get_new_direction(current_direction, relative_action);


    // Vraiment pas sûr du reste
    int nd_index = new_direction - 1;
    // printf("NEW DIR : %d\n", nd_index);
    int x = 0;
    int y = 0;
    if(nd_index < 0){
        // x = bot_position.x;
        // y = bot_position.y;
        return NODIRECTION;
    }else{
        x = bot_position.x + dx[nd_index];
        y = bot_position.y + dy[nd_index];
    }

    int reward = calculate_reward(nb_lignes, nb_colonnes, grid, x, y);
    if (reward == PENALTY_COLLISION) {
        update_Q(current_state, relative_action, reward, current_state);
        return NODIRECTION;
    } else {
        int next_state = get_state_index(nb_lignes, nb_colonnes, grid, (position){x, y});
        update_Q(current_state, relative_action, reward, next_state);
        episode_count++;
        return new_direction;
    }
}
