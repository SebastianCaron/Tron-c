#pragma once

#include "maillon.h"

#define DEFAULT_SIZE_HASH_MAP 1024
/**
 * @struct hashmap
 * @brief Represents a hashmap that uses a linked list for handling 2D coordinates
 */
typedef struct hashmap {
    maillon **tab;            /**< Array of pointers to linked lists */
    int size;                 /**< Size of the hashmap  */
    int (*hash)(int x, int y); /**< Function pointer to the hash function */
} hashmap;

/**
 * @brief Checks if a coordinate (x, y) is in the hashmap
 * @param h Pointer to the hashmap
 * @param x The x coordinate
 * @param y The y coordinate
 * @return 1 if the coordinate is in the hashmap, 0 otherwise
 */
int hashmap_is_in(hashmap *h, int x, int y);

/**
 * @brief Adds a coordinate (x, y) to the hashmap
 * @param h Pointer to the hashmap
 * @param x The x coordinate
 * @param y The y coordinate
 * @return 1 on success, 0 otherwise
 */
int hashmap_add(hashmap *h, int x, int y);

/**
 * @brief Removes a coordinate (x, y) from the hashmap
 * @param h Pointer to the hashmap
 * @param x The x coordinate
 * @param y The y coordinate
 */
void hashmap_remove(hashmap *h, int x, int y);

/**
 * @brief Initializes a hashmap with a given size and hash function
 * @param size The size of the hashmap
 * @param hash Function pointer to the hash function
 * @return A pointer to the initialized hashmap
 */
hashmap *init_hashmap(int size, int (*hash)(int x, int y));

/**
 * @brief Destroys the hashmap and frees allocated memory
 * @param h Pointer to the hashmap to be destroyed
 */
void destroy_hashmap(hashmap *h);