#include "../headers/utiles.h"

tNodoDob* buscarNodoEntidad(tListaDobCirc* tablero, char entidad)
{
    char caracterEnNodo;
    tNodoDob* actual;
    if (!(*tablero))
        return NULL;

    actual= *tablero;

    // Recorremos la lista circular
    do
    {
        caracterEnNodo = *(char*)(actual->info);

        if (caracterEnNodo == entidad)
            return actual;
        actual = actual->sig;
    }while (actual != *tablero);

    return NULL; // No se encontró la entidad en el tablero
}


int contieneEntidad(tNodoDob* nodo, char entidad)
{
    if (!nodo || !nodo->info)
        return 0;

    // Comparamos si el caracter del nodo coincide con el que buscamos
    return (*(char*)(nodo->info) == entidad);
}


void removerEntidadDeNodo(tNodoDob* nodo, char entidad, int id_entidad)
{
    if (!nodo || !nodo->info) return;

    // Si efectivamente está la entidad en este nodo, la borramos
    if (*(char*)(nodo->info) == entidad)
        *(char*)(nodo->info) = '.';
}




void agregarEntidadANodo(tNodoDob* nodo, char entidad, int id_entidad)
{
    if (!nodo || !nodo->info) return;

    // Sobrescribimos el casillero con la nueva entidad
    *(char*)(nodo->info) = entidad;
}

void moverJugadorAInicio(tListaDobCirc* tablero, tNodoDob* nodoActual, tEstadoJuego* estado)
{
    tNodoDob* nodoInicio;
    if(!nodoActual)
        return;

    *(char*)(nodoActual->info) = estado->terrenoBajoJugador;

    //Nos movemos al nodo de inicio
    nodoInicio = *tablero;

    //Volvemos a memorizar la 'I' y nos posicionamos al inicio
    estado->terrenoBajoJugador = *(char*)(nodoInicio->info);
    *(char*)(nodoInicio->info) = 'J';
}

//guarda los movimientos en un archivo
void registrarMovimientoHistorial(tEstadoJuego* estado, char direccion, int pasos)
{
    FILE* arch = fopen(ARCH_MOVIMIENTOS, "at");
    if (arch)
    {
        fprintf(arch, "Direccion: %c | Pasos: %d\n", direccion, pasos);
        fclose(arch);
    }
}
