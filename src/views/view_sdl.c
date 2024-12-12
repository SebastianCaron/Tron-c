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

    v->affiche_menu = affiche_menu_sdl;

    // v->affiche_menu_principal = afficheMenuPrincipalSDL;
    // v->affiche_menu_multijoueur = afficheMenuMultiplayerSDL;
    // v->affiche_menu_solo = afficheMenuSoloSDL;


    v->type = 's';
    v->width = 30;
    v->height = 30;
    v->nbMenu = 0;

    view_sdl *viewSdl =(view_sdl *)malloc(sizeof(view_sdl));
    if(!viewSdl){
        perror("[VIEW SDL] erreur allocation de la structure view_Sdl.");
        exit(EXIT_FAILURE);
    }
    v->sdl = viewSdl; 
    v->sdl->menu_current = 1000;

    v->sdl->renderer = NULL;
    v->sdl->window = NULL;

    v->sdl->window = SDL_CreateWindow("TRONC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGEUR, HAUTEUR, SDL_WINDOW_SHOWN);
    if (v->sdl->window == NULL) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        quitter(v->sdl->window , v->sdl->renderer);
    }

    v->sdl->renderer = SDL_CreateRenderer(v->sdl->window, -1, SDL_RENDERER_ACCELERATED);
    if (v->sdl->renderer == NULL) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        quitter(v->sdl->window, v->sdl->renderer);
    }
    
    SDL_SetRenderDrawBlendMode(v->sdl->renderer, SDL_BLENDMODE_BLEND);
    return v;
}


void destroy_view_sdl(view *v){
    if(v == NULL) return;
    if(v->sdl){
        SDL_DestroyRenderer(v->sdl->renderer);
        SDL_DestroyWindow(v->sdl->window);
    }
    TTF_Quit();
    SDL_Quit();
    free_buttons(v->sdl);
    free(v->sdl);
    free(v);
}


void free_buttons(view_sdl *sdl) {
    for (int i = 0; i < sdl->nb_buttons; i++) {
        if (sdl->buttons[i] != NULL) {
            free(sdl->buttons[i]);
            sdl->buttons[i] = NULL;
        }
    }
    sdl->nb_buttons = 0;
}


void quitter(SDL_Window *window, SDL_Renderer *renderer){
    if(renderer != NULL) SDL_DestroyRenderer(renderer);
    if(window != NULL) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}


// J'ai mis l'ecoute d'évenement pour quitter ici parce que je sais pas ou le mettre sinon a part dans le main
direction get_direction_sdl(view *v, int nb_player_on_keyboard, direction *dirs){    
    SDL_Event event;
    while (SDL_PollEvent(&event) > 0){
        if (event.type == SDL_QUIT){
            quitter(v->sdl->window, v->sdl->renderer);
        }else if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_UP:
                    if((dirs[0]) != DOWN) (dirs[0]) = UP;
                    return UP;
                case SDLK_DOWN:
                    if((dirs[0]) != UP) (dirs[0]) = DOWN;
                    return DOWN;
                case SDLK_LEFT :
                    if((dirs[0]) != RIGHT) (dirs[0]) = LEFT;
                    return LEFT;
                case SDLK_RIGHT:
                    if((dirs[0]) != LEFT) (dirs[0]) = RIGHT;
                    return RIGHT;
                case SDLK_z:
                    if((dirs[nb_player_on_keyboard-1]) != DOWN) (dirs[nb_player_on_keyboard-1]) = UP;
                    return UP;
                case SDLK_q:
                    if((dirs[nb_player_on_keyboard-1]) != RIGHT) (dirs[nb_player_on_keyboard-1]) = LEFT;
                    return LEFT;
                case SDLK_s:
                    if((dirs[nb_player_on_keyboard-1]) != UP) (dirs[nb_player_on_keyboard-1]) = DOWN;
                    return DOWN;
                case SDLK_d:
                    if((dirs[nb_player_on_keyboard-1]) != LEFT) (dirs[nb_player_on_keyboard-1]) = RIGHT;
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

SDL_Rect afficheButton(SDL_Renderer *renderer,char *texte, int y, int titre) {
    TTF_Font *font = TTF_OpenFont("./res/game.ttf", 40);
    SDL_Color color = {0, 0, 0, 255}; 
    if(titre==1){
        font = TTF_OpenFont("./res/game.ttf", 50);
        color = (SDL_Color) {255, 255, 255, 255};
    }
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(font, texte, color);
    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);

    SDL_Rect destRect = (SDL_Rect) {LARGEUR/2-surfaceTexte->w/2, y, surfaceTexte->w, surfaceTexte->h};
    SDL_RenderFillRect(renderer, &destRect);
    SDL_RenderCopy(renderer, textureTexte, NULL, &destRect);
    

    SDL_DestroyTexture(textureTexte);
    SDL_FreeSurface(surfaceTexte);
    TTF_CloseFont(font);

    return destRect;   
}

void afficheScore(SDL_Renderer *renderer, int nbPlayer, int *scores) {

    int posX[8] = {10, 200, 400, 600,10, 100, 200, 300};
    int posY[8] = {5, 5, 5, 5, 30,30,30,30};

    TTF_Font *font = TTF_OpenFont("./res/game.ttf", 20);
    SDL_Color color = {0, 0, 0, 255}; 
    SDL_Surface *surfaceTexte;
    SDL_Texture *textureTexte; 
    SDL_Rect destRect;

    for(int i = 0; i<nbPlayer; i++){
        char score[20];
        snprintf(score, sizeof(score), "Player %d: %d", i + 1, scores[i]);

        surfaceTexte = TTF_RenderText_Solid(font, score, color);
        textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);

        destRect = (SDL_Rect) { posX[i], posY[i], surfaceTexte->w, surfaceTexte->h};
        SDL_RenderCopy(renderer, textureTexte, NULL, &destRect);
    }

    SDL_DestroyTexture(textureTexte);
    SDL_FreeSurface(surfaceTexte);
    TTF_CloseFont(font);
}   


void affiche_win_sdl(view *v, SDL_Renderer *renderer, int indexPlayer, int score){
    char gagnant[50];
    snprintf(gagnant, sizeof(gagnant), "Player %d win with %d points !", indexPlayer, score);

    SDL_Rect *bg = createRect(300, 500, LARGEUR/2-500/2, HAUTEUR/2-300/2);
    SDL_SetRenderDrawColor(renderer, 255,0,0,255);
    SDL_RenderFillRect(renderer, bg);
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderDrawRect(renderer, bg);
    

    TTF_Font *font = TTF_OpenFont("./res/game.ttf", 20);
    SDL_Color color = {255, 255, 255, 255}; 

    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(font, gagnant, color);
    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);

    SDL_Rect destRect = (SDL_Rect) {LARGEUR/2-surfaceTexte->w/2, HAUTEUR/2-surfaceTexte->h/2, surfaceTexte->w, surfaceTexte->h};
    SDL_RenderCopy(renderer, textureTexte, NULL, &destRect);

    SDL_Rect ok = afficheButton(renderer, "  OK !  ", 300, 0);

    SDL_DestroyTexture(textureTexte);
    SDL_FreeSurface(surfaceTexte);
    TTF_CloseFont(font);
    

    v->sdl->buttons[0] = malloc(sizeof(SDL_Rect));
    v->sdl->nb_buttons = 1;
    if (!v->sdl->buttons[0]) {
        perror("[VIEW SDL] erreur allocation de mémoire pour les boutons.");
        quitter(v->sdl->window, v->sdl->renderer);
    }

    *(v->sdl->buttons[0]) = ok;

    SDL_RenderPresent(renderer);
}


void affiche_menu_sdl(view *v, int *act, int nbMenu){
    // MODFIIER SI HOVER EFFECT
    // NE RAFFRAICHI PAS LE MENU SI DEJA INIT
    if(v->sdl->menu_current == nbMenu) return;
    v->sdl->menu_current = nbMenu;
    free_buttons(v->sdl);

    char *menuText[3][4] = {
        {"TRON", "  solo  ", "  multiplayer  ", "  EXIT  "},       
        {"SOLO", "  vs Algo  ", "  vs Q-Learning  ", "  BACK  "},     
        {"MULTIPLAYER", "  on this machine (2 players)  ", "  with others  ", "  BACK  "}      
    };

    SDL_Renderer *renderer = v->sdl->renderer;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    v->get_action = get_action_menu_sdl;
    
    afficheButton(renderer, menuText[nbMenu][0], 100, 1);

    SDL_SetRenderDrawColor(renderer, 237, 237, 148, 255);

    SDL_Rect c1 = afficheButton(renderer, menuText[nbMenu][1],300, 0);

    SDL_Rect c2 = afficheButton(renderer, menuText[nbMenu][2],400, 0);

    SDL_Rect c3 = afficheButton(renderer, menuText[nbMenu][3],500, 0);

    v->sdl->nb_buttons = 3;
    v->sdl->buttons[0] = malloc(sizeof(SDL_Rect));
    v->sdl->buttons[1] = malloc(sizeof(SDL_Rect));
    v->sdl->buttons[2] = malloc(sizeof(SDL_Rect));

    if (!v->sdl->buttons[0] || !v->sdl->buttons[1] || !v->sdl->buttons[2]) {
        perror("[VIEW SDL] erreur allocation de mémoire pour les boutons.");
        quitter(v->sdl->window, v->sdl->renderer);
    }

    *(v->sdl->buttons[0]) = c1;
    *(v->sdl->buttons[1]) = c2;
    *(v->sdl->buttons[2]) = c3;

    SDL_RenderPresent(renderer);
}


void get_action_menu_sdl(view *v, actions *act, int *selected_option, int nbMenu) {
    SDL_Event event;

    const actions menuActions[3][3] = {
        {MENU_SOLO, MENU_MULTI, QUITTER},       
        {PLAY_BOT_ALGO, PLAY_BOT_Q, RETOUR},    
        {PLAY_MULTI, PLAY_ONLINE, RETOUR}       
    };

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
                        *act = menuActions[nbMenu][i];
                        return;
                        }
                }
                
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for(int i = 0; i<nb_lignes;i++){
        for (int j= 0 ;j <nb_colonnes;j++){

            SDL_Rect pixel;
            pixel.h = HAUTEUR / nb_lignes;
            pixel.w= LARGEUR / nb_colonnes;
            pixel.x = j*(LARGEUR / nb_colonnes);
            pixel.y = i*(HAUTEUR / nb_lignes);

            if(grid[i][j] == WALL){
                SDL_SetRenderDrawColor(renderer, tabColors[9].r, tabColors[9].g, tabColors[9].b, tabColors[9].a);
            }
            else{
                int indice = grid[i][j];
                if (indice < 0){
                    SDL_SetRenderDrawColor(renderer, tabColors[indice*-1].r, tabColors[indice*-1].g, tabColors[indice*-1].b, (tabColors[indice*-1].a) - 100);
                }else{
                    SDL_SetRenderDrawColor(renderer, tabColors[indice].r, tabColors[indice].g, tabColors[indice].b, tabColors[indice].a);
                }
            }
            SDL_RenderFillRect(renderer, &pixel);
        }
    }

    afficheScore(renderer, nb_player, scores);
    SDL_RenderPresent(renderer);

}