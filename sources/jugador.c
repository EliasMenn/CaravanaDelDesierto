#include "../headers/jugador.h"


int guardarJugador(tJugador* jugador,const char* archJugadores)
{
    FILE* pj;
    if(jugadorExiste(jugador->id,archJugadores)==DUPLICADO)
            return DUPLICADO;
    pj=fopen(archJugadores,"ab");
    if(!pj)
        return ERROR_ARCHIVO;
    fwrite(jugador,sizeof(tJugador),1,pj);
    fclose(pj);
    return EXITO;
}

int jugadorExiste(int id,const char* archJugadores)///LOGICA PROVISORIA, CUANDO VEAMOS INDICES SOBRE ARBOLES BINARIOS VAMOS A TENER QUE MANEJARLO CON ESO (PARA NO RECORRER EL ARCHIVO)
{
    tJugador jugador;
    FILE* pf=fopen(archJugadores,"rb");
    if (!(pf))
        return NO_EXISTE_ARCHIVO;

    while(fread(&jugador, sizeof(tJugador), 1, pf) == 1)
    {
        if(jugador.id == id)
        {
            fclose(pf);
            return DUPLICADO;  // ya existe
        }
    }

    fclose(pf);
    return NO_EXISTE;  // no existe
}
