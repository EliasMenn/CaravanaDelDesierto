#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED
#include "ListaDoEnCi.h"
typedef struct sNodo
{
    void* dato;
    int tamDato;
    struct sNodo* sig;
}tNodo;

typedef tNodo* tCola;

void crearCola(tCola* cola)
int colaVacia(const tCola* cola)
int aColar(tCola* cola, const void* dato, unsigned tam)
int verTope(const tCola* cola, void *dato, unsigned tam)
int outCola(tCola* cola, void* dato, unsigned tam)
void vaciarCola(tCola* cola)
#endif // COLA_H_INCLUDED
