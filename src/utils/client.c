#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#include "client.h"
#include "utils.h"

client *init_client(char *ip, int port){
    
}
void destroy_client(client *c){

}

int **get_grid(client *c);
int is_over(client *c);
int *get_scores(client *c);
position **get_positions(client *c);
int get_nb_player(client *c);
int get_start_signal(client *c);
char **get_names(client *c);
int *get_winners(client *c);


void send_movement(client *s, direction d);
void ask_for_grid(client *s);
