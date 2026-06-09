#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include "configuracion.h"
#include "interfaz.h"
#include "juego.h"
#include "jugador.h"

int buscarJugadorEnArchivo(const char* archJugadores,int posicion,tJugador* jugador);// esta funcion es para buscar en el archivo original de jugadores el id y partidas jugadas del jugador,
                                                                          // a partir de la posicion que me dio el indice (que es la posicion del nombre del jugador en el archivo original)
#endif // ARCHIVOS_H_INCLUDED
