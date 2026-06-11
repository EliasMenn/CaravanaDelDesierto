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
    int izq, der;
    
    // Puntero auxiliar para recorrer el bloque de memoria
    int *pBandido = pos->posBandidos; 
    // Puntero de condición de corte (apunta al final lógico del arreglo)
    int *pFinBandidos = pos->posBandidos + pos->cantBandidos; 
    
    // Usamos esto solo para asignarles un ID visual en el tMovimiento (1, 2, 3...)
    int idBandido = 1; 

    while (pBandido < pFinBandidos)
    {
        mov = tirar_dado(6);
        
        // Calculamos distancias desreferenciando el puntero actual (*pBandido)
        der = (pos->posJugador - *pBandido + pos->TamTablero) % pos->TamTablero;
        izq = (*pBandido - pos->posJugador + pos->TamTablero) % pos->TamTablero;
        
        char dirCalculada;
        if(der <= izq)
        {
            dirCalculada = 'F';
        }
        else
        {
            dirCalculada = 'B';
        }
        
        tMovimiento movBandido;
        movBandido.tipoEntidad = 'B';
        movBandido.id_entidad = idBandido; 
        movBandido.direccion = dirCalculada;
        movBandido.pasos = mov;
        
        aColar(cola, &movBandido, sizeof(tMovimiento));
        
        // Desplazamos el puntero a la siguiente dirección de memoria de tipo int
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
