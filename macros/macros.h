#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED




//jugador.h && algun otro .h
#define ERROR_ARCHIVO 0 //el archivo fallo al abrirse
#define EXITO 1


#define ARCH_JUGADORES "jugadores2.bin"
#define DUPLICADO 10
#define NO_EXISTE 11
#define ARCH_CONFIG "config.txt"
#define NO_EXISTE_ARCHIVO -1

#define ERROR_MEM 51
#define LISTA_VACIA 61

//menu.h
#define OPCIONES_MENU_PRINCIPAL "123"
#define MENSAJE_MENU_PRINCIPAL \
"=====================================\n" \
"\tCARAVANA DEL DESIERTO\n" \
"=====================================\n\n" \
"\t1. Nueva Partida\n" \
"\t2. Ver Ranking de jugadores\n" \
"\t3. Salir\n"

#define NUEVA_PARTIDA '1'
#define TABLA_DE_PUNTAJES '2'
#define SALIR '3'
#define CANT_OPCIONES_MENU_PRINCIPAL 3

//ListaDoEnCi.h
#define MIN(x,y) (x)>(y)?(y):(x)

//configuracion.h
#define MAX_LINEA 30

//interfaz.h
#define ARCH_CONFIG "config.txt"
#define ARCH_CARAVANA "caravana.txt"
#define ARCH_CONFIG_MAL_FORMADO 71

//juego.h
#define ARCH_INDICE_JUGADORES "ix_nomJugador.bin" // cambien el nombre como quieran


//bandidos.h
#define MAX_BANDIDOS 10


//cola.h
#define COLA_VACIA 111
#define COLA_NO_VACIA 121
#define COLA_LLENA 131
#define COLA_NO_LLENA 141

#endif //MACROS_H_INCLUDED
