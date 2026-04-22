#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VACIO 0
#define ERROR 0
#define TODO_BIEN 1
typedef struct{

    int id;
    char username[40];
    long int puntos;
}tJugador;

int guardarJugador(tJugador* jugador,const char* archJugador);
int parseoJugador(char* linea,tJugador* jugador);

#endif // JUGADOR_H_INCLUDED
