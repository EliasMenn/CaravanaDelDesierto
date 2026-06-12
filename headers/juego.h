#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED
#include "jugador.h"
#include "ListaDoEnCi.h"
#include "cola.h"
#include "configuracion.h"
#include "bandidos.h"
#include "menu.h"
#include "archivos.h"
#include "utils.h"
typedef struct {
    char tipoEntidad; // 'J' para Jugador, 'B' para Bandido
    int id_entidad;   // Identificador del bandido (ej: 1, 2...). 0 para el jugador.
    char direccion;   // 'F' para adelante (Forward), 'B' para atrás (Backward)
    int pasos;        // Cantidad de posiciones a moverse (el valor del dado)
    int posOrigen;
} tMovimiento;

typedef struct {
    tJugador jugador;       // Quién está jugando
    tListaDobCirc* tablero;         // El tablero actual
    tCola* colaMovimientos;       // Historial de "FX"(Forward X casillas) y "BX" (Backward X casillas)
    int vidasActuales;            // Arranca en config.vidas_inicio
    int puntosActuales;           // Los premios que va agarrando
    int turnosJugados;            // Para contar los movimientos
    int jugadorProtegido;         // Bandera (1 o 0) si pisó un Oasis
    int perdioTurno;              // Bandera (1 o 0) si pisó Tormenta
    char terrenoBajoJugador; // Para saber qué había debajo del jugador antes de moverse (B, P, V, O, T o .)
    int colisionBandido;          // Bandera (1 o 0) para saber si un bandido cayó sobre el jugador en su movimiento, se resetea cada turno
    char terrenoBajoBandido[MAX_BANDIDOS];
}tEstadoJuego;

typedef struct
{
    int idJugador;
    int puntosObtenidos;
}tPartida;

void aplicarMovimientoTablero(tEstadoJuego* estado, tMovimiento* mov);
int verificarEstadoTurno(tEstadoJuego* estado, int jugadorSeMovio);
void mostrarHistorialMovimientos();
void bucleJuego(tEstadoJuego* estado, tConfig* config);
void procesarInicioNuevaPartida(tEstadoJuego* estado, tConfig* config, tArbol* jugadores);
void iniciarCaravanaDelDesierto();



//funciones de utils

tNodoDob* buscarNodoEntidad(tListaDobCirc* tablero, char entidad, int id_entidad);
int contieneEntidad(tNodoDob* nodo, char entidad);
void removerEntidadDeNodo(tNodoDob* nodo, char entidad, int id_entidad);
void agregarEntidadANodo(tNodoDob* nodo, char entidad, int id_entidad);
void moverJugadorAInicio(tListaDobCirc* tablero, tNodoDob* nodoActual, tEstadoJuego* estado);
void registrarMovimientoHistorial(tEstadoJuego* estado, char direccion, int pasos);



// funciones de tPartida
int guardarPartida(tEstadoJuego* juegoActual,const char* nomArch);
int actualizarJugador(tEstadoJuego* estado,const char* nomArch);

#endif // JUEGO_H_INCLUDED
