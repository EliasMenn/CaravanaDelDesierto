#include "..\headers\cola.h"

#include "colaDinamica.h"

void crearCola(tCola* cola)
{
    cola->pri = NULL;
    cola->ult = NULL;
}

int colaVacia(const tCola* cola)
{
    if(cola->pri == NULL)
    {
        return COLA_VACIA;
    }
    return COLA_NO_VACIA;
}

/*int colaLlena(const tCola* cola, unsigned tamDato)
{

}*/

int aColar(tCola* cola, const void* dato, unsigned tam)
{
    tNodo* aux = (tNodo*)malloc(sizeof(tNodo));

    if(aux == NULL)
    {
        return ERROR_MEMORIA;
    }

    aux->dato = malloc(tam);

    if(aux->dato == NULL)
    {
        free(aux);
        return ERROR_MEMORIA;
    }

    memcpy(aux->dato,dato, tam);
    aux->tam = tam;
    aux->sig = NULL;

    if(cola->pri == NULL)
    {
        cola->pri = aux;
    }
    else
    {
        cola->ult->sig = aux;
    }

    cola->ult = aux;

    return TODO_OK;
}

int verTope(const tCola* cola, void *dato, unsigned tam)
{

    if(cola->pri == NULL)
    {
        return COLA_VACIA;
    }

    tNodo* aux = cola->pri;
    memcpy(dato,aux->dato,MIN(aux->tam,tam));

    return TODO_OK;
}

int outCola(tCola* cola, void* dato, unsigned tam)
{
    if(cola->pri == NULL)
    {
        return COLA_VACIA;
    }

    tNodo* aux = cola->pri;

    cola->pri = aux->sig;

    memcpy(dato,aux->dato,MIN(aux->tam,tam));

    free(aux->dato);
    free(aux);

    if(cola->pri == NULL)
    {
        cola->ult = NULL;
    }

    return TODO_OK;
}

void vaciarCola(tCola* cola)
{
    tNodo* aux;
    while(cola->pri != NULL)
    {
        aux = cola->pri;
        cola->pri = aux->sig;
        free(aux->dato);
        free(aux);
    }

    cola->ult = NULL;
}
