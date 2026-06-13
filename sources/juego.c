#include "..\headers\juego.h"

void iniciarCaravanaDelDesierto()
{
    tListaDobCirc tablero = NULL;
    tConfig configuracion;
    tArbol indiceJugadores = NULL;
    tCola colaMov;
    tEstadoJuego estado;

    crearCola(&colaMov);

    estado.tablero = &tablero;
    estado.colaMovimientos = &colaMov;

    //se baja el indice de jugadores
    cargarIndiceJugadores(&indiceJugadores, ARCH_INDICE_JUGADORES,sizeof(tIndice));

    char opcion = menuPrincipal(MENSAJE_MENU_PRINCIPAL, OPCIONES_MENU_PRINCIPAL);
    while(opcion != SALIR){
        //se limpia el tablero de la partida anterior
        tablero = NULL;

        if(opcion == NUEVA_PARTIDA){
            creacionArchivoCaravana(ARCH_CARAVANA, estado.tablero, &configuracion);
            procesarInicioNuevaPartida(&estado, &configuracion, &indiceJugadores);
            bucleJuego(&estado, &configuracion);

            actualizarJugador(&estado, ARCH_JUGADORES);
            guardarPartida(&estado, ARCH_PARTIDAS);
            guardarIndiceJugadores(&indiceJugadores, ARCH_INDICE_JUGADORES);

            mostrarPantallaFinPartida(&estado);
        }

        if(opcion == TABLA_DE_PUNTAJES)
            mostrarRanking();

        system("cls");
        opcion = menuPrincipal(MENSAJE_MENU_PRINCIPAL, OPCIONES_MENU_PRINCIPAL);
    }

    //si sale del while es porque el jugador eligio salir del juego
    system("cls");
    puts("Muchas gracias por jugar!");
}

int tirar_dado(unsigned lados)
{
    srand(time(NULL));
    return rand() % lados + 1;
}

int cmp(const void* d1,const void* d2)
{
    tIndice* auxIndice1=(tIndice*)d1;
    tIndice* auxIndice2=(tIndice*)d2;

    return strcmpi(auxIndice1->clave,auxIndice2->clave);
}

void aplicarMovimientoTablero(tEstadoJuego* estado, tMovimiento* mov) {
    if (!estado->tablero || !*(estado->tablero)) return;

    tNodoDob* nodoActual = NULL;
    int offset = 0;
    tNodoDob* nodoDestino;
    int pasosRestantes = mov->pasos;
    char direccionActual = mov->direccion;
    int moviValido = 0;

    if (mov->tipoEntidad == 'J') {
        nodoActual = buscarNodoEntidad(estado->tablero, 'J', 0);
    } else {
        offset = mov->id_entidad - 1;
        nodoActual = *(estado->tablero);
        for (int i = 0; i < mov->posOrigen; i++) {
            nodoActual = nodoActual->sig;
        }
    }

    if (!nodoActual) return;

    if (mov->tipoEntidad == 'B' && *(char*)(nodoActual->info) != 'B') {
        return;
    }

    nodoDestino = nodoActual;

    while (pasosRestantes > 0 && moviValido == 0) {
        tNodoDob* posibleDestino;
        if (direccionActual == 'F' || direccionActual == 'f') posibleDestino = nodoDestino->sig;
        else posibleDestino = nodoDestino->ant;

        // --- LOGICA DE JUGADOR ---
        if (mov->tipoEntidad == 'J') {
            // REBOTE EN LA META ('S')
            if ((direccionActual == 'F' || direccionActual == 'f') && posibleDestino == *(estado->tablero)) {
                direccionActual = 'B';
                posibleDestino = nodoDestino->ant;
            }
            // REBOTE EN EL INICIO ('I')
            else if ((direccionActual == 'B' || direccionActual == 'b') && posibleDestino == (*(estado->tablero))->ant) {
                break;
            }
        }
        // --- LOGICA DE LOS BANDIDOS ---
        else if (mov->tipoEntidad == 'B') {
            if ((direccionActual == 'F' || direccionActual == 'f') && posibleDestino == *(estado->tablero))
                moviValido = 1;
            else if ((direccionActual == 'B' || direccionActual == 'b') && posibleDestino == (*(estado->tablero))->ant)
                moviValido = 1;
            if (*(char*)(posibleDestino->info) == 'B')
                moviValido = 1;
        }
        if(moviValido == 0){
            nodoDestino = posibleDestino;
            pasosRestantes--;
        }
    }

    // Aplicamos el movimiento visual
    if (mov->tipoEntidad == 'J') {
        *(char*)(nodoActual->info) = estado->terrenoBajoJugador;
        char destino = *(char*)(nodoDestino->info);

        if (destino == 'B') {
            estado->colisionBandido = 1;
            estado->terrenoBajoJugador = '.';
        } else {
            estado->terrenoBajoJugador = destino;
        }
        *(char*)(nodoDestino->info) = 'J';
    }
    else { // Bandidos
        if (*(char*)(nodoDestino->info) == 'J') {
            estado->colisionBandido = 1;
            *(char*)(nodoActual->info) = *(estado->terrenoBajoBandido + offset);
        }
        else if (nodoDestino != nodoActual) {
            *(char*)(nodoActual->info) = *(estado->terrenoBajoBandido + offset);
            *(estado->terrenoBajoBandido + offset) = *(char*)(nodoDestino->info);
            *(char*)(nodoDestino->info) = 'B';
        }
    }
}


int verificarEstadoTurno(tEstadoJuego* estado, int jugadorSeMovio) {
    char terreno = estado->terrenoBajoJugador;

    //  Llego a Refugio (S)
    if (terreno == 'S') {
        printf("\nFelicidades! Has llegado a la Ciudad Refugio.\n");
        return 1;
    }

    // Atrapado por bandido (B)
    if (estado->colisionBandido) {
        estado->colisionBandido = 0;

        if (estado->jugadorProtegido) {
            printf("\nUn bandido intento atraparte, pero la proteccion del Oasis te salvo!\n");
        } else {
            estado->vidasActuales--;
            printf("\nUn bandido te ha interceptado! Pierdes una vida. Te quedan %d vidas.\n", estado->vidasActuales);

            tNodoDob* nodoActual = buscarNodoEntidad((estado->tablero), 'J', 0);
            moverJugadorAInicio(estado->tablero, nodoActual, estado);

            if (estado->vidasActuales <= 0) return 1;
            return 0;
        }
    }

    // --- EFECTOS DE TERRENO
    if (jugadorSeMovio) {
        // Tormenta de Arena (T)
        if (terreno == 'T') {
            if (estado->jugadorProtegido) printf("\nAtraviesas una tormenta, pero tu proteccion anula el efecto.\n");
            else {
                printf("\nHas caido en una tormenta de arena! Pierdes tu proximo turno.\n");
                estado->perdioTurno = 1;
            }
        }

        //  Premios (P)
        if (terreno == 'P') {
            estado->puntosActuales++;
            printf("\nHas capturado un Premio! Puntos actuales: %d.\n", estado->puntosActuales);
            estado->terrenoBajoJugador = '.';
        }

        //  Vidas Extra (V)
        if (terreno == 'V') {
            estado->vidasActuales++;
            printf("\nHas capturado una Vida Extra! Vidas actuales: %d.\n", estado->vidasActuales);
            estado->terrenoBajoJugador = '.';
        }

        //  Oasis (O)
        if (terreno == 'O') {
            printf("\nHas llegado a un Oasis! Obtienes proteccion para el turno siguiente.\n");
            estado->jugadorProtegido = 1;
        } else {
            estado->jugadorProtegido = 0;
        }
    }
    else {
        // Si no se movió (ej. por perder el turno), la protección del oasis se termina
        estado->jugadorProtegido = 0;
    }

    return 0;
}

void mostrarHistorialMovimientos() {
    printf("\n--- REGISTRO DE MOVIMIENTOS ---\n");
    FILE* arch = fopen(ARCH_MOVIMIENTOS, "rt");
    if (!arch) {
        printf("No hay movimientos registrados.\n");
        return;
    }

    char linea[TAM_LINEA_MOV];
    while (fgets(linea, TAM_LINEA_MOV, arch)) {
        printf("%s", linea);
    }
    fclose(arch);
    printf("-------------------------------\n");
}

void bucleJuego(tEstadoJuego* estado, tConfig* config) {
    int finPartida = 0;
    int jugadorSeMovio, dadoJ;
    char dirJ;
    tMovimiento movJugador;
    tPosiciones pos;

    if (definirPosiciones(&pos, ARCH_CONFIG) != EXITO) {
        printf("Error al cargar memoria para los bandidos.\n");
        return;
    }

    while (!finPartida) {
        jugadorSeMovio = 0;

        // ENCOLAMOS LOS MOVIMIENTOS
        if (estado->perdioTurno) {
            printf("\nPierdes este turno debido a la tormenta de arena.\n");
            estado->perdioTurno = 0;
            jugadorSeMovio = 0;
        }

        else {
            dadoJ = tirar_dado(CARAS_DADO);
            printf("\nSacaste un %d. Hacia donde te moves? (F: Adelante, B: Atras): ", dadoJ);
            fflush(stdin);
            scanf(" %c", &dirJ);
            movJugador.tipoEntidad = 'J';
            movJugador.id_entidad = 0;

            if (dirJ == 'f')
                dirJ = 'F';

            if (dirJ == 'b')
                dirJ = 'B';

            movJugador.direccion = dirJ;
            movJugador.pasos = dadoJ;

            aColar(estado->colaMovimientos, &movJugador, sizeof(tMovimiento));
            jugadorSeMovio = 1;
        }

        pos.cantBandidos = 0;
        pos.posActual = 0;
        actualizarPosiciones(estado->tablero, &pos);
        calcularMovimientos(&pos, estado->colaMovimientos);

        // DESENCOLAMOS Y MOVEMOS
        while (colaVacia(estado->colaMovimientos) == COLA_NO_VACIA) {
            tMovimiento movActual;
            outCola(estado->colaMovimientos, &movActual, sizeof(tMovimiento));
            aplicarMovimientoTablero(estado, &movActual);

            if (movActual.tipoEntidad == 'J') {
                registrarMovimientoHistorial(estado, movActual.direccion, movActual.pasos);
                estado->turnosJugados++;
            }
        }

        // VERIFICAMOS RESULTADO FINAL
        finPartida = verificarEstadoTurno(estado, jugadorSeMovio);

        // ACTUALIZAMOS PANTALLA
        if (!finPartida) {
            printf("\nPresiona cualquier tecla para continuar...\n");
            getch();
            system("cls");
            mostrarTablero(estado);
        }
    }

    mostrarHistorialMovimientos();
    free(pos.posBandidos);
}

void procesarInicioNuevaPartida(tEstadoJuego* estado, tConfig* config, tArbol* jugadores)
{
    int posicion;
    int i=0;
    tJugador jugadorTmp;
    int estadoBusqueda;
    tIndice nuevoIndice;
    FILE* fReg;
    char* pTerreno;
    tNodoDob* nodoInicio;

    printf("\n Ingrese su nombre (alias): ");
    fflush(stdin);
    scanf("%s", jugadorTmp.nombre);
    estadoBusqueda = BuscarNombreJugador(jugadorTmp.nombre, jugadores, cmp, &posicion);

    if(estadoBusqueda == NO_EXISTE)
    {
        jugadorTmp.id = contarNodosEnIndice(jugadores) + 1;
        jugadorTmp.partidasJugadas = 0;
        guardarJugador(&jugadorTmp, ARCH_JUGADORES);

        strcpy(nuevoIndice.clave, jugadorTmp.nombre);
        nuevoIndice.pos = jugadorTmp.id - 1;

        insertarEnIndice(jugadores, (char*)&nuevoIndice, sizeof(tIndice), cmp);
    }
    else
    {
        if(!buscarJugadorEnArchivo(ARCH_JUGADORES, posicion, &jugadorTmp))
        {
            printf("Error al buscar jugador en archivo.\n");
            return;
        }
    }

    // CARGAMOS EL ESTADO DE INICIO
    estado->jugador = jugadorTmp; //se guardan los datos del jugador
    estado->vidasActuales = config->vidasInicio; //se asignan las vidas desde el archivo config.txt
    estado->puntosActuales = 0;
    estado->turnosJugados = 0;
    estado->jugadorProtegido = 0;
    estado->perdioTurno = 0;
    estado->terrenoBajoJugador = 'I'; //el jugador inicia sobre el campamento inicial (I)
    estado->colisionBandido = 0;

    pTerreno = estado->terrenoBajoBandido;

    while (i <  MAX_BANDIDOS) {
        *pTerreno = '.'; //guardamos el punto en la dirección actual
        pTerreno++;      //avanzamos a la siguiente dirección de memoria
        i++;
    }

    //nos posicionamos en el primer nodo (el Campamento Inicial)
    nodoInicio = *(estado->tablero);
    if (nodoInicio) {
        *(char*)(nodoInicio->info) = 'J';
    }

    system("cls");
    printf("\n -------------------------------CARAVANA DEL DESIERTO-------------------------------\n");

    mostrarTablero(estado);
    system("cls");
    printf("\n -------------------------------CARAVANA DEL DESIERTO-------------------------------\n");
    printf("\nBienvenido %s, tu id es %d y has jugado %d partidas.\n", estado->jugador.nombre, estado->jugador.id, estado->jugador.partidasJugadas);
    printf("\nOprime cualquier tecla para comenzar a jugar...\n");
    getch();
    system("cls");

    mostrarTablero(estado);

    //limpiamos el registro de movimientos de partidas anteriores
    fReg = fopen(ARCH_MOVIMIENTOS, "wt");
    if(fReg)
        fclose(fReg);
}

/// FUNCIONES DE tPartida
int guardarPartida(tEstadoJuego* juegoActual,const char* nomArch)
{
    FILE* pPartidas;
    tPartida partidaActual;
    partidaActual.idJugador=juegoActual->jugador.id;
    partidaActual.puntosObtenidos = juegoActual->puntosActuales;

    pPartidas=fopen(nomArch,"ab");
    if(!pPartidas)
        return ERROR_ARCHIVO;

    fwrite(&partidaActual, sizeof(tPartida), 1, pPartidas);
    fclose(pPartidas);
    return 1;
}

int actualizarJugador(tEstadoJuego* estado,const char* nomArch)
{
    estado->jugador.partidasJugadas++;

    FILE* pf = fopen(nomArch, "r+b");
    if (!pf)
        return ERROR_ARCHIVO;

    fseek(pf, (estado->jugador.id - 1)*sizeof(tJugador), SEEK_SET);
    fwrite(&estado->jugador, sizeof(tJugador), 1, pf);
    fclose(pf);

    return EXITO;
}
