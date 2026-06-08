#ifndef BANDIDOS_H_INCLUDED
#define BANDIDOS_H_INCLUDED

#include "configuracion.h"
#include "interfaz.h"
#include "juego.h"
#include "jugador.h"
#include "dado.h"
#include "..\macros\macros.h"

typedef struct
{
    int posJugador;
    int *posBandidos;
    int capacidadBandidos;
    int cantBandidos;
}tPosiciones;

int definirPosiciones(tPosiciones *pos, const char* archConfig);
#endif // BANDIDOS_H_INCLUDED
