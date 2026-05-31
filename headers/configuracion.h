#ifndef CONFIGURACION_H_INCLUDED
#define CONFIGURACION_H_INCLUDED

typedef struct {
    int vidas_inicio;       // Vidas con las que inicia el jugador
    int puntos_premio;      // Puntos que otorga cada premio
} tConfig;
int cargarConfiguracion(tConfig* config, const char* archConfig);

#endif // CONFIGURACION_H_INCLUDED