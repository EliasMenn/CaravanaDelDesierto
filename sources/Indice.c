#include "Indice.h"


int contarNodosEnIndice(tArbol* indice)
{
    if(!*indice)
        return 0;

    return contarNodosEnIndice(&(*indice)->izq) + contarNodosEnIndice(&(*indice)->der) + 1;
}

int insertarEnIndice(tArbol* indice, char* nom, unsigned tamNom,Cmp cmp)
{
    if(*indice){
        if(cmp((*indice)->dato, nom) > 0)
            return insertarEnArbol(&(*indice)->izq, nom, tamNom, cmp);

        else if(cmp((*indice)->dato, nom) < 0)
            return insertarEnArbol(&(*indice)->der, nom, tamNom, cmp);

        else
            return DUPLICADO;
    }

    *indice = (tNodoArbol *)malloc(sizeof(tNodoArbol));
    if(!*indice)
        return ERROR_MEM;

    (*indice)->dato = malloc(tamNom);
    if(!(*indice)->dato){
        free(*indice);
        return ERROR_MEM;
    }

    memcpy((*indice)->dato, nom, tamNom);
    (*indice)->tamDato = tamNom;
    (*indice)->izq = NULL;
    (*indice)->der= NULL;

    return EXITO;
}

int cargarIndiceJugadores(tArbol* indice, const char* archIndice,unsigned tamDato)
{
    FILE *pIndice;
    int resultado;

    //si el arbol tiene elementos, no se sobrescribe
    if(*indice)
        return EXITO;

    pIndice = fopen(archIndice, "rb");
    if(!pIndice)
        return ERROR_ARCHIVO;

    //uso una "auxiliar" para el resultado porque no puedo llamar a la funcion despues de cerrar el archivo
    resultado = cargarDesdeArchivoBinarioOrdenadoArbol(pIndice, indice, tamDato);

    fclose(pIndice);
    return resultado;
}

// 1. Nueva Búsqueda por referencia
int buscarEnIndice(const tArbol* indice, char* nombre, Cmp cmp, int* posEncontrada)
{
    if(!*indice)
        return NO_EXISTE;

    int comparacion = cmp((*indice)->dato, nombre);

    if(comparacion > 0)
        return buscarEnIndice(&(*indice)->izq, nombre, cmp, posEncontrada);
    else if(comparacion < 0)
    return buscarEnIndice(&(*indice)->der, nombre, cmp, posEncontrada);

    // Lo encontró: Guardamos la posición en el puntero y devolvemos EXITO
    tIndice* registroEncontrado = (tIndice*)((*indice)->dato);
    *posEncontrada = registroEncontrado->pos;
    return EXITO;
}

// 2. Funciones para guardar el Árbol en disco antes de salir
void _grabarArbol(tArbol* a, FILE* pf) {
    if (!*a) return;
    _grabarArbol(&(*a)->izq, pf); // Recorrido In-Orden para que quede ordenado
    fwrite((*a)->dato, (*a)->tamDato, 1, pf);
    _grabarArbol(&(*a)->der, pf);
}

void guardarIndiceJugadores(tArbol* indice, const char* archIndice) {
    FILE* pf = fopen(archIndice, "wb"); // wb = Pisar y crear de cero
    if (pf) {
        _grabarArbol(indice, pf);
        fclose(pf);
    }
}

