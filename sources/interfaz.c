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
    int indiceCasillero = 0, totalCasilleros = 0;
    int esJugador, esBandido, i;
    char caracterNodo, terrenoBase;

    if (!actual)
        return;

    tNodoDob* aux = actual;
    do {
        totalCasilleros++;
        aux = aux->sig;
    } while (aux != *(estado->tablero));
    // --------------------------------------------------------------------------

    do {
        caracterNodo = *(char*)(actual->info);
        esJugador = (caracterNodo == 'J') ? 1 : 0;
        esBandido = 0;
        terrenoBase = caracterNodo;

        if (pos) {
            i = 0;
            // El ciclo sigue mientras haya bandidos por revisar Y todavía no hayamos encontrado a ninguno acá
            while (i < pos->cantBandidos && esBandido == 0) {
                if (*(pos->posBandidos+i) == indiceCasillero) {
                    esBandido = 1; // Al ponerse en 1, el while se va a cortar limpiamente en la próxima vuelta
                    terrenoBase = *(estado->terrenoBajoBandido+i);
                    if (terrenoBase == 'B' || terrenoBase == 'J') {
                        terrenoBase = '.';
                    }
                }
                i++;
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

