#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "ListaDoEnCi.h"
#include "juego.h"
tNodoDob* buscarNodoEntidad(tListaDobCirc* tablero, char entidad, int id_entidad);
int contieneEntidad(tNodoDob* nodo, char entidad);
void removerEntidadDeNodo(tNodoDob* nodo, char entidad, int id_entidad);
void agregarEntidadANodo(tNodoDob* nodo, char entidad, int id_entidad);
void moverJugadorAInicio(tListaDobCirc* tablero, tNodoDob* nodoActual, tEstadoJuego* estado);
void registrarMovimientoHistorial(tEstadoJuego* estado, char direccion, int pasos);
#endif // UTILS_H_INCLUDED