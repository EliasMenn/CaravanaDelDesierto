#include "../headers/utiles.h"

tNodoDob* buscarNodoEntidad(tListaDobCirc* tablero, void* entidad,Cmp cmp)
{
    void* caracterEnNodo;
    tNodoDob* actual;
    if (!(*tablero))
        return NULL;

    actual= *tablero;

    // Recorremos la lista circular
    do
    {
        caracterEnNodo = (actual->info);

        if (cmp(entidad,caracterEnNodo)==0)
            return actual;
        actual = actual->sig;
    }while (actual != *tablero);

    return NULL; // No se encontró la entidad en el tablero
}

void moverJugadorAInicio(tListaDobCirc* tablero, tNodoDob* nodoActual, tEstadoJuego* estado)
{
    tNodoDob* nodoInicio;
    if(!nodoActual)
        return;

    *(char*)(nodoActual->info) = estado->terrenoBajoJugador;

    nodoInicio = *tablero;

    estado->terrenoBajoJugador = *(char*)(nodoInicio->info);
    *(char*)(nodoInicio->info) = 'J';
}

void registrarMovimientoHistorial(tEstadoJuego* estado, char direccion, int pasos)
{
    FILE* arch = fopen(ARCH_MOVIMIENTOS, "at");
    if (arch)
    {
        fprintf(arch, "Direccion: %c | Pasos: %d\n", direccion, pasos);
        fclose(arch);
    }
}
