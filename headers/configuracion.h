#ifndef CONFIGURACION_H_INCLUDED
#define CONFIGURACION_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\macros\macros.h"
typedef struct {
    int cantPosiciones;
    int vidasInicio;
    int maximoBandidos;
    int maximoPremios;
    int maximoVidasExtras;
    int maximoOasis;
    int maximoTormentas;
 } tConfig;
int cargarConfiguracion(tConfig* config, const char* archConfig);

#endif // CONFIGURACION_H_INCLUDED
