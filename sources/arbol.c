#include "../headers/arbol.h"

void crearArbol(tArbol *a){
    *a = NULL;
}

void recorrerPreOrden(tArbol *a, Accion accion, void *contexto){
    if(!*a)
        return ;

    accion((*a)->dato, contexto);
    recorrerPreOrden(&(*a)->izq, accion, contexto);
    recorrerPreOrden(&(*a)->der, accion, contexto);
}

void recorrerInOrden(tArbol *a, Accion accion, void *contexto){
    if(!*a)
        return ;

    recorrerInOrden(&(*a)->izq, accion, contexto);
    accion((*a)->dato, contexto);
    recorrerInOrden(&(*a)->der, accion, contexto);
}

void recorrerPosOrden(tArbol *a, Accion accion, void *contexto){
    if(!*a)
        return ;

    recorrerPosOrden(&(*a)->izq, accion, contexto);
    recorrerPosOrden(&(*a)->der, accion, contexto);
    accion((*a)->dato, contexto);
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

int contarNodos(tArbol *a){
    if(!*a)
        return 0;

    return contarNodos(&(*a)->izq) + contarNodos(&(*a)->der) + 1;
}

int contarHojas(tArbol *a){
    if(!*a)
        return 0;

    if(!(*a)->izq && !(*a)->der)
        return 1;

    return contarHojas(&(*a)->izq) + contarHojas(&(*a)->der);
}

int contarNodosConHijos(tArbol *a){
    if(!*a)
        return 0;

    if((*a)->izq || (*a)->der)
        return contarNodosConHijos(&(*a)->izq) + contarNodosConHijos(&(*a)->der) + 1;

    return 0;
}

int contarNodosConHijosSoloPorIzq(tArbol *a){
    if(!*a)
        return 0;

    if((*a)->izq && !(*a)->der)
        return 1 + contarNodosConHijosSoloPorIzq(&(*a)->izq);

    return contarNodosConHijosSoloPorIzq(&(*a)->izq) + contarNodosConHijosSoloPorIzq(&(*a)->der);
}

int contarNodosConHijosSoloPorDer(tArbol *a){
    if(!*a)
        return 0;

    if(!(*a)->izq && (*a)->der)
        return 1 + contarNodosConHijosSoloPorDer(&(*a)->der);

    return contarNodosConHijosSoloPorDer(&(*a)->izq) + contarNodosConHijosSoloPorDer(&(*a)->der);
}

int obtenerAltura(tArbol *a){
    if(!*a)
        return 0;

    int altIzq = obtenerAltura(&(*a)->izq);
    int altDer = obtenerAltura(&(*a)->der);

    return MAX(altIzq, altDer) + 1;
}

int contarNodosEnAlturaN(tArbol *a, int altura){
    if(!*a)
        return 0;

    if(altura == 1)
        return 1;

    return contarNodosEnAlturaN(&(*a)->izq, altura-1) + contarNodosEnAlturaN(&(*a)->der, altura-1);
}

int contarNodosHastaAlturaN(tArbol *a, int altura){
    if(!*a)
        return 0;

    if(altura == 2)
        return 1;

    return contarNodosEnAlturaN(&(*a)->izq, altura-1) + contarNodosEnAlturaN(&(*a)->der, altura-1) + 1;
}

int contarNodosHastaAlturaNInclusive(tArbol *a, int altura){
    if(!*a)
        return 0;

    if(altura == 1)
        return 1;

    return contarNodosHastaAlturaNInclusive(&(*a)->izq, altura-1) + contarNodosHastaAlturaNInclusive(&(*a)->der, altura-1) + 1;
}

int contarNodosAPartirDeAlturaN(tArbol *a, int altura){
    return _contarNodosAPartirDeAlturaN(a, 1, altura);
}

int _contarNodosAPartirDeAlturaN(tArbol *a, int alturaActual, int altura){
    if(!*a)
        return 0;

    int cont = alturaActual >= altura ? 1 : 0;

    return _contarNodosAPartirDeAlturaN(&(*a)->izq, alturaActual+1, altura) + _contarNodosAPartirDeAlturaN(&(*a)->der, alturaActual+1, altura) + cont;
}

void cortarHojas(tArbol *a){
    if(!*a)
        return ;

    if(!(*a)->izq && !(*a)->der){
        free((*a)->dato);
        free(*a);
        *a = NULL;
        return ;
    }

    cortarHojas(&(*a)->izq);
    cortarHojas(&(*a)->der);
}

void vaciarArbol(tArbol *a){
    if(!*a)
        return ;

    vaciarArbol(&(*a)->izq);
    vaciarArbol(&(*a)->der);

    free((*a)->dato);
    free(*a);
    *a = NULL;
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

//recibe un archivo abierto y lo carga en un arbol, esta funcion es un envoltorio
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