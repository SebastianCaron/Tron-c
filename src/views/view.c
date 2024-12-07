#include <ncurses.h>
#include <SDL2/SDL.h>

#include "./view.h"
#include "./view_ncurse.h"
#include "./view_sdl.h"
#include "../model/model.h"
#include "../controller/controller.h"


void affiche_menu_principal(view *v, actions *act){
    if(v == NULL) return;
    if(v->type == 'n'){
        afficheMenuPrincipalNC(v, act);
    }else{
        afficheMenuPrincipalSDL(v->sdl->renderer, act);
    }
}


void affiche_menu_solo(view *v, actions *act){
    if(v == NULL) return;
    if(v->type == 'n'){
        afficheMenuSoloNC(v, act);
    }else{
        afficheMenuSoloSDL(v->sdl->renderer, act);
    }
}
void affiche_menu_multijoueur(view *v, actions *act){
    if(v == NULL) return;
    if(v->type == 'n'){
        afficheMenuMultiplayerNC(v, act);
    }else{
        afficheMenuMultiplayerSDL(v->sdl->renderer, act);
    }
}