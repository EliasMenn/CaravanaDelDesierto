#ifndef INTERFAZ_H_INCLUDED
#define INTERFAZ_H_INCLUDED
#include "juego.h"
#include "jugador.h"
#include "ListaDoEnCi.h"
#include "configuracion.h"
#include <time.h>
int creacionArchivoCaravana(const char* archCaravana,tListaDobCirc* pldc,tConfig* configuracion);
char extraerElementoAlAzar(int *cantBandidos, int *cantPremios, int *cantVidasExtras, int *cantOasis, int *cantTormentas, int *vacios, int restantes);
void mostrarTablero(tEstadoJuego* estado);
#endif // INTERFAZ_H_INCLUDED
