#include "..\headers\interfaz.h"


char extraerElementoAlAzar(int *cantBandidos, int *cantPremios, int *cantVidasExtras, int *cantOasis, int *cantTormentas, int *vacios, int restantes)
{
    srand(time(NULL));
    int r = rand() % restantes;


    if (r < *cantBandidos)
    {
        (*cantBandidos)--;
        return 'B';
    }
    r -= *cantBandidos;

    if (r < *cantPremios)
    {
        (*cantPremios)--;
        return 'P';
    }
    r -= *cantPremios;

    if (r < *cantVidasExtras)
    {
        (*cantVidasExtras)--;
        return 'V';
    }
    r -= *cantVidasExtras;

    if (r < *cantOasis)
    {
        (*cantOasis)--;
        return 'O';
    }
    r -= *cantOasis;

    if (r < *cantTormentas)
    {
        (*cantTormentas)--;
        return 'T';
    }
    r -= *cantTormentas;

    (*vacios)--;
    return '.';
}

void mostrarTablero(tEstadoJuego* estado, tPosiciones* pos, tSDLCtx* ctx)
{
    tNodoDob* actual = *(estado->tablero);
    int indiceCasillero = 0;
    int esJugador, esBandido;

    if (!actual)
        return;

    // --- NUEVO: Contamos cuántos casilleros hay en total para poder centrar ---
    int totalCasilleros = 0;
    tNodoDob* aux = actual;
    do {
        totalCasilleros++;
        aux = aux->sig;
    } while (aux != *(estado->tablero));
    // --------------------------------------------------------------------------

    do {
        char caracterNodo = *(char*)(actual->info);
        esJugador = (caracterNodo == 'J') ? 1 : 0;
        esBandido = 0;
        char terrenoBase = caracterNodo;

        if (pos) {
            for (int i = 0; i < pos->cantBandidos; i++) {
                if (pos->posBandidos[i] == indiceCasillero) {
                    esBandido = 1;
                    terrenoBase = estado->terrenoBajoBandido[i];
                    if (terrenoBase == 'B' || terrenoBase == 'J') {
                        terrenoBase = '.';
                    }
                    break;
                }
            }
        }

        if (esJugador) {
            terrenoBase = estado->terrenoBajoJugador;
        }

        // AHORA LE PASAMOS EL TOTAL DE CASILLEROS AL FINAL DEL LLAMADO
        sdl_renderizarCasilleroIndividual(ctx, terrenoBase, indiceCasillero, esJugador, esBandido, totalCasilleros);

        indiceCasillero++;
        actual = actual->sig;

    } while (actual != *(estado->tablero));

    sdl_renderizarHUD(ctx, estado->jugador.nombre, estado->vidasActuales,
                      estado->puntosActuales, estado->turnosJugados,
                      estado->jugadorProtegido, estado->perdioTurno);
}

