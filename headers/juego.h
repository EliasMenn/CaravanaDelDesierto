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
#include "../headers/sdl_renderer.h"
#include <windows.h>
#include <mmsystem.h>

typedef struct {
    char tipoEntidad;
    int id_entidad;
    char direccion;
    int pasos;
    int posOrigen;
} tMovimiento;

typedef struct tEstadoJuego{
    tJugador jugador;
    tListaDobCirc* tablero;
    tCola* colaMovimientos;
    int vidasActuales;
    int puntosActuales;
    int turnosJugados;
    int jugadorProtegido;
    int perdioTurno;
    char terrenoBajoJugador;
    int colisionBandido;
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
int verificarEstadoTurno(tEstadoJuego* estado, int jugadorSeMovio, struct sPosiciones* pos, tSDLCtx* ctx);
//int verificarEstadoTurno(tEstadoJuego* estado, int jugadorSeMovio,struct sPosiciones* pos);

//void bucleJuego(tEstadoJuego* estado, tConfig* config);
void bucleJuego(tEstadoJuego* estado, tConfig* config, tSDLCtx* ctx);
//void procesarInicioNuevaPartida(tEstadoJuego* estado, tConfig* config, tArbol* jugadores);
void procesarInicioNuevaPartida(tEstadoJuego* estado, tConfig* config, tArbol* jugadores, tSDLCtx* ctx);

//void mostrarHistorialMovimientos(); se borró porque se creó sdl_mostrarHistorial

void iniciarCaravanaDelDesierto(tSDLCtx* ctx);


tNodoDob* calcularNodoDestino(tEstadoJuego* estado, tMovimiento* mov, tNodoDob* nodoActual);
void movimientoVisual(tEstadoJuego* estado,tMovimiento* mov,tNodoDob* nodoActual,tNodoDob* nodoDestino, int offset);
void sincronizarBandidosApilados(tEstadoJuego* estado, struct sPosiciones* pos);
// funciones de tPartida
int guardarPartida(tEstadoJuego* juegoActual, const char* nomArch);
int actualizarJugador(tEstadoJuego* estado, const char* nomArch);

#endif // JUEGO_H_INCLUDED
