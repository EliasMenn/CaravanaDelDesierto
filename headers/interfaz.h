#ifndef INTERFAZ_H_INCLUDED
#define INTERFAZ_H_INCLUDED

#include <time.h>
#include "ListaDoEnCi.h"
#include "../headers/juego.h"
#include "../headers/sdl_renderer.h"

//tEstadoJuego esta declarada en "juego.h"
typedef struct tEstadoJuego tEstadoJuego;

struct sPosiciones;
//char menuPrincipal(const char *mensaje, const char *opciones); se cambio por sdl_menu
char extraerElementoAlAzar(int *cantBandidos, int *cantPremios, int *cantVidasExtras, int *cantOasis, int *cantTormentas, int *vacios, int restantes);

void mostrarTablero(tEstadoJuego* estado, struct sPosiciones* pos, tSDLCtx* ctx);

//void mostrarPantallaFinPartida(tEstadoJuego* estado); se cambio por sdl_pantallaFin

#endif // INTERFAZ_H_INCLUDED
