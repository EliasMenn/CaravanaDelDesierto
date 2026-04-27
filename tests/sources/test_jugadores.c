    #include "../tests.h"

    int test_jugador()
    {
        tJugador jugador = {0};
        jugador.id = 1;
        strcpy(jugador.nombre, "Malcom");
        jugador.puntosTotales = 26611;
        int res;

        printf("\nAgregamos un jugador nuevo");
        res = guardarJugador(&jugador,ARCH_JUGADORES);
        if(res != EXITO)
            return 1;

        printf("\nIntentamos agregar de vuelta el mismo jugador");
        res = guardarJugador(&jugador,ARCH_JUGADORES);
        if(res != DUPLICADO)
            return 1;

        printf("\nVemos si el jugador existe");
        res = jugadorExiste(jugador.id,ARCH_JUGADORES);
        if(res != DUPLICADO)
            return 1;

        jugador.id = 2;
        strcpy(jugador.nombre, "Reese");
        jugador.puntosTotales = 26611;

        printf("\nProbamos con otro jugador");
        res = jugadorExiste(jugador.id,ARCH_JUGADORES);
        if(res != NO_EXISTE)
            return 1;

        printf("\nProbamos con otro archivo");
        res = jugadorExiste(jugador.id,"jugadores.bin");
        if(res != NO_EXISTE_ARCHIVO)
            return 1;

        return 0;
    }
