#include "../headers/configuracion.h"

int cargarConfiguracion(tConfig* config, const char* archConfig)
{
    char* linea;
    int campos_leidos =0;
    FILE* pconfig = fopen(archConfig,"rt");
    if(!pconfig)
        return ERROR_ARCHIVO;

    linea= malloc(MAX_LINEA);
    if(!linea)
    {
        fclose(pconfig);
        return ERROR_MEM;
    }

    if (fgets(linea, MAX_LINEA, pconfig) == NULL) {
        fclose(pconfig);
        free(linea);
        return ERROR_ARCHIVO;
    }
    fclose(pconfig);


    // sscanf devuelve la cantidad de variables que pudo leer exitosamente
    campos_leidos = sscanf(linea, "%d|%d|%d|%d|%d|%d|%d",
                               &config->cantPosiciones,
                               &config->vidasInicio,
                               &config->maximoBandidos,
                               &config->maximoPremios,
                               &config->maximoVidasExtras,
                               &config->maximoOasis,
                               &config->maximoTormentas);

    free(linea);

    //verifico que se leyeron todos los campos
    if (campos_leidos != 7) {
        return ERROR_ARCH_MAL_FORMADO;
    }

    return 1;
}
