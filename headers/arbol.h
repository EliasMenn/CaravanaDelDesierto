#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../macros/macros.h"

#define TAM_NOMBRE 51 //si se usa pasar a la bibilioteca macros.h

typedef struct sNodoArbol
{
    void* dato;
    unsigned tamDato;
    struct sNodoArbol* izq;
    struct sNodoArbol* der;
}tNodoArbol;

typedef struct{
    char nombre[TAM_NOMBRE];
    int reg;
}tIndice;

typedef tNodoArbol* tArbol;
typedef int(*Cmp)(const void *n1, const void *n2);

/*void crearArbol(tArbol* arbol);
int insertarEnArbolRecursivo(tArbol* arbol,const void* dato,unsigned tam,int cmp(const void*,const void*));
int insertarEnArbol(tArbol* arbol,const void* dato,unsigned tam,int cmp(const void*,const void*));
int _contarTodosLosNodos(tArbol* arbol,int nodosContados);
void recorrerPreOrden(tArbol* arbol,void accion(const void*));
void recorrerInOrden(tArbol* arbol,void accion(const void*));
void recorrerPosOrden(tArbol* arbol,void accion(const void*));
int contarTodosLosNodos(tArbol* arbol);
int contarNodosConHijos(tArbol* arbol);
int contarNodosHojas(tArbol* arbol);
int cortarNodosHojas(tArbol* arbol);
void vaciarArbol(tArbol* arbol);
int contarNodosConHijosXIzquierda(tArbol* arbol);
int contarNodosDondeDatoMayorA(tArbol* arbol,void* dato,int resolverCondicion(const void*,const void*));
int contarNodosDondeDatoMayorAReducido(tArbol* arbol,void* dato,int resolverCondicion(const void*,const void*));
int alturaArbol(tArbol* arbol);
int mostrarNodosDeXAltura(tArbol* arbol,int nivelElegido,void accion(const void*));
int mostrarNodosHastaXAltura(tArbol* arbol,int nivelMax,int inclusive,void accion(const void*));
int mostrarNodosDesdeXAltura(tArbol* arbol,int nivelMax,int inclusive, void accion(const void*));*/

void crearArbol(tArbol *a);
int insertarEnArbol(tArbol *a, void *dato, unsigned tamDato, Cmp cmp);
tArbol *buscarEnArbol(tArbol *a, void *elem, Cmp cmp);

int cargarArchivoBinarioOrdenadoArbol(char *nomArch, tArbol *a, unsigned tamDato);
int cargarDesdeArchivoBinarioOrdenadoArbol(FILE *archivo, tArbol *a, unsigned tamDato);
int _cargarDesdeArchivoBinarioOrdenadoArbol(FILE *archivo, tArbol *a, unsigned tamDato, int li, int ls);

#endif // ARBOL_H_INCLUDED
