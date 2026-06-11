#ifndef INDICE_H_INCLUDED
#define INDICE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../macros/macros.h"
#include "../headers/arbol.h"

int contarNodosEnIndice(tArbol* indice);
int insertarEnIndice(tArbol* indice,char* nom,unsigned tamNom);
int cargarIndiceJugadores(tArbol* indice,const char* archIndice);
int buscarEnIndice(const tArbol* indice,char* nombre);

#endif // INDICE_H_INCLUDED
