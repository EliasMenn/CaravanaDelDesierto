#ifndef CONFIGURACION_H_INCLUDED
#define CONFIGURACION_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macros.h"
typedef struct {
    int cant_posiciones;
    int vidas_inicio;
    int maximo_bandidos;
    int maximo_premios;
    int maximo_vidas_extras;
    int maximo_oasis;
    int maximo_tormentas;
 } tConfig;
int cargarConfiguracion(tConfig* config, const char* archConfig);

#endif // CONFIGURACION_H_INCLUDED
