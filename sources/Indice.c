#include "Indice.h"


int contarNodosEnIndice(tArbol* arbol)
{
    /// FALTA IMPLEMENTAR
    return EXITO;
}


int insertarEnIndice(tArbol* indice,char* nom,unsigned tamNom)
{
    /// FALTA IMPLEMENTAR
    return EXITO;
}


int cargarIndiceJugadores(tArbol* indice,const char* archIndice)
{
    FILE* pIndice=fopen(archIndice,"rb");


    if(!pIndice)
        return ERROR_ARCHIVO;

    ///FALTA IMPLEMENTAR
    return EXITO;
}

int buscarEnIndice(const tArbol* indice,char* nombre)
{
    ///FALTA IMPLEMENTAR
    return EXITO;
}
