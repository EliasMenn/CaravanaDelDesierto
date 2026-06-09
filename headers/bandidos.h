#ifndef BANDIDOS_H_INCLUDED
#define BANDIDOS_H_INCLUDED

#include "configuracion.h"
#include "interfaz.h"
#include "juego.h"
#include "jugador.h"
#include "dado.h"
#include "ListaDoEnCi.h"
#include "cola.h"
#include "..\macros\macros.h"

typedef struct
{
    int TamTablero;
    int posActual;
    int posI;
    int posJugador;
    int *posBandidos;
    int capacidadBandidos;
    int cantBandidos;
}tPosiciones;

int definirPosiciones(tPosiciones *pos, const char* archConfig);
void actualizarPosiciones(tListaDobCirc *pLista, tPosiciones *pos);
void calcularMovimientos(tPosiciones *pos, tCola *cola);

void encontrarPosiciones(const void* dato, void* contexto);
#endif // BANDIDOS_H_INCLUDED
