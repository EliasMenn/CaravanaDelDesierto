#include "./headers/juego.h"

int main(int argc, char *argv[])
{
    iniciarCaravanaDelDesierto();
    return 0;
}

///TESTEOS

/*
    tJugador jugador;
    jugador=(tJugador){1,"Malcom",26611};

    menuPrincipal(MENSAJE_MENU_PRINCIPAL,OPCIONES_MENU_PRINCIPAL);
    tJugador jugador;
    jugador=(tJugador){3,"Dewey",412};
    if(guardarJugador(&jugador,ARCH_JUGADORES))
        printf("se guardo exitosamente el jugador\n");
    else
        printf("hubo un error al guardar el jugador\n");
    FILE* pf=fopen(ARCH_JUGADORES,"rb");
    if(!pf)
        return 1;
    fread(&jugador,sizeof(tJugador),1,pf);
    while(!feof(pf))
    {
        printf(" %d | %s | %lld\n",jugador.id,jugador.nombre,jugador.puntosTotales);
        fread(&jugador,sizeof(tJugador),1,pf);
    }

    /// testeo creado de archivo caravana.txt
    void mostrarString(const void* datoLista)
    {
    char* auxDato=(char*)datoLista;

    printf("[%c]\n",*auxDato);
    }

    tListaDobCirc pldc;
    tConfig config;
    crearLista(&pldc);
    creacionArchivoCaravana("caravana.txt",&pldc,&config);
    recorrerListaDobCirc(&pldc,mostrarString);












    if (argc > 1 && strcmp(argv[1], "test") == 0)
    {
        return main_test();
    }
    else
    {

    }
    return 0;







*/
