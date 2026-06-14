#include "../headers/archivos.h"
#include "../headers/interfaz.h"

int buscarJugadorEnArchivo(const char* archJugadores,int posicion,tJugador* jugador)
{
    FILE* pf = fopen(archJugadores, "rb");
    if (!pf)
        return 0; // Error al abrir el archivo


    fseek(pf, posicion * sizeof(tJugador), SEEK_SET);

    fread(jugador,sizeof(tJugador),1,pf);//Dadas las validaciones sabemos que existe entonces no hago la validacion de la lectura

    fclose(pf);
    return 1; // Retorna 1 si el jugador fue encontrado, 0 en caso contrario
}

int cargarConfiguracion(tConfig* config, const char* archConfig)
{
    char* linea,*aux;
    linea=malloc(MAX_LINEA);
    if(!linea)
        return ERROR_MEM;
    FILE* pconfig=fopen(archConfig,"rt");
    if(!pconfig)
    {
        free(linea);
        return ERROR_ARCHIVO;
    }

    if (fgets(linea, MAX_LINEA, pconfig) == NULL)
    {
        fclose(pconfig);
        free(linea);
        return ERROR_ARCHIVO;
    }
    fclose(pconfig);

    aux=strchr(linea,'\n');
    if(aux)
        *aux='\0';
    /** cant_tormentas*/
    aux=strrchr(linea,'|');
    sscanf(aux+1,"%d",&config->maximoTormentas);
    if(aux)
        *aux='\0';
    /** cant_oasis*/
    aux=strrchr(linea,'|');
    sscanf(aux+1,"%d",&config->maximoOasis);
    if(aux)
        *aux='\0';
    /** cant_vidas_extras*/
    aux=strrchr(linea,'|');
    sscanf(aux + 1,"%d",&config->maximoVidasExtras);
    if(aux)
        *aux='\0';
    /** cant_premios */
    aux=strrchr(linea,'|');
    sscanf(aux + 1,"%d",&config->maximoPremios);
    if(aux)
        *aux='\0';
    /** cant_bandidos*/
    aux=strrchr(linea,'|');
    sscanf(aux + 1,"%d",&config->maximoBandidos);
    if(aux)
        *aux='\0';
    /** cant_vidas_inicio*/
    aux=strrchr(linea,'|');
    sscanf(aux + 1,"%d",&config->vidasInicio);
    if(aux)
        *aux='\0';
    /** cant_posiciones*/
    sscanf(linea,"%d",&config->cantPosiciones);

    free(linea);
    return 1;
}

int creacionArchivoCaravana(const char* archCaravana, tListaDobCirc* pldc, tConfig* configuracion)
{
    FILE* pCaravana;
    char caracterAInsertar;
    int cantPosiciones=0,cantBandidos=0,
        cantTormentas=0,cantOasis=0,
        cantPremios=0,cantVidasExtras=0,
        cantEspeciales,cantEspaciosVacios,posIterativa=1;

    if(!cargarConfiguracion(configuracion,ARCH_CONFIG))
        return ERROR_ARCHIVO;
    if(configuracion->cantPosiciones<2) // solo tiene posicion para inicio y fin, no se puede jugar
        return ARCH_CONFIG_MAL_FORMADO; //no se puede jugar

    /// carga de cantidades auxiliares
    cantPosiciones=configuracion->cantPosiciones -2 ;// eliminamos de la cuenta al inicio (I:Inicio) y al fin (S:Salida)
    cantBandidos=configuracion->maximoBandidos;
    cantVidasExtras=configuracion->maximoVidasExtras;
    cantOasis=configuracion->maximoOasis;
    cantPremios=configuracion->maximoPremios;
    cantTormentas=configuracion->maximoTormentas;

    cantEspeciales= cantBandidos + cantOasis + cantPremios + cantTormentas + cantVidasExtras;
    cantEspaciosVacios= cantPosiciones - cantEspeciales;

    if(cantEspaciosVacios <0)
        return ARCH_CONFIG_MAL_FORMADO;

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
