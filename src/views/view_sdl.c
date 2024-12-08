#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"  //Il faut installer libsdl2-ttf-dev   //=>sudo apt-get install libsdl2-ttf-dev


#include "../controller/controller.h"
#include "view.h"
#include "view_sdl.h"

SDL_Color tabColors[10] = { {0,0,0,255}, {255,255,0,255}, {255,0,255,255}, //Sol, J1, J2
                            {0,255,255,255}, {255,0,0,255}, {0,255,0,255}, //J3, J4, J5
                            {0,0,255,255}, {0,140,140,255}, {140,0,0,255}, //J6, J7, J8
                            {255,255,255,255}}; // Mur de la map

view *init_view_sdl(){
    view *v = (view *)malloc(sizeof(view));
    if(!v){
        perror("[VIEW SDL] erreur allocation de la structure view.");
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "[VIEW SDL] ERREUR SDL_ttf - %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    v->destroy_self = destroy_view_sdl;
    v->get_direction = get_direction_sdl;
    // v->update_change_screen = ;
    v->update_screen = update_screen_sdl;

    v->affiche_menu_principal = afficheMenuPrincipalSDL;
    v->affiche_menu_multijoueur = afficheMenuMultiplayerSDL;
    v->affiche_menu_solo = afficheMenuSoloSDL;


    v->type = 's';

    view_sdl *viewSdl =(view_sdl *)malloc(sizeof(view_sdl));
    if(!viewSdl){
        perror("[VIEW SDL] erreur allocation de la structure view_Sdl.");
        exit(EXIT_FAILURE);
    }
    v->sdl = viewSdl; 

    v->sdl->renderer = NULL;
    v->sdl->window = NULL;

    v->sdl->window = SDL_CreateWindow("TRONC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, HAUTEUR, LARGEUR, SDL_WINDOW_SHOWN);
    if (v->sdl->window == NULL) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        quitter(v->sdl->window , v->sdl->renderer);
    }

    v->sdl->renderer = SDL_CreateRenderer(v->sdl->window, -1, SDL_RENDERER_ACCELERATED);
    if (v->sdl->renderer == NULL) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        quitter(v->sdl->window, v->sdl->renderer);
    }
    return v;
}


void destroy_view_sdl(view *v){
    if(v == NULL) return;
    if(v->sdl != NULL){
        SDL_DestroyRenderer(v->sdl->renderer);
        SDL_DestroyWindow(v->sdl->window);
    }
    TTF_Quit();
    SDL_Quit();
    free(v->sdl);
    free(v);
}

void quitter(SDL_Window *window, SDL_Renderer *renderer){
    if(renderer != NULL) SDL_DestroyRenderer(renderer);
    if(window != NULL) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}


// J'ai mis l'ecoute d'évenement pour quitter ici parce que je sais pas ou le mettre sinon a part dans le main
direction get_direction_sdl(view *v){    
    SDL_Event event;
    while (SDL_WaitEvent(&event) > 0){
        if (event.type == SDL_QUIT){
            quitter(v->sdl->window, v->sdl->renderer);
        }else if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_UP :
                    return UP;
                case SDLK_DOWN:
                    return DOWN;
                case SDLK_LEFT :
                    return LEFT;
                case SDLK_RIGHT:
                    return RIGHT;
                default:
                    return NODIRECTION;
            }
        }
    }
    return NODIRECTION;
}


SDL_Rect *createRect(int h, int w, int x, int y){
    SDL_Rect *ret = calloc(1, sizeof(SDL_Rect));
    ret->h = h;
    ret->w = w;
    ret->x = x;
    ret->y =y;

    return ret;
}

void afficheTexte(SDL_Renderer *renderer,char *texte, int x, int y) {
    TTF_Font *font = TTF_OpenFont("./res/arial.ttf", 24);//faut voir la taille
    if (font == NULL) {
        fprintf(stderr, "Erreur lors du chargement de la police : %s\n", TTF_GetError());
    }
    SDL_Color color = {255, 255, 255, 255}; 
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(font, texte, color);
    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);

    SDL_Rect destRect = (SDL_Rect) {x, y, surfaceTexte->w, surfaceTexte->h};
    SDL_RenderCopy(renderer, textureTexte, NULL, &destRect);

    SDL_DestroyTexture(textureTexte);
    SDL_FreeSurface(surfaceTexte);
    TTF_CloseFont(font);
}


void afficheMenuPrincipalSDL(view *v, actions *act){
    SDL_Renderer *renderer = v->sdl->renderer;
    v->get_action = get_action_menu_principal_sdl;
    // Affichage Titre
    afficheTexte(renderer, "Tron", LARGEUR/2, HAUTEUR/3);

    // Rectangle Solo
    SDL_Rect *solo = createRect(50, 100, LARGEUR/2, 2*(HAUTEUR/3));
    SDL_SetRenderDrawColor(renderer, 237, 237, 148, 255);
    SDL_RenderDrawRect(renderer, solo);
    afficheTexte(renderer, "solo", LARGEUR/2,HAUTEUR/2);
    // Le dessiner et mettre le titre dedans

    // Rectangle Multiplayer
    SDL_Rect *multiplayer = createRect(50, 100, LARGEUR/2,2.5*(HAUTEUR/3));//Les positions x et y c'est du test la pour le coup 
    SDL_SetRenderDrawColor(renderer, 237, 237, 148, 255);
    SDL_RenderDrawRect(renderer, multiplayer);
    afficheTexte(renderer, "multiplayer", LARGEUR/2,2.5*(HAUTEUR/3));
    // Le dessiner et mettre le titre dedans

    // Ajoute les boutons dans la structure pour detecter les actions
    v->sdl->nb_buttons = 2;
    v->sdl->buttons[0] = solo;
    v->sdl->buttons[1] = multiplayer;

    SDL_RenderPresent(renderer);

}

void get_action_menu_principal_sdl(view *v, actions *act, int *selected_option) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *act = QUITTER;
                return;

            case SDL_MOUSEBUTTONDOWN: {
                int x = event.button.x;
                int y = event.button.y;
                // printf("X : %d, Y : %d\n", x, y);
                for (unsigned i = 0; i < v->sdl->nb_buttons; i++) {
                    if (x >= v->sdl->buttons[i]->x &&
                        x <= (v->sdl->buttons[i]->x + v->sdl->buttons[i]->w) &&
                        y >= v->sdl->buttons[i]->y &&
                        y <= (v->sdl->buttons[i]->y + v->sdl->buttons[i]->h)) {
                        switch (i) {
                            case 0:
                                *act = MENU_SOLO;
                                return;
                            case 1:
                                *act = MENU_MULTI;
                                return;
                            default:
                                break;
                        }
                    }
                }
                break;
            }

            default:
                break;
        }
    }
}



void afficheMenuSoloSDL(view *v, actions *act){
    SDL_Renderer *renderer = v->sdl->renderer;
    v->get_action = get_action_menu_solo_sdl;
    afficheTexte(renderer, "Solo",LARGEUR/2,(HAUTEUR/3));

    // Rectangle Vs algo  
    SDL_Rect *algo = createRect(50, 100, LARGEUR/2,2*(HAUTEUR/3));
    SDL_SetRenderDrawColor(renderer, 237, 237, 148, 255);
    SDL_RenderDrawRect(renderer, algo);
    afficheTexte(renderer, "vs algo", LARGEUR/2,2*(HAUTEUR/3));

    // Rectangle Vs Q-learning
    SDL_Rect *q = createRect(50, 100, LARGEUR/2,2.5*(HAUTEUR/3));
    SDL_SetRenderDrawColor(renderer, 237, 237, 148, 255);
    SDL_RenderDrawRect(renderer, q);
    afficheTexte(renderer, "vs q-learning",LARGEUR/2,2.5*(HAUTEUR/3));
    
    v->sdl->nb_buttons = 2;
    v->sdl->buttons[0] = algo;
    v->sdl->buttons[1] = q;

    SDL_RenderPresent(renderer);

}

void get_action_menu_solo_sdl(view *v, actions *act, int *selected_option) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *act = QUITTER;
                return;

            case SDL_MOUSEBUTTONDOWN: {
                int x = event.button.x;
                int y = event.button.y;
                // printf("X : %d, Y : %d\n", x, y);
                for (unsigned i = 0; i < v->sdl->nb_buttons; i++) {
                    if (x >= v->sdl->buttons[i]->x &&
                        x <= (v->sdl->buttons[i]->x + v->sdl->buttons[i]->w) &&
                        y >= v->sdl->buttons[i]->y &&
                        y <= (v->sdl->buttons[i]->y + v->sdl->buttons[i]->h)) {
                        switch (i) {
                            case 0:
                                *act = PLAY_BOT_ALGO;
                                return;
                            case 1:
                                *act = PLAY_BOT_Q;
                                return;
                            default:
                                break;
                        }
                    }
                }
                break;
            }

            default:
                break;
        }
    }
}


void afficheMenuMultiplayerSDL(view *v, actions *act){
    SDL_Renderer *renderer = v->sdl->renderer;
    v->get_action = get_action_menu_multi_sdl;
    // Affichage Titre
    afficheTexte(renderer, "Multiplayer", LARGEUR/2,(HAUTEUR/3));

    // Rectangle On this machine
    SDL_Rect *machine = createRect(50, 100, LARGEUR/2,2*(HAUTEUR/3));
    SDL_SetRenderDrawColor(renderer, 237, 237, 148, 255);
    SDL_RenderDrawRect(renderer, machine);
    afficheTexte(renderer, "on this machine (2 players)", LARGEUR/2,2*(HAUTEUR/3));
    // Le dessiner et mettre le titre dedans

    // Rectangle with other
    SDL_Rect *others = createRect(50, 100, LARGEUR/2,2.5*(HAUTEUR/3));
    SDL_SetRenderDrawColor(renderer, 237, 237, 148, 255);
    SDL_RenderDrawRect(renderer, others);
    afficheTexte(renderer, "with others", LARGEUR/2,2.5*(HAUTEUR/3));
    // Le dessiner et mettre le titre dedans

    SDL_RenderPresent(renderer);

}

void get_action_menu_multi_sdl(view *v, actions *act, int *selected_option) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *act = QUITTER;
                return;

            case SDL_MOUSEBUTTONDOWN: {
                int x = event.button.x;
                int y = event.button.y;
                // printf("X : %d, Y : %d\n", x, y);
                for (unsigned i = 0; i < v->sdl->nb_buttons; i++) {
                    if (x >= v->sdl->buttons[i]->x &&
                        x <= (v->sdl->buttons[i]->x + v->sdl->buttons[i]->w) &&
                        y >= v->sdl->buttons[i]->y &&
                        y <= (v->sdl->buttons[i]->y + v->sdl->buttons[i]->h)) {
                        switch (i) {
                            case 0:
                                *act = PLAY_MULTI;
                                return;
                            case 1:
                                *act = PLAY_ONLINE;
                                return;
                            default:
                                break;
                        }
                    }
                }
                break;
            }

            default:
                break;
        }
    }
}


void update_screen_sdl(view *v, int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes){
    if(v == NULL){
        perror("[VIEW SDL] V is null.");
        exit(EXIT_FAILURE);
    }
    
    if(v->sdl == NULL){
        perror("[VIEW SDL] V is not SDL.");
        exit(EXIT_FAILURE);
    }

    if(v->sdl->renderer == NULL){
        perror("[VIEW SDL] No renderer available in the view.");
        exit(EXIT_FAILURE);
    }

    SDL_Renderer *renderer = v->sdl->renderer;

    for(int i = 0; i<nb_lignes;i++){
        for (int j= 0 ;j <nb_colonnes;j++){

            SDL_Rect pixel;
            pixel.h = HPIXEL;
            pixel.w= WPIXEL;
            pixel.x = j*WPIXEL;
            pixel.y = i*HPIXEL;

            if(grid[i][j]=='#'){
                SDL_SetRenderDrawColor(renderer, tabColors[9].r, tabColors[9].g, tabColors[9].b, tabColors[9].a);
            }
            else{
                int indice = grid[i][j];
                if (indice < 0){
                    SDL_SetRenderDrawColor(renderer, tabColors[indice*-1].r, tabColors[indice*-1].g, tabColors[indice*-1].b, tabColors[indice*-1].a-40);
                }else{
                    SDL_SetRenderDrawColor(renderer, tabColors[indice].r, tabColors[indice].g, tabColors[indice].b, tabColors[indice].a);
                }
            }
            SDL_RenderFillRect(renderer, &pixel);
        }
    }

    // Afficher les scores 
    TTF_Font *font = TTF_OpenFont("arial.ttf", 10);//faut voir la taille
    SDL_Color color = {255, 255, 255, 255}; 

    int decalageY = 10;

    for (int i = 0; i < nb_player; i++) {
        char score[20];
        snprintf(score, sizeof(score), "Player %d: %d", i + 1, scores[i]);
        afficheTexte(renderer, score, 10, decalageY);
        decalageY += 10;
    }
    SDL_RenderPresent(renderer);
}