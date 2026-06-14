    #include "../tests.h"
    int cmpCadena(const void* d1,const void* d2)
    {
        tIndice* indice=(tIndice*)d1;
        char* nombre=(char*)d2;

        return strcmpi(indice->clave,nombre);
    }
    int test_jugador()
    {
        tArbol indice;
        tJugador jugador = {0};
        int res,pos=0;
        jugador.id = 1;
        crearArbol(&indice);
        strcpy(jugador.nombre, "Malcom");

        printf("\nAgregamos un jugador nuevo");
        res = guardarJugador(&jugador,ARCH_JUGADORES,&indice,cmpCadena);
        if(res != EXITO)
            return 1;
        insertarEnIndice(&indice,jugador.nombre,sizeof(jugador.nombre),cmpCadena);
        printf("\nIntentamos agregar de vuelta el mismo jugador");
        res = guardarJugador(&jugador,ARCH_JUGADORES,&indice,cmpCadena);
        if(res != DUPLICADO)
            return 1;

        printf("\nVemos si el jugador existe");
        res = buscarEnIndice(&indice,jugador.nombre,cmpCadena,&pos);
        if(res != DUPLICADO)
            return 1;

        jugador.id = 2;
        strcpy(jugador.nombre, "Reese");

        printf("\nProbamos con otro jugador");
        res = buscarEnIndice(&indice,jugador.nombre,cmpCadena,&pos);
        if(res != NO_EXISTE)
            return 1;


        return 0;
    }
