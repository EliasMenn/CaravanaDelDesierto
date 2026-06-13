#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include "../headers/jugador.h"
#include "../headers/ListaDoEnCi.h"

typedef struct {
    int cantPosiciones;
    int vidasInicio;
    int maximoBandidos;
    int maximoPremios;
    int maximoVidasExtras;
    int maximoOasis;
    int maximoTormentas;
 } tConfig;

/**esta funcion es para buscar en el archivo original de jugadores el id y partidas jugadas del jugador,
a partir de la posicion que me dio el indice (que es la posicion del nombre del jugador en el archivo original)*/
int buscarJugadorEnArchivo(const char* archJugadores,int posicion,tJugador* jugador);

int cargarConfiguracion(tConfig* config, const char* archConfig);
int creacionArchivoCaravana(const char* archCaravana, tListaDobCirc* pldc, tConfig* configuracion);

#endif // ARCHIVOS_H_INCLUDED
