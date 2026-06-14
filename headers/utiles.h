#ifndef UTILES_H_INCLUDED
#define UTILES_H_INCLUDED

#include "../headers/juego.h"
#include "../macros/macros.h"

//tEstadoJuego esta declarada en "juego.h"
typedef struct tEstadoJuego tEstadoJuego;

tNodoDob* buscarNodoEntidad(tListaDobCirc* tablero, char entidad);
int contieneEntidad(tNodoDob* nodo, char entidad);
void removerEntidadDeNodo(tNodoDob* nodo, char entidad, int id_entidad);
void agregarEntidadANodo(tNodoDob* nodo, char entidad, int id_entidad);
void moverJugadorAInicio(tListaDobCirc* tablero, tNodoDob* nodoActual, tEstadoJuego* estado);
void registrarMovimientoHistorial(tEstadoJuego* estado, char direccion, int pasos);

#endif // UTILES_H_INCLUDED
