#pragma once

#include "maillon.h"

#pragma once

#include "maillon.h"

/**
 * @struct _stack_queue
 * @brief Represents a queue or stack with a linked list structure
 */
typedef struct _stack_queue {
    maillon *start;  /**< Pointer to the first element in the stack/queue */
    maillon *end;    /**< Pointer to the last element in the stack/queue */
    int size;        /**< The number of elements in the stack/queue */
} stack;

typedef stack queue;

/**
 * @brief Initializes a new stack or queue
 * @return A pointer to the initialized stack/queue
 */
struct _stack_queue *QS_init();

/**
 * @brief Returns the first element in the queue/stack without removing it
 * @param q Pointer to the queue/stack
 * @return A pointer to the first element
 */
maillon *QS_show_first(struct _stack_queue *q);

/**
 * @brief Returns the last element in the queue/stack without removing it
 * @param q Pointer to the queue/stack
 * @return A pointer to the last element
 */
maillon *QS_show_last(struct _stack_queue *q);

/**
 * @brief Destroys the queue/stack and frees the allocated memory
 * @param q Pointer to the queue/stack to be destroyed
 */
void QS_destroy(struct _stack_queue *q);

/**
 * @brief Removes the first element from the queue/stack
 * @param q Pointer to the queue/stack
 */
void QS_pop_first(struct _stack_queue *q);

/**
 * @brief Removes the last element from the queue/stack
 * @param s Pointer to the queue/stack
 */
void QS_pop(struct _stack_queue *s);

/**
 * @brief Adds a new element to the end of the queue/stack
 * @param s Pointer to the queue/stack
 * @param x The x coordinate value of the new element
 * @param y The y coordinate value of the new element
 */
void QS_add(struct _stack_queue *s, int x, int y);

/**
 * @brief Adds a new element to the beginning of the queue/stack
 * @param s Pointer to the queue/stack
 * @param x The x coordinate value of the new element
 * @param y The y coordinate value of the new element
 */
void QS_add_first(struct _stack_queue *s, int x, int y);
