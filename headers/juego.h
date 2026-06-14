#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include "../macros/macros.h"
#include "../headers/jugador.h"
#include "../headers/ListaDoEnCi.h"
#include "../headers/cola.h"
#include "../headers/bandidos.h"
#include "../headers/archivos.h"
#include "../headers/interfaz.h"
#include "../headers/ranking.h"
#include "../headers/utiles.h"

typedef struct {
    char tipoEntidad; // 'J' para Jugador, 'B' para Bandido
    int id_entidad;   // Identificador del bandido (ej: 1, 2...). 0 para el jugador.
    char direccion;   // 'F' para adelante (Forward), 'B' para atrás (Backward)
    int pasos;        // Cantidad de posiciones a moverse (el valor del dado)
    int posOrigen;
} tMovimiento;

typedef struct tEstadoJuego{
    tJugador jugador;
    tListaDobCirc* tablero;
    tCola* colaMovimientos; //historial de FX y BX
    int vidasActuales;
    int puntosActuales;
    int turnosJugados; //para contar los movimientos
    int jugadorProtegido; //bandera si cayo en un oasis
    int perdioTurno; //bandera si cayo en una tormenta
    char terrenoBajoJugador; //para saber que habia debajo del jugador antes de moverse (B, P, V, O, T o .)
    int colisionBandido; //bandera para saber si un bandido cayo sobre el jugador en su movimiento, se resetea cada turno
    char terrenoBajoBandido[MAX_BANDIDOS];
}tEstadoJuego;

typedef struct
{
    int idJugador;
    int puntosObtenidos;
}tPartida;

struct sPosiciones;

int tirar_dado(unsigned lados);
void aplicarMovimientoTablero(tEstadoJuego* estado, tMovimiento* mov,struct sPosiciones* pos);
int verificarEstadoTurno(tEstadoJuego* estado, int jugadorSeMovio,struct sPosiciones* pos);
void mostrarHistorialMovimientos();
void bucleJuego(tEstadoJuego* estado, tConfig* config);
void procesarInicioNuevaPartida(tEstadoJuego* estado, tConfig* config, tArbol* jugadores);
void iniciarCaravanaDelDesierto();
tNodoDob* calcularNodoDestino(tEstadoJuego* estado, tMovimiento* mov, tNodoDob* nodoActual);
void movimientoVisual(tEstadoJuego* estado,tMovimiento* mov,tNodoDob* nodoActual,tNodoDob* nodoDestino, int offset);
void sincronizarBandidosApilados(tEstadoJuego* estado, struct sPosiciones* pos);
// funciones de tPartida
int guardarPartida(tEstadoJuego* juegoActual, const char* nomArch);
int actualizarJugador(tEstadoJuego* estado, const char* nomArch);

#endif // JUEGO_H_INCLUDED
