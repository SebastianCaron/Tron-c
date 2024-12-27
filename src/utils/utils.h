#pragma once

#define BUFFER_SIZE 1024

/** 
∗ @struct grid
∗ @brief easy access to table information
*/
typedef struct{
    int nb_colonnes;
    int nb_lignes;
    int **grid;
} grid;

typedef enum {QUITTER, MENU_PRINCIPAL, MENU_SOLO, MENU_MULTI, NO_ACTION, PLAY_BOT_ALGO, PLAY_BOT_RANDOM, PLAY_BOT_Q, PLAY_MULTI, PLAY_ONLINE, RETOUR} actions;

/**
 * @brief Loads a map from a file and initializes a grid with the given dimensions
 * @param path The file path to load the map from
 * @param nb_colonnes The number of columns in the grid
 * @param nb_lignes The number of lines in the grid
 * @return A pointer to the loaded grid
 */
grid *load_map(char *path, int nb_colonnes, int nb_lignes);

/**
 * @brief Loads a grid from a file without altering its dimensions
 * @param path The file path to load the grid from
 * @param nb_lignes The number of lines in the grid
 * @param nb_colonnes The number of columns in the grid
 * @return A pointer to the loaded grid
 */
grid *load_grid_as_it_is(char *path, int nb_lignes, int nb_colonnes);

/**
 * @brief Counts the number of lines and columns in a file
 * @param path The file path to count lines and columns from
 * @param nb_lignes Pointer to store the number of lines
 * @param nb_colonnes Pointer to store the number of columns
 */
void count_nb_lignes_colonnes(char *path, int *nb_lignes, int *nb_colonnes);

/**
 * @brief Scales up a grid to the given dimensions
 * @param g Pointer to the grid to be upscaled
 * @param nb_lignes The new number of lines in the grid
 * @param nb_colonnes The new number of columns in the grid
 * @return A pointer to the upscaled grid
 */
grid *upscale_grid(grid *g, int nb_lignes, int nb_colonnes);

/**
 * @brief Initializes a grid with the given dimensions
 * @param nb_lignes The number of lines in the grid
 * @param nb_colonnes The number of columns in the grid
 * @return A pointer to the initialized grid
 */
grid *init_grid(int nb_lignes, int nb_colonnes);

/**
 * @brief Allocates memory for a grid with the given dimensions
 * @param nb_lignes The number of lines in the grid
 * @param nb_colonnes The number of columns in the grid
 * @return A pointer to the allocated grid
 */
int **allocate_grid(int nb_lignes, int nb_colonnes);

/**
 * @brief Frees the memory allocated for a grid
 * @param g Pointer to the grid to be destroyed
 */
void destroy_grid(grid *g);

/**
 * @brief Compares two strings for equality
 * @param a The first string to compare
 * @param b The second string to compare
 * @return 1 if the strings are equal, 0 otherwise
 */
int string_equal(char *a, char *b);

/**
 * @brief Checks if a string starts with a given pattern
 * @param a The string to check
 * @param pattern The pattern to look for
 * @return 1 if the string starts with the pattern, 0 otherwise
 */
int start_with(char *a, char *pattern);

/**
 * @brief Displays a grid
 * @param g Pointer to the grid to be displayed
 */
void display_grid(grid *g);

/**
 * @brief Displays a grid with specified dimensions
 * @param tab Pointer to the grid to be displayed
 * @param nb_lignes The number of lines in the grid
 * @param nb_colonnes The number of columns in the grid
 */
void display_grid_i(int **tab, int nb_lignes, int nb_colonnes);