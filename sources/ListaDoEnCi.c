#include "../headers/ListaDoEnCi.h"
#include "../macros/macros.h"

void crearLista(tListaDobCirc *pListaDobCirc)
{
    pListaDobCirc = NULL;
}
//Operaciones al principio
int insPrimListDobCirc(tListaDobCirc *pLista, void *dato, size_t tam)
{
    tNodoDob *ult, *nue;

    nue = malloc(sizeof(tNodoDob));

    if(!nue)
        return ERROR_MEM;

    nue->info = malloc(tam);
    if(!nue->info)
    {
        free(nue);
        return ERROR_MEM;
    }

    memcpy(nue->info, dato, tam);
    nue->tamInfo = tam;

    if(!*pLista)
    {
        nue->sig = nue;
        nue->ant = nue;
        *pLista = nue;
        return EXITO;
    }

    ult = (*pLista)->ant;

    nue->sig = *pLista;
    nue->ant = ult;

    ult->sig = nue;
    (*pLista)->ant = nue;

    *pLista = nue;

    return EXITO;
}

int quiPrimListDobCirc(tListaDobCirc *pLista, void *dato, size_t tam)
{
    tNodoDob *elim, *aux;

    if(!*pLista)
    {
        return LISTA_VACIA;
    }

    elim = *pLista;

    if(elim->ant == *pLista)
    {
        free(elim->info);
        free(elim);
        *pLista = NULL;
        return EXITO;
    }

    aux = (*pLista)->ant;
    aux->sig = elim->sig;

    aux = (*pLista)->sig;
    aux->ant = elim->ant;

    *pLista = aux;

    free(elim->info);
    free(elim);

    return EXITO;
}
