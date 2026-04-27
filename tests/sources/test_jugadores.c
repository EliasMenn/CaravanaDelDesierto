    #include "../tests.h"

    int test_jugador()
    {
        tJugador jugador;
        jugador=(tJugador){1,"Malcom",26611};
        int res;

        printf("Agregamos un jugador nuevo");
        res = guardarJugador(&jugador,ARCH_JUGADORES);
        if(res != EXITO)
            return 1;

        printf("Intentamos agregar de vuelta el mismo jugador");
        res = guardarJugador(&jugador,ARCH_JUGADORES);
        if(res != DUPLICADO)
            return 1;

        printf("Vemos si el jugador existe");
        res = guardarJugador(&jugador,ARCH_JUGADORES);
        if(res != DUPLICADO)
            return 1;

        jugador=(tJugador){2,"Reese",211};

        printf("Probamos con otro jugador");
        res = guardarJugador(&jugador,ARCH_JUGADORES);
        if(res != NO_EXISTE)
            return 1;

        printf("Probamos con otro archivo");
        res = guardarJugador(&jugador,"jugadores.bin");
        if(res != NO_EXISTE_ARCHIVO)
            return 1;

        return 0;
    }
