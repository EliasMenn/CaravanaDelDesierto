#include "../headers/interfaz.h"
char extraerElementoAlAzar(int *cantBandidos, int *cantPremios, int *cantVidasExtras, int *cantOasis, int *cantTormentas, int *vacios, int restantes)
{
    srand(time(NULL));
    int r = rand() % restantes;

    // Evaluamos y si no es, le restamos el bloque a 'r' y pasamos al siguiente.
    if (r < *cantBandidos)
    {
        (*cantBandidos)--;
        return 'B';
    }
    r -= *cantBandidos;

    if (r < *cantPremios)
    {
        (*cantPremios)--;
        return 'P';
    }
    r -= *cantPremios;

    if (r < *cantVidasExtras)
    {
        (*cantVidasExtras)--;
        return 'V';
    }
    r -= *cantVidasExtras;

    if (r < *cantOasis)
    {
        (*cantOasis)--;
        return 'O';
    }
    r -= *cantOasis;

    if (r < *cantTormentas)
    {
        (*cantTormentas)--;
        return 'T';
    }
    r -= *cantTormentas;

    // Si no cay� en ninguno de los anteriores, es un espacio vac�o
    (*vacios)--;
    return '.';
}

int creacionArchivoCaravana(const char* archCaravana,tListaDobCirc* pldc,tConfig* configuracion)
{
    FILE* pCaravana;
    char caracterAInsertar;
    int cantPosiciones=0,cantBandidos=0,
        cantTormentas=0,cantOasis=0,
        cantPremios=0,cantVidasExtras=0,
        cantEspeciales,cantEspaciosVacios,posIterativa=1;

    if(!cargarConfiguracion(configuracion,"config.txt"))
        return ERROR_ARCHIVO;
    if(configuracion->cantPosiciones<2) // solo tiene posicion para inicio y fin, no se puede jugar
        return ERROR_ARCH_MAL_FORMADO; //no se puede jugar

    /// carga de cantidades auxiliares
    cantPosiciones=configuracion->cantPosiciones -2 ;// eliminamos de la cuenta al inicio y al fin
    cantBandidos=configuracion->maximoBandidos;
    cantVidasExtras=configuracion->maximoVidasExtras;
    cantOasis=configuracion->maximoOasis;
    cantPremios=configuracion->maximoPremios;
    cantTormentas=configuracion->maximoTormentas;

    cantEspeciales= cantBandidos + cantOasis + cantPremios + cantTormentas + cantVidasExtras;
    cantEspaciosVacios= cantPosiciones - cantEspeciales;

    if(cantEspaciosVacios <0)
        return ERROR_ARCH_MAL_FORMADO;

    pCaravana=fopen(archCaravana,"wt");
    if(!pCaravana)
        return ERROR_ARCHIVO;

    caracterAInsertar='I';
    insUltListDobCirc(pldc,&caracterAInsertar,sizeof(char)); //siempre primero cargo el inicio
    fprintf(pCaravana,"%02d:[%c J]\n",posIterativa,caracterAInsertar);
    posIterativa++;
    while(cantPosiciones>0)
    {
        caracterAInsertar=extraerElementoAlAzar(&cantBandidos,&cantPremios,&cantVidasExtras,&cantOasis,&cantTormentas,&cantEspaciosVacios,cantPosiciones);

        insUltListDobCirc(pldc,&caracterAInsertar,sizeof(char));
        fprintf(pCaravana,"%02d:%c\n",posIterativa,caracterAInsertar);
        cantPosiciones--;
        posIterativa++;

    }
    caracterAInsertar='S';
    insUltListDobCirc(pldc,&caracterAInsertar,sizeof(char));
    fprintf(pCaravana,"%02d:%c\n",posIterativa,caracterAInsertar);
    fclose(pCaravana);
    return 1;
}
