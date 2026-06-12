#include "..\headers\bandidos.h"

int definirPosiciones(tPosiciones *pos, const char* archConfig)
{
    tConfig configuracion;
    if(!cargarConfiguracion(&configuracion,archConfig))
    {
        return ERROR_ARCHIVO;
    }
    pos->capacidadBandidos = configuracion.maximoBandidos;
    pos->posBandidos = (int*)malloc(sizeof(int)*pos->capacidadBandidos);
    if(pos->posBandidos == NULL)
    {
        // free innecesario ya que si posBandidos es NULL no reservo la memoria
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
    int mov;
    int *pBandido = pos->posBandidos;
    int *pFinBandidos = pos->posBandidos + pos->cantBandidos;
    int idBandido = 1;

    while (pBandido < pFinBandidos)
    {
        mov = tirar_dado(6);

        // --- NUEVO: CÁLCULO LINEAL DE PERSECUCIÓN ---
        // Como el mapa es una línea estática, van directo hacia el jugador
        char dirCalculada;
        if(pos->posJugador >= *pBandido) {
            dirCalculada = 'F';
        } else {
            dirCalculada = 'B';
        }

        tMovimiento movBandido;
        movBandido.tipoEntidad = 'B';
        movBandido.id_entidad = idBandido;
        movBandido.direccion = dirCalculada;
        movBandido.pasos = mov;
        movBandido.posOrigen = *pBandido; // El GPS para no perder memoria

        aColar(cola, &movBandido, sizeof(tMovimiento));

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
    else if(caracterComp == 'I')
    {
        pos->posI = pos->posActual;
    }
    pos->posActual++;
}
