#ifndef LISTADOENCI_H_INCLUDED
#define LISTADOENCI_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sNodoDob
{
    void* info;
    size_t tamInfo;
    struct sNodoDob* sig;
    struct sNodoDob* ant;
}tNodoDob;

typedef tNodoDob* tListaDobCirc;

void crearLista(tListaDobCirc *pListaDobCirc);

//Operaciones al final
int insUltListDobCirc(tListaDobCirc *pLista, void *dato, size_t tam);
int quiUltListDobCirc(tListaDobCirc *pLista, void *dato, size_t tam);

//Vaciar
void VaciarListaDobCirc(tListaDobCirc *pLista);

//Operaciones de recorrido
void recorrerListaDobCirc(tListaDobCirc* pLista, void accion(const void*, void*), void* contexto);

#endif // LISTADOENCI_H_INCLUDED
