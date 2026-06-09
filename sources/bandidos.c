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
    pos->TamTablero = configuracion.cantPosiciones;
    pos->cantBandidos = 0;
    pos->posJugador = -1;
    pos->posActual = 0;
    return EXITO;
}

void actualizarPosiciones(tListaDobCirc *pLista, tPosiciones *pos)
{
    recorrerListaDobCirc(pLista,encontrarPosiciones,pos);
}

void calcularMovimientos(tPosiciones *pos, tCola *cola)
{
    int i = 0, mov;
    int izq, der;
    while(i<(pos->cantBandidos))
    {
        mov = tirar_dado(6);
        der = (pos->posJugador-*(pos->posBandidos+i)+pos->TamTablero)%pos->TamTablero;
        izq = (*(pos->posBandidos+i)-pos->posJugador+pos->TamTablero)%pos->TamTablero;
        if(der<izq)
        {
            *(pos->posBandidos+i)+=mov;
            if((*(pos->posBandidos+i)>=pos->TamTablero)
            {
                (*(pos->posBandidos+i)-=pos->TamTablero;
            }
            aColar(cola,(pos->posBandidos+i),sizeof(int));
        }
        else
        {
            (*pos->posBandidos+i)-=mov;
            if((*(pos->posBandidos+i)<0)
            {
                (*(pos->posBandidos+i)+=pos->TamTablero;
            }
            aColar(cola,(pos->posBandidos+i),sizeof(int));
        }
        i++;
    }
}

void encontrarPosiciones(const void* dato, void* contexto)
{
    tPosiciones pos = (tPosiciones*)contexto;
    char caracterComp = *(char*)dato;

    if(caracterComp == 'B')
    {
        *(pos->posBandidos+pos->cantBandidos) = pos->posActual;
        pos->cantBandidos++;
    }
    else if(caracterComp == 'J')
    {
        pos->posJugador = pos->posActual;
    }
    else if(caracterComp == 'I')
    {
        pos->posI = pos->posActual;
    }
    pos->posActual++;
}




