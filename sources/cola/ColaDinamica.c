#include "../../headers/cola/ColaDinamica.h"


void CrearCola(tCola* pc)
{
    pc->primero=pc->ultimo=NULL;
}

int PonerEnCola(tCola* pc,void* pdato,unsigned tam)
{
    tNodo* nue=(tNodo*)malloc(sizeof(tNodo));
    if(!nue)
        return 0;
    nue->dato=malloc(tam);
    if(!nue->dato)
        return 0;
    nue->tam=tam;
    memcpy(nue->dato,pdato,tam);
    nue->siguiente=NULL;
    if(!pc->primero)
        pc->ultimo->siguiente=nue;
    else
        pc->primero=nue;
    pc->ultimo=nue;
    return 1;
}

int SacarDeCola(tCola* pc,void* pdato,unsigned tam)
{
    tNodo* elim=pc->primero;
    if(!elim)
        return 0;
    memcpy(pdato,elim->dato,MIN(tam,elim->tam));
    pc->primero=elim->siguiente;
    if(!pc->primero) ///ES DECIR, SI DESPUES DEL NODO QUE ELIMINO NO HAY OTRO NODO ENTONCES EL ULTIMO NODO VA A SER NULO
        pc->ultimo=NULL;
    free(elim->dato);
    free(elim);
    return 1;
}

int ColaVacia(tCola* pc)
{
    return !pc->primero; ///SI NO EXISTE PRIMERO DEVUELVE (1), SI EXISTE Y NO ES NULL ENTONCES DEVUELVE (0)
}

int ColaLlena(tCola* pc,unsigned tam)
{
    void* aux=malloc(sizeof(tNodo));
    void* dato=malloc(tam);
    free(aux);
    free(dato);
    return !dato || !aux;

}


void VaciarCola(tCola* pc)
{
    while(pc->primero)
    {
        tNodo* elim=pc->primero;
        pc->primero=elim->siguiente;
        free(elim->dato);
        free(elim);
    }
    pc->ultimo=NULL;
}

int VerPrimero(const tCola* pc,void* pdato,unsigned tam)
{
    if(!pc->primero)
        return 0;

    memcpy(pdato,pc->primero,MIN(tam,pc->primero->tam));

    return 1;

}
