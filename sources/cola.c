#include "../headers/cola.h"

void crearCola(tCola* cola)
{
    cola->pri = NULL;
    cola->ult = NULL;
}

int colaVacia(const tCola* cola)
{
    if(cola->pri == NULL)
        return COLA_VACIA;

    return COLA_NO_VACIA;
}

int colaLlena(const tCola* cola, unsigned tamDato)
{
    tNodo* aux = (tNodo*)malloc(sizeof(tNodo));

    if(aux == NULL)
        return COLA_LLENA;

    aux->dato = malloc(tamDato);

    if(aux->dato == NULL)
    {
        free(aux);
        return COLA_LLENA;
    }

    free(aux->dato);
    free(aux);

    return COLA_NO_LLENA;
}

int aColar(tCola* cola, const void* dato, unsigned tam)
{
    tNodo* aux = (tNodo*)malloc(sizeof(tNodo));

    if(aux == NULL)
        return ERROR_MEM;

    aux->dato = malloc(tam);

    if(aux->dato == NULL)
    {
        free(aux);
        return ERROR_MEM;
    }

    memcpy(aux->dato,dato, tam);
    aux->tam = tam;
    aux->sig = NULL;

    if(cola->pri == NULL)
        cola->pri = aux;

    else
        cola->ult->sig = aux;

    cola->ult = aux;

    return EXITO;
}

int verTope(const tCola* cola, void *dato, unsigned tam)
{
    tNodo *aux;
    if(cola->pri == NULL)
        return COLA_VACIA;

    aux = cola->pri;
    memcpy(dato,aux->dato,MIN(aux->tam,tam));

    return EXITO;
}

int outCola(tCola* cola, void* dato, unsigned tam)
{
    tNodo *aux;

    if(cola->pri == NULL)
        return COLA_VACIA;

    aux = cola->pri;
    cola->pri = aux->sig;

    memcpy(dato,aux->dato,MIN(aux->tam,tam));

    free(aux->dato);
    free(aux);

    if(cola->pri == NULL)
        cola->ult = NULL;

    return EXITO;
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

