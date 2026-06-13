#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "../macros/macros.h"
#include "../headers/jugador.h"
#include "../headers/juego.h"

typedef struct {
    int  puntajeTotal;
    char nombre[51];
    int  partidasJugadas;
} tNodoRanking;


//para enviarle a la funcion de recorrido In-Orden
typedef struct {
    int total;
    int actual;
} tContextoRanking;

int cmpRanking(const void* a, const void* b);
void imprimirNodoRanking(void* dato, void* contexto);
void mostrarRanking();

#endif // RANKING_H_INCLUDED
