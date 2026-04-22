#include "../../headers/jugador/jugador.h"

int guardarJugador(tJugador* jugador,const char* archJugador)
{
    FILE* pj=fopen(archJugador,"at");
    if(!pj)
        return ERROR;
    fprintf(pj,"%d;%s;%ld\n",jugador->id,jugador->username, jugador->puntos);
    fclose(pj);
    return TODO_BIEN;
}
