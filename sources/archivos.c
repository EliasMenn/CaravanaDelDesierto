#include "..\headers\archivos.h"


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
