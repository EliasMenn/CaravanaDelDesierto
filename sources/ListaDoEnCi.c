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
//Operaciones al final
int insUltListDobCirc(tListaDobCirc *pLista, void *dato, size_t tam)
{
    tNodoDob *nue, *ult;
    nue=(tNodoDob*)malloc(sizeof(tNodoDob));
    if(!nue)
        return ERROR_MEM;
    nue->info=malloc(tam);
    if(!nue->info)
    {
        free(nue);
        return ERROR_MEM;
    }
    memcpy(nue->info,dato,tam);
    nue->tamInfo=tam;
    if(!(*pLista))
    {
        (*pLista)=nue;
        nue->sig=nue;
        nue->ant=nue;
        return EXITO;
    }
    ult=(*pLista)->ant;
    nue->sig=(*pLista);
    nue->ant=ult;
    ult->sig=nue;
    (*pLista)->ant=nue;
    return EXITO;
}
int quiUltListDobCirc(tListaDobCirc *pLista, void *dato, size_t tam)
{
    tNodoDob *elim, *aux;

    if(!*pLista)
    {
        return LISTA_VACIA;
    }

    elim = (*pLista)->ant;

    if(elim->ant == elim)
    {
        free(elim->info);
        free(elim);
        *pLista = NULL;
        return EXITO;
    }

    aux = elim->ant;
    aux->sig = *pLista;

    (*pLista)->ant = aux;

    free(elim->info);
    free(elim);

    return EXITO;
}

//Operaciones de consulta
int verPrimListDobCirc(const tListaDobCirc *pLista, void *dato, size_t tam)
{
    if(!(*pLista))
        return LISTA_VACIA;
    memcpy(dato,(*pLista)->info,MIN(tam,(*pLista)->tamInfo));
    return EXITO;
}
int verUltListDobCirc(const tListaDobCirc *pLista, void *dato,size_t tam)
{
    if(!(*pLista))
        return LISTA_VACIA;
    memcpy(dato,(*pLista)->ant->info,MIN((*pLista)->ant->tamInfo,tam));
    return EXITO;
}
//Operaciones de ordenamiento
int InsOrdListDobCirc(tListaDobCirc *pLista,void * dato, unsigned tam, int cmp(const void* dato1,const void* dato2))
{
    tNodoDob *nue, *act, *ant;
    int comp;

    if(!pLista)
        return NO_EXISTE;

    nue = (tNodoDob*)malloc(sizeof(tNodoDob));
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

    act=*pLista;
    ant=(*pLista)->ant;
    do
    {
        comp = cmp(dato, act->info);

        if(comp==0)
        {
            free(nue->info);
            free(nue);
            return DUPLICADO;
        }

        if(comp>0)
        {
            ant=act;
            act=act->sig;
        }
    } while(act!=*pLista && comp>0);

    nue->sig=act;
    nue->ant=ant;
    ant->sig=nue;
    act->ant=nue;

    if(act==*pLista && comp<0)
        *pLista=nue;

    return EXITO;
}
//Operaciones de eliminación
void VaciarListaDobCirc(tListaDobCirc *pLista)
{
    tNodoDob *elim;
    tNodoDob *aux;

    if (!*pLista)
        return;
    
    elim = (*pLista)->sig; // Empezamos desde el nodo siguiente a la cabecera
    
    while (elim!=*pLista )// Iteramos mientras no demos una vuelta completa
    {
        aux = elim->sig;
        free(elim->info);      
        free(elim);               
        elim = aux;            
    }
    free((*pLista)->info); //Liberamos la cabecera
    free(*pLista);
    *pLista = NULL;
}

int elimListDobCirc(tListaDobCirc *pLista, void *dato, size_t tam, int cmp(const void * dato1, const void *dato2))
{
    return EXITO;
}
