#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPCIONES_MENU_PRINCIPAL "1234"
#define MENSAJE_MENU_PRINCIPAL \
"=====================================\n" \
"\tCARAVANA DEL DESIERTO\n" \
"=====================================\n\n" \
"\t1. Nueva Partida\n" \
"\t2. Continuar Partida\n" \
"\t3. Tabla de Puntajes\n" \
"\t4. Salir\n"

#define NUEVA_PARTIDA '1'
#define CONTINUAR_PARTIDA '2'
#define TABLA_DE_PUNTAJES '3'
#define SALIR '4'
#define CANT_OPCIONES_MENU_PRINCIPAL 4

char menuPrincipal(const char *mensaje, const char *opciones);

#endif // MENU_H_INCLUDED
