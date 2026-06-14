#include "..\headers\interfaz.h"

//muestra el menu y devuelve la opcion elegida
char menuPrincipal(const char *mensaje, const char *opciones){
    char opc;

    puts(mensaje);
    printf("\nSeleccione una opcion: ");
    fflush(stdin);
    scanf("%c", &opc);
    while(strchr(opciones, opc) == NULL){
        printf("Error! Opcion no valida. Intente nuevamente: ");
        fflush(stdin);
        scanf("%c", &opc);
    }

    system("cls");
    return opc;
}

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

void mostrarTablero(tEstadoJuego* estado, tPosiciones* pos)
{
    tNodoDob* actual = *(estado->tablero);
    int posicion = 1,contadorBandidos=0,flagBandidos=0,i=0;
    char caracter;
    if (!actual)
        return;
    do {
        caracter = *(char*)(actual->info);

        // Si hay que imprimir al jugador, armamos el casillero compuesto
        if (caracter == 'J')
        {
            if (estado->terrenoBajoJugador == '.' || estado->terrenoBajoJugador == 'I')
            {
                if (estado->terrenoBajoJugador == 'I')
                    printf("%02d:[I J]\n", posicion);
                else
                    printf("%02d:[J]\n", posicion);
            }
            else
                printf("%02d:[%c J]\n", posicion, estado->terrenoBajoJugador);
        }
        else if (caracter == 'B')
        {
            int idReal = contadorBandidos;

            if (pos != NULL && pos->posBandidos != NULL)
            {
                while( i < pos->cantBandidos && flagBandidos==0)
                {
                    if (*(pos->posBandidos + i) == posicion - 1 )
                    {
                        idReal = i;
                        flagBandidos=1;
                    }
                    i++;
                }
            }

            if ((*(estado->terrenoBajoBandido + idReal)) == '.')
                printf("%02d:B\n", posicion);
            else
                printf("%02d:[%c B]\n", posicion, (*(estado->terrenoBajoBandido + idReal)));

            contadorBandidos++;
        }
        else
            printf("%02d:%c\n", posicion, caracter);

        actual = actual->sig;
        posicion++;
    } while (actual != *(estado->tablero));

    // IMPRESIÓN DE ESTADO (HUD)
    printf("\n======================================================\n");
    printf(" JUGADOR: %s | VIDAS: %d | PUNTOS: %d | TURNOS: %d\n",
           estado->jugador.nombre,
           estado->vidasActuales,
           estado->puntosActuales,
           estado->turnosJugados);

    // Indicador visual si está protegido por el oasis o atrapado en tormenta
    if (estado->jugadorProtegido)
        printf(" [ESTADO: Protegido por el Oasis]\n");
    if (estado->perdioTurno)
        printf(" [ESTADO: Atrapado en Tormenta de Arena]\n");
    printf("======================================================\n");
}

void mostrarPantallaFinPartida(tEstadoJuego* estado)
{
    system("cls");
    printf("=====================================\n");
    printf("          PARTIDA FINALIZADA\n");
    printf("=====================================\n\n");
    printf("  Jugador : %s\n", estado->jugador.nombre);
    printf("  Puntos  : %d\n", estado->puntosActuales);
    printf("  Vidas   : %d\n", estado->vidasActuales);
    printf("  Turnos  : %d\n", estado->turnosJugados);

    if (estado->vidasActuales <= 0) {
        printf("\nResultado: Sin vidas! La caravana no llego a destino.\n");
    } else {
        printf("\nResultado: Llegaste a la Ciudad Refugio!\n");
    }

    printf("\n=====================================\n");
    printf("Presiona cualquier tecla para continuar...\n");
    getch();

    mostrarHistorialMovimientos();

    printf("\nPresiona cualquier tecla para volver al menu...\n");
    getch();
}
