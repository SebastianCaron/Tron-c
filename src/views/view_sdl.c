#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "view_sdl.h"

view *init_view_sdl(){
    view *v = (view *)malloc(sizeof(view));
    if(!v){
        perror("[VIEW SDL] erreur allocation de la structure view sdl.");
        exit(EXIT_FAILURE);
    }


    v->destroy_self = destroy_view_sdl;
    v->get_direction = get_direction_sdl;
    // v->update_change_screen = ;
    v->update_screen = update_screen_sdl;

    return v;
}


void destroy_view_sdl(view *v, SDL_Renderer *renderer, SDL_Window *window){
    if (NULL != renderer) SDL_DestroyRenderer(renderer);
    if (NULL != window) SDL_DestroyWindow(window);
    SDL_Quit();
    free(v);
}

direction get_direction_sdl(){    

}

void update_screen_sdl(int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes, SDL_Renderer *renderer){
    if(!renderer){
        perror("[VIEW SDL] Renderer invalide.");
        exit(EXIT_FAILURE);
    }

    int pixelW =  LARGEUR / nb_colonnes;
    int pixelH= HAUTEUR / nb_lignes;

    for(int i = 0; i<nb_lignes;i++){
        for (int j= 0 ;j <nb_colonnes;j++){

            SDL_Rect pixel;
            pixel.h = pixelH;
            pixel.w= pixelW;
            pixel.x = j*pixelH;
            pixel.y = j*pixelW;

            switch (grid[nb_lignes][nb_colonnes])
            {
            case 0:
                // Couleur mur
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                break;
            case 1:
                // Couleur j1
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                break;
            case -1:
                // Couleur mur j1
                SDL_SetRenderDrawColor(renderer, 240, 0, 0, 255);
                break;
            case 2:
                // Couleur j2
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                break;
            case -2:
                // Couleur mur j2
                SDL_SetRenderDrawColor(renderer, 0, 240, 0, 255);
                break;
            case 3:
                // Couleur j3
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                break;
            case -3:
                // Couleur mur j3
                SDL_SetRenderDrawColor(renderer, 0, 0, 240, 255);
                break;
            case 4:
                // Couleur j4
                SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255);
                break;
            case -4:
                // Couleur mur j4
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                break;
            default:
                break;
            }

            SDL_RenderFillRect(renderer, &pixel);
        }
    }

    // Afficher les scores 


    SDL_RenderPresent(renderer);
}