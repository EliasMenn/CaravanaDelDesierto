#include "../headers/arbol.h"

void crearArbol(tArbol *a){
    *a = NULL;
}

int insertarEnArbol(tArbol *a, void *dato, unsigned tamDato, int cmp(const void*, const void *)){
    if(*a){
        if(cmp((*a)->dato, dato) > 0)
            return insertarEnArbol(&(*a)->izq, dato, tamDato, cmp);

        else if(cmp((*a)->dato, dato) < 0)
            return insertarEnArbol(&(*a)->der, dato, tamDato, cmp);

        else
            return DUPLICADO;
    }

    *a = (tNodoArbol *)malloc(sizeof(tNodoArbol));
    if(!*a)
        return ERROR_MEM;

    (*a)->dato = malloc(tamDato);
    if(!(*a)->dato){
        free(*a);
        return ERROR_MEM;
    }

    memcpy((*a)->dato, dato, tamDato);
    (*a)->tamDato = tamDato;
    (*a)->izq = NULL;
    (*a)->der= NULL;

    return EXITO;
}

tArbol *buscarEnArbol(tArbol *a, void *elem, Cmp cmp){
    if(!*a)
        return NULL;

    if(cmp((*a)->dato, elem) > 0)
        return buscarEnArbol(&(*a)->izq, elem, cmp);
    else if(cmp((*a)->dato, elem) < 0)
        return buscarEnArbol(&(*a)->der, elem, cmp);

    return a;
}

int cargarArchivoBinarioOrdenadoArbol(char *nomArch, tArbol *a, unsigned tamDato){
    FILE *archivo;
    int resultado;

    //si el arbol tiene elementos, no se sobrescribe
    if(*a)
        return EXITO;

    archivo = fopen(nomArch, "rb");
    if(!archivo)
        return ERROR_ARCHIVO;

    resultado = cargarDesdeArchivoBinarioOrdenadoArbol(archivo, a, tamDato);

    fclose(archivo);
    return resultado;
}

int cargarDesdeArchivoBinarioOrdenadoArbol(FILE *archivo, tArbol *a, unsigned tamDato){
    fseek(archivo, 0, SEEK_END);

    int tam = ftell(archivo);
    //si el archivo esta vacio, no se carga nada
    if(!tam)
        return EXITO;

    return _cargarDesdeArchivoBinarioOrdenadoArbol(archivo, a, tamDato, 0, tam/tamDato - 1);
}

int _cargarDesdeArchivoBinarioOrdenadoArbol(FILE *archivo, tArbol *a, unsigned tamDato, int li, int ls){
    if(li > ls)
        return EXITO;

    int medio = (li+ls)/2, resultado;

    *a = malloc(sizeof(tNodoArbol));
    if(!*a)
        return ERROR_MEM;

    (*a)->dato = malloc(tamDato);
    if(!(*a)->dato){
        free(*a);
        return ERROR_MEM;
    }

    fseek(archivo, medio*tamDato, SEEK_SET);
    if(fread((*a)->dato, tamDato, 1, archivo) != 1){
        free((*a)->dato);
        free(*a);
        *a = NULL;
        return ERROR_ARCHIVO;
    }

    (*a)->tamDato = tamDato;
    (*a)->izq = NULL;
    (*a)->der = NULL;

    resultado = _cargarDesdeArchivoBinarioOrdenadoArbol(archivo, &(*a)->izq, tamDato, li, medio-1);
    //si hubo algun error en la rama izquierda no tiene sentido recorrer la derecha, asi que retorna el codigo de error correspondiente
    if(resultado != EXITO)
        return resultado;

    return _cargarDesdeArchivoBinarioOrdenadoArbol(archivo, &(*a)->der, tamDato, medio+1, ls);
}