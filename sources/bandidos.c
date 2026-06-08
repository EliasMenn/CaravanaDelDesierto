#include "..\headers\bandidos.h"

int definirPosiciones(tPosiciones *pos, const char* archConfig)
{
    tConfig configuracion;
    cargarConfiguracion(&configuracion,archConfig);
    pos->capacidadBandidos = configuracion.maximoBandidos;
    pos->posBandidos = (int*)malloc(sizeof(int)*pos->capacidadBandidos);
    if(pos->posBandidos == NULL)
    {
        free(pos->posBandidos);
        return ERROR_MEM;
    }
    pos->cantBandidos = 0;
    pos->posJugador = -1;
    return EXITO;
}



