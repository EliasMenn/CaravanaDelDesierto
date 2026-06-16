#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/arbol.h"
#include "../macros/macros.h"
#include "Indice.h"
#include <conio.h>

typedef struct {
    int id;
    char nombre[51];
    int partidasJugadas;
} tJugador;

int guardarJugador(tJugador* jugador,const char* archJugadores,const tArbol* indice,Cmp cmp);
int BuscarNombreJugador(char* nombre, const tArbol* jugadores, Cmp cmp, int* posEncontrada);

#endif // JUGADOR_H_INCLUDED
