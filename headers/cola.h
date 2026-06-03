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


#endif // COLA_H_INCLUDED
