#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED
#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macros.h"

typedef struct {
    int id;
    char nombre[50];
    int puntosTotales;
    int partidasJugadas;
} tJugador;
int guardarJugador(tJugador* jugador,const char* archJugadores);
int jugadorExiste(int id,const char* archJugadores);
#endif // JUGADOR_H_INCLUDED
