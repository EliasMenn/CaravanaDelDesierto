#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED
#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbol.h"
#include "..\macros\macros.h"
#include "Indice.h"
#include <conio.h>

typedef struct {
    int id;
    char nombre[50];
    int partidasJugadas;
} tJugador;
int guardarJugador(tJugador* jugador,const char* archJugadores);
int jugadorExiste(int id,const char* archJugadores);
int BuscarNombreJugador(char* nombre,const tArbol* jugadores);
#endif // JUGADOR_H_INCLUDED
