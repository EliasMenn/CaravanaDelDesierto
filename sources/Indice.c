#include "Indice.h"

int contarNodosEnIndice(tArbol* indice)
{
    if(!*indice)
        return 0;

    return contarNodosEnIndice(&(*indice)->izq) + contarNodosEnIndice(&(*indice)->der) + 1;
}

int insertarEnIndice(tArbol* indice, char* nom, unsigned tamNom)
{
    if(*indice){
        if(cmp((*indice)->dato, nom) > 0)
            return insertarEnArbol(&(*indice)->izq, nom, tamNom, cmp);

        else if(cmp((*indice)->dato, nom) < 0)
            return insertarEnArbol(&(*indice)->der, nom, tamNom, cmp);

        else
            return DUPLICADO;
    }

    *indice = (tNodoArbol *)malloc(sizeof(tNodoArbol));
    if(!*indice)
        return ERROR_MEM;

    (*indice)->dato = malloc(tamNom);
    if(!(*indice)->dato){
        free(*indice);
        return ERROR_MEM;
    }

    memcpy((*indice)->dato, nom, tamNom);
    (*indice)->tamDato = tamNom;
    (*indice)->izq = NULL;
    (*indice)->der= NULL;

    return EXITO;
}

int cargarIndiceJugadores(tArbol* indice, const char* archIndice)
{
    FILE *pIndice;
    int resultado;

    //si el arbol tiene elementos, no se sobrescribe
    if(*indice)
        return EXITO;

    pIndice = fopen(nomArch, "rb");
    if(!pIndice)
        return ERROR_ARCHIVO;

    //uso una "auxiliar" para el resultado porque no puedo llamar a la funcion despues de cerrar el archivo
    resultado = cargarDesdeArchivoBinarioOrdenadoArbol(pIndice, indice, tamDato);

    fclose(pIndice);
    return resultado;
}

int buscarEnIndice(const tArbol* indice, char* nombre)
{
    if(!*indice)
        return NO_EXISTE;

    if(cmp((*indice)->dato, nombre) > 0)
        return buscarEnIndice(&(*indice)->izq, nombre, cmp);
    else if(cmp((*indice)->dato, nombre) < 0)
        return buscarEnIndice(&(*indice)->der, nombre, cmp);

    return EXITO;
}