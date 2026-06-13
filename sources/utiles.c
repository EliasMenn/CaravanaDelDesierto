#include "../headers/utiles.h"

tNodoDob* buscarNodoEntidad(tListaDobCirc* tablero, char entidad, int id_entidad) {
    if (!(*tablero)) return NULL;

    tNodoDob* actual = *tablero;
    int contadorBandidos = 0;

    // Recorremos la lista circular
    do {
        char caracterEnNodo = *(char*)(actual->info);

        if (caracterEnNodo == entidad) {
            // Si buscamos a la 'J', devolvemos el primero que encontremos
            if (entidad == 'J') {
                return actual;
            }
            // Si buscamos a la 'B', tenemos que contar para devolver el bandido correcto
            else if (entidad == 'B') {
                contadorBandidos++;
                if (contadorBandidos == id_entidad) {
                    return actual;
                }
            }
        }
        actual = actual->sig;
    } while (actual != *tablero);

    return NULL; // No se encontró la entidad en el tablero
}


int contieneEntidad(tNodoDob* nodo, char entidad) {
    if (!nodo || !nodo->info) return 0;

    // Comparamos si el caracter del nodo coincide con el que buscamos
    return (*(char*)(nodo->info) == entidad);
}


void removerEntidadDeNodo(tNodoDob* nodo, char entidad, int id_entidad) {
    if (!nodo || !nodo->info) return;

    // Si efectivamente está la entidad en este nodo, la borramos
    if (*(char*)(nodo->info) == entidad) {
        // El documento establece que '.' representa una posición vacía o ruta despejada [cite: 76]
        *(char*)(nodo->info) = '.';
    }
}




void agregarEntidadANodo(tNodoDob* nodo, char entidad, int id_entidad) {
    if (!nodo || !nodo->info) return;

    // Sobrescribimos el casillero con la nueva entidad
    *(char*)(nodo->info) = entidad;
}

void moverJugadorAInicio(tListaDobCirc* tablero, tNodoDob* nodoActual, tEstadoJuego* estado) {
    // 1. Dejamos el casillero actual despejado (el bandido ya se consumió)
    *(char*)(nodoActual->info) = estado->terrenoBajoJugador;

    // 2. Nos movemos al nodo de inicio
    tNodoDob* nodoInicio = *tablero;

    // 3. Volvemos a memorizar la 'I' y nos posamos
    estado->terrenoBajoJugador = *(char*)(nodoInicio->info);
    *(char*)(nodoInicio->info) = 'J';
}

//guarda los movimientos en un archivo
void registrarMovimientoHistorial(tEstadoJuego* estado, char direccion, int pasos) {
    FILE* arch = fopen(ARCH_MOVIMIENTOS, "at");
    if (arch) {
        fprintf(arch, "%c%d\n", direccion, pasos);
        fclose(arch);
    }
}
