#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED
#include "ListaDoEnCi.h"
#include "cola.h"
#include "jugador.h"
#include "arbol.h" // Para poder cargar el indice de jugadores a memoria
#include "configuracion.h"
#include "Indice.h"
#include "archivos.h"
#include "menu.h"
typedef struct {
    tJugador jugador;       // Quién está jugando
    tListaDobCirc* tablero;         // El tablero actual
    tCola* colaMovimientos;       // Historial de "FX"(Forward X casillas) y "BX" (Backward X casillas)
    int vidasActuales;            // Arranca en config.vidas_inicio
    int puntosActuales;           // Los premios que va agarrando
    int turnosJugados;            // Para contar los movimientos
    int jugadorProtegido;         // Bandera (1 o 0) si pisó un Oasis
    int perdioTurno;              // Bandera (1 o 0) si pisó Tormenta
} tEstadoJuego;
void iniciarJuego(tEstadoJuego* estado, const char* nombreJugador, const char* archTablero, const char* archJugadores);
void mostrarEstado(tEstadoJuego* estado);
void realizarMovimiento(tEstadoJuego* estado, const char* movimiento);
void finalizarJuego(tEstadoJuego* estado, const char* archJugadores, const char* archPartidas);
void procesarInicioNuevaPartida(tListaDobCirc* tablero,tArbol* jugadores);
void iniciarCaravanaDelDesierto();
#endif // JUEGO_H_INCLUDED
