#ifndef COLADINAMICA_H_INCLUDED
#define COLADINAMICA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MIN(x,y) ((x)>=(y)?(x):(y))
typedef struct sNodo{

    void* dato;
    unsigned tam;
    struct sNodo* siguiente;
}tNodo;

typedef struct{
    tNodo* primero;
    tNodo* ultimo;
}tCola;

void CrearCola(tCola* pc);
int PonerEnCola(tCola* pc,void* pdato,unsigned tam);
int SacarDeCola(tCola* pc,void* pdato,unsigned tam);
int VerPrimero(const tCola* pc,void* pdato,unsigned tam);
void VaciarCola(tCola* pc);
int ColaVacia(tCola* pc);
int ColaLlena(tCola* pc,unsigned tam);
#endif // COLADINAMICA_H_INCLUDED
