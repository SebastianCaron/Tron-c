#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"  //Il faut installer libsdl2-ttf-dev   //=>sudo apt-get install libsdl2-ttf-dev


#include "../controller/controller.h"

// VIEWS
#include "view.h"
#include "view_sdl.h"

// FONTS
// xxd -i nextg.ttf > nextg.h
#include "./fonts/nextg.h"

SDL_Color tabColors[10] = { {0,0,0,255}, {255,255,0,255}, {255,0,255,255}, //Sol, J1, J2
                            {0,255,255,255}, {255,0,0,255}, {0,255,0,255}, //J3, J4, J5
                            {0,0,255,255}, {0,140,140,255}, {140,0,0,255}, //J6, J7, J8
                            {255,255,255,255}}; // Mur de la map

TTF_Font *load_font_from_memory(const unsigned char *data, unsigned int size, int ptsize) {
    SDL_RWops *rw = SDL_RWFromMem((void *)data, size);
    if (!rw) {
        fprintf(stderr, "[VIEW SDL] Erreur de creation de RWops : %s\n", SDL_GetError());
        return NULL;
    }
    TTF_Font *font = TTF_OpenFontRW(rw, 1, ptsize);
    if (!font) {
        fprintf(stderr, "[VIEW SDL] Erreur de chargement de la police : %s\n", TTF_GetError());
        return NULL;
    }
    return font;
}


view *init_view_sdl(){
    view *v = (view *) calloc(1, sizeof(view));
    if(!v){
        perror("[VIEW SDL] erreur allocation de la structure view.");
        exit(EXIT_FAILURE);
    }

    v->destroy_self = destroy_view_sdl; 
    v->get_direction = get_direction_sdl;
    v->update_change_screen = update_change_screen_sdl;
    v->update_screen = update_screen_sdl;

    v->affiche_menu = affiche_menu_sdl;
    v->affiche_winner = affiche_win_sdl;
    v->get_event = get_event_sdl;


    v->type = 's';
    v->width = 30;
    v->height = 30;
    v->nbMenu = 0;

    view_sdl *viewSdl =(view_sdl *) calloc(1, sizeof(view_sdl));
    if(!viewSdl){
        perror("[VIEW SDL] erreur allocation de la structure view_Sdl.");
        exit(EXIT_FAILURE);
    }
    v->sdl = viewSdl;

    v->sdl->font_title = load_font_from_memory(nextg_ttf, nextg_ttf_len, 70);
    v->sdl->font = load_font_from_memory(nextg_ttf, nextg_ttf_len, 46);
    v->sdl->font_score = load_font_from_memory(nextg_ttf, nextg_ttf_len, 20);

    
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
        TTF_CloseFont(v->sdl->font);
        TTF_CloseFont(v->sdl->font_title);
        TTF_CloseFont(v->sdl->font_score);
        free_buttons(v->sdl);
        SDL_DestroyRenderer(v->sdl->renderer);
        SDL_DestroyWindow(v->sdl->window);
    }
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


direction get_direction_sdl(view *v, int nb_player_on_keyboard, direction *dirs){    
    SDL_Event event;
    while (SDL_PollEvent(&event) > 0){
        if (event.type == SDL_QUIT){
            // quitter(v->sdl->window, v->sdl->renderer);
            return NODIRECTION;
        }else if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_UP:
                    dirs[0] = UP;
                    return UP;
                case SDLK_DOWN:
                    dirs[0] = DOWN;
                    return DOWN;
                case SDLK_LEFT :
                    dirs[0] = LEFT;
                    return LEFT;
                case SDLK_RIGHT:
                    dirs[0] = RIGHT;
                    return RIGHT;
                case SDLK_z:
                    dirs[nb_player_on_keyboard-1] = UP;
                    return UP;
                case SDLK_q:
                    dirs[nb_player_on_keyboard-1] = LEFT;
                    return LEFT;
                case SDLK_s:
                    dirs[nb_player_on_keyboard-1] = DOWN;
                    return DOWN;
                case SDLK_d:
                    dirs[nb_player_on_keyboard-1] = RIGHT;
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

SDL_Rect *afficheButton(SDL_Renderer *renderer,char *texte, int y, int titre, TTF_Font *font) {
    SDL_Color color; 
    if(titre == 1){
        color = (SDL_Color) {255, 255, 255, 255};
    }else{
        color = (SDL_Color) {0, 0, 0, 255};
    }
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(font, texte, color);
    if(surfaceTexte == NULL){
        perror("[VIEW SDL] erreur allocation surface button\n");
        return NULL;
    }
    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
    if(textureTexte == NULL){
        perror("[VIEW SDL] erreur allocation texture button\n");
        return NULL;
    }

    SDL_Rect *destRect = calloc(1, sizeof(SDL_Rect));
    if(destRect == NULL){
        perror("[VIEW SDL] erreur allocation rect button\n");
        return NULL;
    }
    destRect->x = LARGEUR/2 - surfaceTexte->w/2;
    destRect->y = y;
    destRect->w = surfaceTexte->w;
    destRect->h = surfaceTexte->h;
    SDL_RenderFillRect(renderer, destRect);
    SDL_RenderCopy(renderer, textureTexte, NULL, destRect);
    

    SDL_DestroyTexture(textureTexte);
    SDL_FreeSurface(surfaceTexte);

    return destRect;   
}

void afficheScore(SDL_Renderer *renderer, int nbPlayer, int *scores, TTF_Font *font) {

    int posX[8] = {10, 200, 400, 600,10, 200, 400, 600};
    int posY[8] = {5, 5, 5, 5, 24,24,24,24};

    SDL_Color color = {0, 0, 0, 255}; 
    SDL_Surface *surfaceTexte;
    SDL_Texture *textureTexte; 
    SDL_Rect destRect;

    for(int i = 0; i<nbPlayer; i++){
        char score[20];
        snprintf(score, sizeof(score), "Player %d : %d", i + 1, scores[i]);

        surfaceTexte = TTF_RenderText_Solid(font, score, color);
        textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);

        destRect = (SDL_Rect) { posX[i], posY[i], surfaceTexte->w, surfaceTexte->h};
        SDL_RenderCopy(renderer, textureTexte, NULL, &destRect);

        SDL_DestroyTexture(textureTexte);
        SDL_FreeSurface(surfaceTexte);
    }
}   


void affiche_win_sdl(view *v, int indexPlayer){
    char gagnant[50];
    snprintf(gagnant, sizeof(gagnant), "Player %d win", indexPlayer+1);

    SDL_Rect *bg = createRect(300, 500, LARGEUR/2-500/2, HAUTEUR/2-300/2);
    SDL_SetRenderDrawColor(v->sdl->renderer, 0,0,0,255);
    SDL_RenderFillRect(v->sdl->renderer, bg);
    SDL_SetRenderDrawColor(v->sdl->renderer, 255,255,255,255);
    SDL_RenderDrawRect(v->sdl->renderer, bg);
    
    SDL_Color color = {255, 255, 255, 255}; 

    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(v->sdl->font, gagnant, color);
    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(v->sdl->renderer, surfaceTexte);

    SDL_Rect destRect = (SDL_Rect) {LARGEUR/2-surfaceTexte->w/2, HAUTEUR/2-surfaceTexte->h/2, surfaceTexte->w, surfaceTexte->h};
    SDL_RenderCopy(v->sdl->renderer, textureTexte, NULL, &destRect);

    SDL_Rect *ok = afficheButton(v->sdl->renderer, "  OK  ", 370, 0, v->sdl->font);

    SDL_DestroyTexture(textureTexte);
    SDL_FreeSurface(surfaceTexte);
    
    free_buttons(v->sdl);

    v->sdl->buttons[0] = ok;
    v->sdl->nb_buttons = 1;
    if (!v->sdl->buttons[0]) {
        perror("[VIEW SDL] erreur allocation de mémoire pour les boutons.");
        quitter(v->sdl->window, v->sdl->renderer);
    }

    SDL_RenderPresent(v->sdl->renderer);
    free(bg);
}


void affiche_menu_sdl(view *v, int *act, int nbMenu){
    if(v->sdl->menu_current == nbMenu) return;
    v->sdl->menu_current = nbMenu;
    free_buttons(v->sdl);

    char *menuText[4][6] = {
        {"TRON", "  solo  ", "  multiplayer  ", "  EXIT  ", NULL, NULL},       
        {"SOLO", "  vs Algo (alphonse) ", "  vs Algo (kami cases) ", "  vs Algo (m'sieur Frodon)  ", "  vs Algo (mon espace)  ", "  BACK  "},     
        {"MULTIPLAYER", "  on this machine (2 players)  ", "  with others  ", "  BACK  ", NULL, NULL},
        {"Retour"}      
    };

    SDL_Renderer *renderer = v->sdl->renderer;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    v->get_action = get_action_menu_sdl;
    
    SDL_Rect *title = afficheButton(renderer, menuText[nbMenu][0], 50, 1, v->sdl->font_title);

    SDL_SetRenderDrawColor(renderer, 237, 237, 148, 255);
    SDL_Rect *c1 = NULL;
    SDL_Rect *c2 = NULL;
    SDL_Rect *c3 = NULL;
    SDL_Rect *c4 = NULL;
    SDL_Rect *c5 = NULL;


    c1 = afficheButton(renderer, menuText[nbMenu][1],200,0, v->sdl->font);
    c2 = afficheButton(renderer, menuText[nbMenu][2],270,0, v->sdl->font);
    c3 = afficheButton(renderer, menuText[nbMenu][3],340,0, v->sdl->font);
    if(menuText[nbMenu][4] != NULL) c4 = afficheButton(renderer, menuText[nbMenu][4],410,0, v->sdl->font);
    if(menuText[nbMenu][5] != NULL) c5 = afficheButton(renderer, menuText[nbMenu][5],480,0, v->sdl->font);

    v->sdl->nb_buttons = 5;
    v->sdl->buttons[0] = c1;
    v->sdl->buttons[1] = c2;
    v->sdl->buttons[2] = c3;
    v->sdl->buttons[3] = c4;
    v->sdl->buttons[4] = c5;

    if (!v->sdl->buttons[0] || !v->sdl->buttons[1] || !v->sdl->buttons[2]) {
        perror("[VIEW SDL] erreur allocation de mémoire pour les boutons.");
        quitter(v->sdl->window, v->sdl->renderer);
    }

    SDL_RenderPresent(renderer);
    free(title);
}


void get_action_menu_sdl(view *v, actions *act, int *selected_option, int nbMenu) {
    SDL_Event event;

    const actions menuActions[4][5] = {
        {MENU_SOLO, MENU_MULTI, QUITTER, NO_ACTION, NO_ACTION},       
        {PLAY_BOT_ALGO, PLAY_BOT_Q, PLAY_BOT_COPY, PLAY_BOT_ESPACE, RETOUR},    
        {PLAY_MULTI, PLAY_ONLINE, RETOUR, NO_ACTION, NO_ACTION},
        {NO_ACTION}
    };

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *act = QUITTER;
                return;

            case SDL_MOUSEBUTTONDOWN: {
                int x = event.button.x;
                int y = event.button.y;
                if(v->sdl->nb_buttons==1){
                    *act = RETOUR;
                    return;
                }
                for (unsigned i = 0; i < v->sdl->nb_buttons; i++) {
                    if(v->sdl->buttons[i] == NULL) continue;
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
            pixel.w = LARGEUR / nb_colonnes;
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

    afficheScore(renderer, nb_player, scores, v->sdl->font_score);
    SDL_RenderPresent(renderer);

}

void update_change_screen_sdl(view *v, int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes, position **players){
    update_screen_sdl(v, nb_player, scores, grid, nb_lignes, nb_colonnes);
    return;
    /** if(v == NULL){
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
    int to_check[5][2];
    for(int i = 0; i < nb_player; i++){
        position *p = players[i];
        to_check[0][0] = p->x;
        to_check[0][1] = p->y;
        to_check[1][0] = p->x + 1;
        to_check[2][0] = p->x - 1;
        to_check[1][1] = p->y;
        to_check[2][1] = p->y;
        to_check[3][0] = p->x;
        to_check[4][0] = p->x;
        to_check[3][1] = p->y + 1;
        to_check[4][1] = p->y - 1;

        for(int k = 0; k < 5; k++){
            SDL_Rect pixel;
            pixel.h = HAUTEUR / nb_lignes;
            pixel.w = LARGEUR / nb_colonnes;
            pixel.x = to_check[k][0]*(LARGEUR / nb_colonnes);
            pixel.y = to_check[k][1]*(HAUTEUR / nb_lignes);

            if(to_check[k][1] < 0 || to_check[k][1] >= nb_lignes || to_check[k][0] < 0 || to_check[k][0] >= nb_colonnes) continue;
            if(grid[to_check[k][1]][to_check[k][0]] == WALL){
                SDL_SetRenderDrawColor(renderer, tabColors[9].r, tabColors[9].g, tabColors[9].b, tabColors[9].a);
            }
            else{
                int indice = grid[to_check[k][1]][to_check[k][0]];
                if (indice < 0){
                    SDL_SetRenderDrawColor(renderer, tabColors[indice*-1].r, tabColors[indice*-1].g, tabColors[indice*-1].b, (tabColors[indice*-1].a) - 100);
                }else{
                    SDL_SetRenderDrawColor(renderer, tabColors[indice].r, tabColors[indice].g, tabColors[indice].b, tabColors[indice].a);
                }
            }
            SDL_RenderFillRect(renderer, &pixel);
        }
    }
    afficheScore(renderer, nb_player, scores, v->sdl->font_score);
    SDL_RenderPresent(renderer); **/
}

void get_event_sdl(view *v, actions *act){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *act = QUITTER;
                return;
            default:
                break;
        }
    }
}