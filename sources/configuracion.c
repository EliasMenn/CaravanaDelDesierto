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
    sscanf(aux+1,"%d",&config->maximo_tormentas);
    if(aux)
        *aux='\0';
    /** cant_oasis*/
    aux=strrchr(linea,'|');
    sscanf(aux+1,"%d",&config->maximo_oasis);
    if(aux)
        *aux='\0';
    /** cant_vidas_extras*/
    aux=strrchr(linea,'|');
    sscanf(aux + 1,"%d",&config->maximo_vidas_extras);
    if(aux)
        *aux='\0';
    /** cant_premios */
    aux=strrchr(linea,'|');
    sscanf(aux + 1,"%d",&config->maximo_premios);
    if(aux)
        *aux='\0';
    /** cant_bandidos*/
    aux=strrchr(linea,'|');
    sscanf(aux + 1,"%d",&config->maximo_bandidos);
    if(aux)
        *aux='\0';
    /** cant_vidas_inicio*/
    aux=strrchr(linea,'|');
    sscanf(aux + 1,"%d",&config->vidas_inicio);
    if(aux)
        *aux='\0';
    /** cant_posiciones*/
    sscanf(linea,"%d",&config->cant_posiciones);

    free(linea);
    return 1;
}
