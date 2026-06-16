#ifndef SDL_RENDERER_H_INCLUDED
#define SDL_RENDERER_H_INCLUDED

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>


#define VENTANA_W       1600
#define VENTANA_H       900
#define CELL_W          46
#define CELL_H          75
#define CELL_GAP        4
#define BOARD_Y         150
#define HUD_Y           700
#define FONT_PATH       "DejaVuSans.ttf"
#define FONT_SIZE_HUD   20
#define FONT_SIZE_CELL  13

typedef struct {
    SDL_Window* ventana;
    SDL_Renderer* renderer;
    TTF_Font* fuenteHud;
    TTF_Font* fuenteCell;
    int           inicializado;

    char          eventoTitulo[64];
    char          eventoDetalle[128];
    int           eventoEsMalo;
    int           mostrarEvento;
} tSDLCtx;

// FUNCIONES DE EVENTOS E HISTORIAL
void sdl_mostrarMensajeContextual(tSDLCtx* ctx, const char* titulo, const char* detalle, int esMalo);
void sdl_renderizarOverlayEvento(tSDLCtx* ctx);
void sdl_mostrarHistorial(tSDLCtx* ctx, const char* ruta_archivo);

// Ciclo de vida
int  sdl_iniciar(tSDLCtx* ctx);
void sdl_cerrar(tSDLCtx* ctx);

// Renderizado principal
void sdl_renderizarCasilleroIndividual(tSDLCtx* ctx, char tipo, int idx,
                                       int esJugador, int esBandido);


void sdl_renderizarHUD(tSDLCtx* ctx, const char* nombre, int vidas,
                       int puntos, int turnos, int protegido, int perdioTurno);

void sdl_presentar(tSDLCtx* ctx);
void sdl_limpiar(tSDLCtx* ctx);

// Input
// Devuelve 'F', 'B', 'Q' (salir) o 0 (ninguna tecla relevante)
void sdl_dibujarMensajeDado(tSDLCtx* ctx, int dado);
char sdl_esperarDireccion(tSDLCtx* ctx);

// Pantalla de fin de partida
void sdl_pantallaFin(tSDLCtx* ctx, const char* nombre, int puntos,
                     int vidas, int turnos, int gano);

// Pantalla de menu
// Devuelve '1', '2' o '3'
char sdl_menu(tSDLCtx* ctx);

void sdl_pedirNombre(tSDLCtx* ctx, char* buffer, int maxLen);
void sdl_pantallaBienvenida(tSDLCtx* ctx, const char* nombre, int id, int partidas);

void sdl_iniciarPantallaRanking(tSDLCtx* ctx);
void sdl_dibujarFilaRanking(tSDLCtx* ctx, const char* pos, const char* nom, const char* pts, const char* part, int y);
void sdl_finalizarPantallaRanking(tSDLCtx* ctx);

#endif // SDL_RENDERER_H_INCLUDED
