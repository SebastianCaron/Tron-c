#pragma once

/**
 * @file maillon.h
 * @date 2024-12-27
 * @brief Header file for linked list nodes (maillon).
 * 
 * This file contains the declarations of structures and functions 
 * related to linked list nodes (maillon), used in stack and queue 
 * implementations.
 * 
 * @authors
 * Pierre Magieu
 * Sebastian Caron
 * Lina Lethoor
 */

/**
 * @struct _maillon
 * @brief Represents a node in a doubly linked list, storing coordinates and pointers to the next and previous nodes
 */
typedef struct _maillon {
    int x;                  /**< The x coordinate value */
    int y;                  /**< The y coordinate value */
    struct _maillon *next;  /**< Pointer to the next node in the list */
    struct _maillon *prev;  /**< Pointer to the previous node in the list */
} maillon;

/**
 * @brief Initializes a new maillon with the specified coordinates
 * @param x The x coordinate value
 * @param y The y coordinate value
 * @return A pointer to the initialized maillon
 */
maillon *init_maillon(int x, int y);

/**
 * @brief Frees the memory allocated for a maillon and all subsequent maillons in the list
 * @param m Pointer to the maillon to be destroyed
 */
void destroy_maillons(maillon *m);

/**
 * @brief Removes a maillon with the specified coordinates from the list
 * @param m Pointer to the head of the list
 * @param x The x coordinate value of the maillon to be removed
 * @param y The y coordinate value of the maillon to be removed
 * @return A pointer to the modified list
 */
maillon *maillon_remove(maillon *m, int x, int y);

/**
 * @brief Checks if a maillon with the specified coordinates exists in the list
 * @param m Pointer to the head of the list
 * @param x The x coordinate value to check
 * @param y The y coordinate value to check
 * @return 1 if the maillon exists in the list, 0 otherwise
 */
int maillon_is_in(maillon *m, int x, int y);