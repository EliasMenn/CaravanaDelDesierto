#include "../headers/ranking.h"

//ordena por puntaje total ascendente. Si hubo empate ordena por nombre
int cmpRanking(const void* a, const void* b)
{
    const tNodoRanking* na = (const tNodoRanking *)a;
    const tNodoRanking* nb = (const tNodoRanking *)b;
    if(na->puntajeTotal != nb->puntajeTotal)
        return na->puntajeTotal - nb->puntajeTotal;

    return strcmpi(na->nombre, nb->nombre);
}

//como el arbol esta ordenado de menor a mayor, el primer nodo es el de menor puntaje, le corresponde la ultima posicion.
void imprimirNodoRanking(void* dato, void* contexto)
{
    tNodoRanking *nodo = (tNodoRanking *)dato;
    tContextoRanking *ctx = (tContextoRanking *)contexto;

    int posicion = ctx->total - ctx->actual + 1;
    printf(" %-5d %-20s %-14d %-10d\n", posicion, nodo->nombre, nodo->puntajeTotal, nodo->partidasJugadas);
    ctx->actual++;
}

void mostrarRanking()
{
    FILE *archJugadores, *archPartida;
    tJugador *jugadores;
    tPartida partida;
    int cantJugadores, idx;
    int *puntajes;
    tArbol arbolRanking;
    tContextoRanking contexto;

    system("cls");
    puts("===================================================");
    puts("\t\tRANKING DE JUGADORES");
    puts("===================================================\n");

    //abre el archivo de jugadores. Si esta vacio no muestra nada y da la opcion de volver al menu
    archJugadores = fopen(ARCH_JUGADORES, "rb");
    if(!archJugadores)
    {
        puts("No hay jugadores registrados aun.");
        puts("\nPresiona cualquier tecla para volver al menu...");
        getch();
        return;
    }

    //se obtiene la cantidad de jugadores
    fseek(archJugadores, 0, SEEK_END);
    cantJugadores = (ftell(archJugadores) / sizeof(tJugador));
    rewind(archJugadores);

    if(cantJugadores == 0) {
        fclose(archJugadores);
        puts("No hay jugadores registrados.");
        puts("\nPresiona cualquier tecla para volver al menu...");
        getch();
        return;
    }

    jugadores = (tJugador*)malloc(cantJugadores*sizeof(tJugador));
    if (!jugadores)
    {
        fclose(archJugadores);
        puts("Error de memoria.");
        puts("\nPresiona cualquier tecla para volver al menu...");
        getch();
        return;
    }

    puntajes = (int*)calloc(cantJugadores, sizeof(int)); //es un vector para acumular los puntajes de cada jugador
    if (!puntajes) {
        free(jugadores);
        fclose(archJugadores);
        puts("Error de memoria.");
        puts("\nPresiona cualquier tecla para volver al menu...");
        getch();
        return;
    }

    //carga los registros del archivo
    fread(jugadores, sizeof(tJugador), cantJugadores, archJugadores);
    fclose(archJugadores);

    archPartida = fopen(ARCH_PARTIDAS, "rb");
    if(!archPartida){
        free(jugadores);
        free(puntajes);
        puts("Error al abrir el archivo de partidas.");
        puts("\nPresiona cualquier tecla para volver al menu...");
        getch();
        return;
    }

    else{
        while(fread(&partida, sizeof(tPartida), 1, archPartida) == 1){
            idx = partida.idJugador - 1; //el archivo de jugadores empieza desde el id 1, por eso se resta 1 aca

            //si el indice esta dentro del "rango" suma los puntos
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

    printf(" %-5s %-20s %-14s %-10s\n", "Pos.", "Nombre", "Puntos Totales", "Partidas");
    printf(" %-5s %-20s %-14s %-10s\n", "-----", "--------------------", "--------------", "--------");

    contexto.total = contarNodos(&arbolRanking);
    contexto.actual = 1;
    recorrerInOrden(&arbolRanking, imprimirNodoRanking, &contexto);

    vaciarArbol(&arbolRanking);

    puts("\n===================================================");
    puts("Presiona cualquier tecla para volver al menu...");
    getch();
}
