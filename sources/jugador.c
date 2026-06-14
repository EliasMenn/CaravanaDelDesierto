#include "../headers/jugador.h"

int guardarJugador(tJugador* jugador,const char* archJugadores,const tArbol* indice,Cmp cmp)
{
    FILE* pj;
    int pos;
    if(buscarEnIndice(indice,jugador->nombre,cmp,&pos) == EXITO)
        return DUPLICADO;

    pj = fopen(archJugadores,"ab");
    if(!pj)
        return ERROR_ARCHIVO;

    fwrite(jugador, sizeof(tJugador), 1, pj);
    fclose(pj);

    return EXITO;
}




int BuscarNombreJugador(char* nombre, const tArbol* jugadores, Cmp cmp, int* posEncontrada)
{
    return buscarEnIndice(jugadores, nombre, cmp, posEncontrada);
}
