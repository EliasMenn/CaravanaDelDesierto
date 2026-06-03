#include "configuracion.h"

int cargarConfiguracion(tConfig* config, const char* archConfig)
{
    char* linea,*aux;
    linea=malloc(MAX_LINEA);
    if(!linea)
        return ERROR_MEM;
    FILE* pconfig=fopen(archConfig,"rt");
    if(!pconfig)
    {
        return ERROR_ARCHIVO;
    }

    if (fgets(linea, MAX_LINEA, pconfig) == NULL) {
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
