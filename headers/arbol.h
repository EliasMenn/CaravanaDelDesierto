#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\macros\macros.h"

typedef struct /// PROVISORIO, CUANDO TENGAMOS EL INDICE DEFINIDO LO MODIFICAMOS, ES PARA QUE NO TIRE ERROR LA FUNCION DE PROCESAR INICIO NUEVA PARTIDA
{
    void* clave;
    int pos;
}tIndice;
typedef struct sNodoArbol
{
    void* dato;
    unsigned tamDato;
    struct sNodo* izq;
    struct sNodo* der;
}tNodoArbol;

typedef tNodoArbol* tArbol;

void crearArbol(tArbol* arbol);
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
int mostrarNodosDesdeXAltura(tArbol* arbol,int nivelMax,int inclusive, void accion(const void*));
#endif // ARBOL_H_INCLUDED
