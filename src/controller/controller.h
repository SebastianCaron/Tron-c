#pragma once

/**
 * @file controller.h
 * @date 2024-12-27
 * @brief Header file for the game controller.
 * 
 * This file contains the declarations of structures and functions 
 * related to the game controller, which manages the game model and views, 
 * handles player input, and coordinates game activities.
 * 
 * @authors
 * Pierre Magieu
 * Sebastian Caron
 * Lina Lethoor
 */

#include <stdarg.h>

#include "../model/model.h"
#include "../views/view.h"
#include "../utils/utils.h"
#include "../network/network.h"

#define SPEED_FRM 100000

/**
 * @struct controller
 * @brief Represents the game controller which manages the game model and views
 */
typedef struct {
    model *m;          /**< Pointer to the game model */
    unsigned nb_view;  /**< Number of views */
    view **views;      /**< Array of pointers to views */

    char *ip;          /**< IP address for online play */
    int port;          /**< Port number for online play */
    char marker;       /**< Marker character for the controller */
    server *s;         /**< A pointer to an initialized server */

    char *map;         /**< Path to the map that will be used as grid */
    int nb_bots;       /**< Number of bots against algorithms */
} controller;

/**
 * @brief Initializes a controller with a variable number of views
 * @param nb_view The number of views to initialize
 * @param ... The views to be included in the controller
 * @return A pointer to the initialized controller
 */
controller *init_controller(int nb_view, ...);

/**
 * @brief Add a view to the controller
 * @param c A pointer to the controller
 * @param v The view that will be added
 */
void add_view(controller *c, view *v);

/**
 * @brief Creates the game model within the controller
 * @param c Pointer to the controller
 * @param nb_player Number of players in the game
 */
void create_model(controller *c, int nb_player);

/**
 * @brief Starts a solo game against bots
 * @param c Pointer to the controller
 * @param get_dir_bot Function pointer to get the direction for the bot
 * @param nb_bots Number of bots in the game
 */
void controller_play_solo_j_vs_bot(controller *c, direction (*get_dir_bot)(int, int, int **, position **, direction *, int), int nb_bots);

/**
 * @brief Frees the memory allocated for the controller
 * @param c Pointer to the controller to be destroyed
 */
void destroy_controller(controller *c);

/**
 * @brief Navigates to the menu
 * @param c Pointer to the controller
 */
void go_to_menu(controller *c);

/**
 * @brief Displays the winner of the game
 * @param c Pointer to the controller
 */
void display_winner(controller *c);

/**
 * @brief Sets the IP address for the controller
 * @param c Pointer to the controller
 * @param ip The IP address to be set
 */
void set_ip(controller *c, char *ip);

/**
 * @brief Sets the port number for the controller
 * @param c Pointer to the controller
 * @param port The port number to be set
 */
void set_port(controller *c, char *port);

/**
 * @brief Joins an online game
 * @param c Pointer to the controller
 */
void controller_play_online_join(controller *c);

/**
 * @brief Hosts an online game
 * @param c Pointer to the controller
 */
void controller_play_online_host(controller *c);

/**
 * @brief Sets the map for the controller
 * @param c Pointer to the controller
 * @param map The path to the map
 */
void set_map(controller *c, char *map);

/**
 * @brief Sets the number of bots for the controller
 * @param c Pointer to the controller
 * @param nb_bots The number of bots
 */
void set_nb_bots(controller *c, char *nb_bots);

/**
 * @brief This function is used to train the QLearning Bot
 * @param c Pointer to the controller
 * @param get_dir_bot Function pointer to get the direction for the bot
 * @param get_dir_bot_q Function pointer to get the direction for the bot for q_learning
 * @param nb_bots Number of bots in the game
 */
void controller_play_train_vs_bot(controller *c, direction (*get_dir_bot)(int, int, int **, position **, direction *, int), direction (*get_dir_bot_q)(int, int, int **, position **, direction *, int), int nb_bots);