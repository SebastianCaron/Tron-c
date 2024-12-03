#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"  //Il faut installer libsdl2-ttf-dev   //=>sudo apt-get install libsdl2-ttf-dev


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

    v->destroy_self = destroy_view_sdl;
    v->get_direction = get_direction_sdl;
    // v->update_change_screen = ;
    v->update_screen = update_screen_sdl;

    view_sdl *viewSdl =(view_sdl *)malloc(sizeof(view_sdl));
    if(!viewSdl){
        perror("[VIEW SDL] erreur allocation de la structure view_Sdl.");
        exit(EXIT_FAILURE);
    }
    v->sdl = viewSdl; 

    v->sdl->renderer=NULL;
    v->sdl->window= NULL;

    v->sdl->window = SDL_CreateWindow("TRONC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 45, 19, SDL_WINDOW_SHOWN);
    if (NULL == v->sdl->window ) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        quitter(v->sdl->window , v->sdl->renderer);
    }

    v->sdl->renderer = SDL_CreateRenderer(v->sdl->window, -1, SDL_RENDERER_ACCELERATED);
    if (NULL ==  v->sdl->renderer) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        quitter(v->sdl->window, v->sdl->renderer);
    }
    return v;
}


void destroy_view_sdl(view *v, SDL_Renderer *renderer, SDL_Window *window){
    if (NULL != renderer) SDL_DestroyRenderer(renderer);
    if (NULL != window) SDL_DestroyWindow(window);
    SDL_Quit();
    free(v);
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
}


SDL_Rect *createRect(int h, int w, int x, int y){
    SDL_Rect *ret;
    ret->h = h;
    ret->w = w;
    ret->x = x;
    ret->y =y;

    return &ret;
}

void afficheTexte(SDL_Renderer *renderer,char *texte, int x, int y) {
    TTF_Font *font = TTF_OpenFont("arial.ttf", 10);//faut voir la taille
    SDL_Color color = {255, 255, 255, 255}; 
    SDL_Surface *surfaceTexte = TTF_RenderText_Solid(font, texte, color);
    SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);

    SDL_Rect destRect = {x, y, surfaceTexte->w, surfaceTexte->h};
    SDL_RenderCopy(renderer, textureTexte, NULL, &destRect);

    SDL_DestroyTexture(textureTexte);
    SDL_FreeSurface(surfaceTexte);
    TTF_CloseFont(font);
}


void afficheMenuPrincipal(SDL_Renderer *renderer){
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

    SDL_RenderPresent(renderer);

    // Boucle d'écoute pour savoir sur qu'elle bouton on a cliqué
    SDL_Event event;
    if(event.type==SDL_MOUSEBUTTONDOWN){
        int x, y;
        SDL_GetMouseState(&x, &y);
        if(x>=solo->x && x<=solo->w && y>=solo->y && y<=solo->h){
            afficheMenuSolo(renderer);
        }else if (x>=multiplayer->x && x<=multiplayer->w && y>=multiplayer->y && y<=multiplayer->h){
            afficheMenuMultiplayer(renderer);
        }
    }

}


void afficheMenuSolo(SDL_Renderer *renderer){
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

    SDL_RenderPresent(renderer);
    // Boucle d'écoute pour savoir sur qu'elle bouton on a cliqué

    SDL_Event event;
    if(event.type==SDL_MOUSEBUTTONDOWN){
        int x, y;
        SDL_GetMouseState(&x, &y);
        if(x>=algo->x && x<=algo->w && y>=algo->y && y<=algo->h){
            // Lancer le jeu contre le bot algo
        }else if (x>=q->x && x<=q->w && y>=q->y && y<=q->h){
            // Lancer le jeu contre le bot qlearning

        }
    }
}


void afficheMenuMultiplayer(SDL_Renderer *renderer){
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

    // Boucle d'écoute pour savoir sur qu'elle bouton on a cliqué
    SDL_Event event;
    if(event.type==SDL_MOUSEBUTTONDOWN){
        int xSouris, ySouris;
        SDL_GetMouseState(&xSouris, &ySouris);
        if(xSouris>=machine->x && xSouris<=machine->w && ySouris>=machine->y && ySouris<=machine->h){
            // Lancer le jeu avec 2 joueurs
        }else if (xSouris>=others->x && xSouris<=others->w && ySouris>=others->y && ySouris<=others->h){
            // Lancer le jeu en ligne mais jsp comment faire ça.
        }
    }
}

void update_screen_sdl(int nb_player, int *scores, int **grid, int nb_lignes, int nb_colonnes, SDL_Renderer *renderer){
    if(!renderer){
        perror("[VIEW SDL] Renderer invalide.");
        exit(EXIT_FAILURE);
    }

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