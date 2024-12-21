#pragma once

#define PORT 7604
#define MAX_CLIENT 7

typedef enum {NBJOUEUR = 1, GRID = 2, POSITIONS = 3, SCORES = 4, MOUVEMENT = 5, ISOVER = 6, START = 7, NEEDGRID = 8, WINNER = 9, IDSERV = 10, READY, ENDPACKET} f_packet;