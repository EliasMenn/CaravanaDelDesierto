#include "../headers/ranking.h"

int cmpRanking(const void* a, const void* b)
{
    const tNodoRanking* na = (const tNodoRanking *)a;
    const tNodoRanking* nb = (const tNodoRanking *)b;
    if(na->puntajeTotal != nb->puntajeTotal)
        return na->puntajeTotal - nb->puntajeTotal;

    return strcmpi(na->nombre, nb->nombre);
}

// Ahora esta funcion es llamada por el arbol, pero en vez de hacer printf, llama a SDL
void imprimirNodoRankingGrafico(void* dato, void* contexto)
{
    tNodoRanking *nodo = (tNodoRanking *)dato;
    tContextoRanking *ctx = (tContextoRanking *)contexto;
    char posStr[16], ptsStr[16], partStr[16];
    int y;
    int posicion = ctx->total - ctx->actual + 1;

    // Solo pedimos dibujar a los mejores 15 para que no desborde hacia abajo la ventana
    if (posicion <= 15) {
        // La Posicion 1 caera en Y=192, la 2 en Y=224, etc
        y = 160 + (posicion * 32);

        SDL_snprintf(posStr, sizeof(posStr), "%d", posicion);
        SDL_snprintf(ptsStr, sizeof(ptsStr), "%d", nodo->puntajeTotal);
        SDL_snprintf(partStr, sizeof(partStr), "%d", nodo->partidasJugadas);

        // Directamente mandamos los strings crudos al motor grafico
        sdl_dibujarFilaRanking(ctx->sdlCtx, posStr, nodo->nombre, ptsStr, partStr, y);
    }

    ctx->actual++;
}

void mostrarRanking(tSDLCtx* ctx)
{
    FILE *archJugadores, *archPartida;
    tJugador *jugadores;
    tPartida partida;
    int cantJugadores, idx;
    int *puntajes;
    tArbol arbolRanking;
    tContextoRanking contexto;

    archJugadores = fopen(ARCH_JUGADORES, "rb");
    if(!archJugadores)
        return;

    fseek(archJugadores, 0, SEEK_END);
    cantJugadores = (ftell(archJugadores) / sizeof(tJugador));
    rewind(archJugadores);

    if(cantJugadores == 0) {
        fclose(archJugadores);
        return;
    }

    jugadores = (tJugador*)malloc(cantJugadores*sizeof(tJugador));
    if (!jugadores) {
        fclose(archJugadores);
        return;
    }

    puntajes = (int*)calloc(cantJugadores, sizeof(int));
    if (!puntajes) {
        free(jugadores);
        fclose(archJugadores);
        return;
    }

    fread(jugadores, sizeof(tJugador), cantJugadores, archJugadores);
    fclose(archJugadores);

    archPartida = fopen(ARCH_PARTIDAS, "rb");
    if(archPartida) {
        while(fread(&partida, sizeof(tPartida), 1, archPartida) == 1) {
            idx = partida.idJugador - 1;
            if (idx >= 0 && idx < cantJugadores)
                *(puntajes + idx) += partida.puntosObtenidos;
        }
        fclose(archPartida);
    }

    crearArbol(&arbolRanking);
    for (int i = 0; i < cantJugadores; i++) {
        tNodoRanking entrada;
        entrada.puntajeTotal = *(puntajes + i);
        entrada.partidasJugadas = (*(jugadores + i)).partidasJugadas;
        strncpy(entrada.nombre, (*(jugadores + i)).nombre, 50);
        entrada.nombre[49] = '\0';

        insertarEnArbol(&arbolRanking, &entrada, sizeof(tNodoRanking), cmpRanking);
    }

    free(jugadores);
    free(puntajes);


    contexto.total = cantJugadores;
    contexto.actual = 1;
    contexto.sdlCtx = ctx; // Le entregamos la variable de SDL al contexto del arbol

    sdl_iniciarPantallaRanking(ctx); // 1. Pinta el fondo y las cabeceras
    recorrerInOrden(&arbolRanking, imprimirNodoRankingGrafico, &contexto); // 2. El In-Orden dibuja las filas
    sdl_finalizarPantallaRanking(ctx); // 3. Se hace RenderPresent y espera que toquemos algo

    vaciarArbol(&arbolRanking);
}
