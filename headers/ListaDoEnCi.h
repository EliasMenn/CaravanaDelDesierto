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
//Operaciones al final
int insUltListDobCirc(tListaDobCirc *pLista, void *dato, size_t tam);
int quiUltListDobCirc(tListaDobCirc *pLista, void *dato, size_t tam);
//Operaciones de consulta
int verPrimListDobCirc(const tListaDobCirc *pLista, void *dato, size_t tam);
int verUltListDobCirc(const tListaDobCirc *pLista, void *dato,size_t tam);
//Operaciones de ordenamiento
int ordListDobCirc(tListaDobCirc *pLista, int (*comparar)(const void *, const void *));
//Operaciones de eliminación
void vaciarListDobCirc(tListaDobCirc *pLista);
int elimListDobCirc(tListaDobCirc *pLista, void *dato, size_t tam, int (*comparar)(const void *, const void *));
#endif // LISTADOENCI_H_INCLUDED
