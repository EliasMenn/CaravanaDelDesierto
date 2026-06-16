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

//void mostrarTablero(tEstadoJuego* estado, tPosiciones* pos)
//{
//    tNodoDob* actual = *(estado->tablero);
//    int posicion = 1,contadorBandidos=0,flagBandidos=0,i=0;
//    char caracter;
//    if (!actual)
//        return;
//    do {
//        caracter = *(char*)(actual->info);
//
//        // Si hay que imprimir al jugador, armamos el casillero compuesto
//        if (caracter == 'J')
//        {
//            if (estado->terrenoBajoJugador == '.' || estado->terrenoBajoJugador == 'I')
//            {
//                if (estado->terrenoBajoJugador == 'I')
//                    printf("%02d:[I J]\n", posicion);
//                else
//                    printf("%02d:[J]\n", posicion);
//            }
//            else
//                printf("%02d:[%c J]\n", posicion, estado->terrenoBajoJugador);
//        }
//        else if (caracter == 'B')
//        {
//            int idReal = contadorBandidos;
//
//            if (pos != NULL && pos->posBandidos != NULL)
//            {
//                while( i < pos->cantBandidos && flagBandidos==0)
//                {
//                    if (*(pos->posBandidos + i) == posicion - 1 )
//                    {
//                        idReal = i;
//                        flagBandidos=1;
//                    }
//                    i++;
//                }
//            }
//
//            if ((*(estado->terrenoBajoBandido + idReal)) == '.')
//                printf("%02d:B\n", posicion);
//            else
//                printf("%02d:[%c B]\n", posicion, (*(estado->terrenoBajoBandido + idReal)));
//
//            contadorBandidos++;
//        }
//        else
//            printf("%02d:%c\n", posicion, caracter);
//
//        actual = actual->sig;
//        posicion++;
//    } while (actual != *(estado->tablero));
//
//    // IMPRESIÓN DE ESTADO (HUD)
//    printf("\n======================================================\n");
//    printf(" JUGADOR: %s | VIDAS: %d | PUNTOS: %d | TURNOS: %d\n",
//           estado->jugador.nombre,
//           estado->vidasActuales,
//           estado->puntosActuales,
//           estado->turnosJugados);
//
//    // Indicador visual si está protegido por el oasis o atrapado en tormenta
//    if (estado->jugadorProtegido)
//        printf(" [ESTADO: Protegido por el Oasis]\n");
//    if (estado->perdioTurno)
//        printf(" [ESTADO: Atrapado en Tormenta de Arena]\n");
//    printf("======================================================\n");
//}
//Funcion mostrar tablero anterior y la nueva que necesita el paramentro de sdl:

void mostrarTablero(tEstadoJuego* estado, tPosiciones* pos, tSDLCtx* ctx)
{
    tNodoDob* actual = *(estado->tablero);
    int indiceCasillero = 0;
    int esJugador, esBandido;

    if (!actual)
        return;

    do {
        char caracterNodo = *(char*)(actual->info);
        esJugador = (caracterNodo == 'J') ? 1 : 0;
        esBandido = 0;

        char terrenoBase = caracterNodo;

        if (esJugador) {
            // Extraemos lo que pisa el jugador (O, T, P, V, I, S, .)
            terrenoBase = estado->terrenoBajoJugador;
        }
        else if (caracterNodo == 'B') {
            esBandido = 1;
            terrenoBase = '.'; // Terreno por defecto por si hay un desfasaje

            // Buscamos en el vector de bandidos cuál de todos es el que está en este índice
            if (pos) {
                for (int i = 0; i < pos->cantBandidos; i++) {
                    if (pos->posBandidos[i] == indiceCasillero) {
                        terrenoBase = estado->terrenoBajoBandido[i];
                        break;
                    }
                }
            }
        }

        // Le mandamos al renderizador el terreno REAL base y las banderas de entidades
        sdl_renderizarCasilleroIndividual(ctx, terrenoBase, indiceCasillero, esJugador, esBandido);

        indiceCasillero++;
        actual = actual->sig;

    } while (actual != *(estado->tablero));

    sdl_renderizarHUD(ctx, estado->jugador.nombre, estado->vidasActuales,
                      estado->puntosActuales, estado->turnosJugados,
                      estado->jugadorProtegido, estado->perdioTurno);
}

