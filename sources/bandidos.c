#include "..\headers\bandidos.h"

int definirPosiciones(tPosiciones *pos, const char* archConfig)
{
    tConfig configuracion;
    if(!cargarConfiguracion(&configuracion, archConfig))
        return ERROR_ARCHIVO;

    pos->capacidadBandidos = configuracion.maximoBandidos;
    pos->posBandidos = (int*)malloc(sizeof(int)*pos->capacidadBandidos);
    if(pos->posBandidos == NULL)
        return ERROR_MEM;


    pos->TamTablero = configuracion.cantPosiciones;
    pos->cantBandidos = 0;
    pos->posJugador = -1;
    pos->posActual = 0;

    return EXITO;
}

void actualizarPosiciones(tListaDobCirc *pLista, tPosiciones *pos)
{
    recorrerListaDobCirc(pLista, encontrarPosiciones, pos);
}

void calcularMovimientos(tPosiciones *pos, tCola *cola)
{
    int mov,izq, der,idBandido,*pBandido,*pFinBandidos;
    tMovimiento movBandido;

    pBandido = pos->posBandidos;
    pFinBandidos = pos->posBandidos + pos->cantBandidos;
    idBandido = 1; // primer id asignado a los bandidos

    while(pBandido < pFinBandidos)
    {
        if(*pBandido != -1)
        {

            mov = tirar_dado(CARAS_DADO);

            der = (pos->posJugador - *pBandido + pos->TamTablero) % pos->TamTablero;
            izq = (*pBandido - pos->posJugador + pos->TamTablero) % pos->TamTablero;


            movBandido.tipoEntidad = 'B';
            movBandido.id_entidad = idBandido;
            movBandido.pasos = mov;
            movBandido.posOrigen = *pBandido;

            if(der <= izq)
                movBandido.direccion = 'F';
            else
                movBandido.direccion = 'B';

            aColar(cola, &movBandido, sizeof(tMovimiento));

        }
        pBandido++;
        idBandido++;
    }
}
void encontrarPosiciones(const void* dato, void* contexto)
{
    tPosiciones* pos = (tPosiciones*)contexto;
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
    pos->posActual++;
}


