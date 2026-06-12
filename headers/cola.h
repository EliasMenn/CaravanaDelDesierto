#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include  "macros.h"

typedef struct sNodo
{
    void* dato;
    unsigned tam;
    struct sNodo* sig;
}tNodo;

typedef struct{
    tNodo* pri;
    tNodo* ult;
}tCola;

void crearCola(tCola* cola);
int colaVacia(const tCola* cola);
int aColar(tCola* cola, const void* dato, unsigned tam);
int verTope(const tCola* cola, void *dato, unsigned tam);
int outCola(tCola* cola, void* dato, unsigned tam);
void vaciarCola(tCola* cola);
#endif // COLA_H_INCLUDED
