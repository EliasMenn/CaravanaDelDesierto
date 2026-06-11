#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../macros/macros.h"

#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))

typedef struct sNodoArbol
{
    void* dato;
    unsigned tamDato;
    struct sNodoArbol* izq;
    struct sNodoArbol* der;
}tNodoArbol;

typedef tNodoArbol* tArbol;
typedef int(*Cmp)(const void *n1, const void *n2);

void crearArbol(tArbol *a);
void recorrerPreOrden(tArbol *a, Accion accion, void *contexto);
void recorrerInOrden(tArbol *a, Accion accion, void *contexto);
void recorrerPosOrden(tArbol *a, Accion accion, void *contexto);
int insertarEnArbol(tArbol *a, void *dato, unsigned tamDato, Cmp cmp);
int contarNodos(tArbol *a);
int contarHojas(tArbol *a);
int contarNodosConHijos(tArbol *a);
int contarNodosConHijosSoloPorIzq(tArbol *a);
int contarNodosConHijosSoloPorDer(tArbol *a);
int obtenerAltura(tArbol *a);
int contarNodosEnAlturaN(tArbol *a, int altura);
int contarNodosHastaAlturaN(tArbol *a, int altura);
int contarNodosHastaAlturaNInclusive(tArbol *a, int altura);
int contarNodosAPartirDeAlturaN(tArbol *a, int altura);
int _contarNodosAPartirDeAlturaN(tArbol *a, int alturaActual, int altura);
void cortarHojas(tArbol *a);
void vaciarArbol(tArbol *a);
tArbol *buscarEnArbol(tArbol *a, void *elem, Cmp cmp);

int cargarArchivoBinarioOrdenadoArbol(char *nomArch, tArbol *a, unsigned tamDato);
int cargarDesdeArchivoBinarioOrdenadoArbol(FILE *archivo, tArbol *a, unsigned tamDato);
int _cargarDesdeArchivoBinarioOrdenadoArbol(FILE *archivo, tArbol *a, unsigned tamDato, int li, int ls);

#endif // ARBOL_H_INCLUDED
