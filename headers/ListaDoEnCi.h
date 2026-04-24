#ifndef LISTADOENCI_H_INCLUDED
#define LISTADOENCI_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sNodo
{
    void* info;
    size_t tamInfo;
    struct sNodo* sig;
    struct sNodo* ant;
}tNodoDob;

typedef tNodoDob* tListaDobCirc;

void crearLista(tListaDobCirc *pListaDobCirc);
//Operaciones al principio
int insPrimListDobCirc(tListaDobCirc *pLista, void *dato, size_t tam);
int quiPrimListDobCirc(tListaDobCirc *pLista, void *dato, size_t tam);
#endif // LISTADOENCI_H_INCLUDED
