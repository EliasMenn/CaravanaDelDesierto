#ifndef INDICE_H_INCLUDED
#define INDICE_H_INCLUDED

#include "../headers/arbol.h"

typedef struct
{
    char clave[51];
    int pos;
}tIndice;

int contarNodosEnIndice(tArbol* indice);
int insertarEnIndice(tArbol* indice, char* nom, unsigned tamNom,Cmp cmp);
int cargarIndiceJugadores(tArbol* indice, const char* archIndice,unsigned tamDato);
int buscarEnIndice(const tArbol* indice, char* nombre, Cmp cmp, int* posEncontrada);
void guardarIndiceJugadores(tArbol* indice, const char* archIndice);

#endif // INDICE_H_INCLUDED
