#include "..\headers\juego.h"

int cmpCaracteres(const void* entidad, const void* caracterEnNodo)
{
    char* aux1=(char*)entidad;
    char* aux2=(char*)caracterEnNodo;
    return (*aux1) - (*aux2);
}

int cmp(const void* d1,const void* d2)
{
    tIndice* auxIndice1=(tIndice*)d1;
    tIndice* auxIndice2=(tIndice*)d2;

    return strcmpi(auxIndice1->clave,auxIndice2->clave);
}


int tirar_dado(unsigned lados)
{
    srand(time(NULL));
    return rand() % lados + 1;
}


//void iniciarCaravanaDelDesierto()
//{
//    tListaDobCirc tablero;
//    tConfig configuracion;
//    tArbol indiceJugadores = NULL;
//    tCola colaMov;
//    tEstadoJuego estado;
//    crearLista(&tablero);
//    crearArbol(&indiceJugadores);
//    crearCola(&colaMov);
//
//    estado.tablero = &tablero;
//    estado.colaMovimientos = &colaMov;
//
//    //se baja el indice de jugadores
//    cargarIndiceJugadores(&indiceJugadores, ARCH_INDICE_JUGADORES,sizeof(tIndice));
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////
//    char opcion = sdl_menu(ctx);
//
//    while(opcion != '3') // '3' es SALIR en tu sdl_menu
//    {
//        if(opcion == '1') // '1' es NUEVA_PARTIDA
//        {
//            // ... (tu codigo actual de vaciar lista y creacion de archivo) ...
//
//            procesarInicioNuevaPartida(&estado, &configuracion, &indiceJugadores);
//
//            // Pasamos ctx al bucle de juego para poder dibujar y leer teclas
//            bucleJuego(&estado, &configuracion, ctx);
//
//            // ... (tu codigo de guardado) ...
//
//            // REEMPLAZO 2: En vez de mostrarPantallaFinPartida
//            sdl_pantallaFin(ctx, estado.jugador.nombre, estado.puntosActuales, estado.vidasActuales, estado.turnosJugados, estado.vidasActuales > 0);
//        }
//
//        if(opcion == '2') // '2' es TABLA_DE_PUNTAJES
//            mostrarRanking(); // (Podemos dejar esto en consola por ahora si no hiciste ranking en SDL)
//
//        opcion = sdl_menu(ctx);
//    }
////    char opcion = menuPrincipal(MENSAJE_MENU_PRINCIPAL, OPCIONES_MENU_PRINCIPAL);
////    while(opcion != SALIR)
////    {
////
////        if(opcion == NUEVA_PARTIDA)
////        {
////            se limpia el tablero de la partida anterior y la cola de movimientos
////            VaciarListaDobCirc(&tablero);
////            vaciarCola(&colaMov);
////            if(creacionArchivoCaravana(ARCH_CARAVANA, estado.tablero, &configuracion)==ERROR_ARCHIVO)
////            {
////                printf("\n Para jugar se necesita contar si o si con un archivo config.txt! Por favor, para intentar jugar cree dicho archivo con los siguientes parametros (en orden) :\n 1- cantidad_posiciones \n 2- vidas_inicio \n 3- maximo_bandidos \n 4- maxmo_premios \n 5- maximo_vidas_extra \n 6- maximo_oasis \n 7- maximo_tormentas");
////                getch();
////                return;
////            }
////            procesarInicioNuevaPartida(&estado, &configuracion, &indiceJugadores);
////            bucleJuego(&estado, &configuracion);
////
////            actualizarJugador(&estado, ARCH_JUGADORES);
////            guardarPartida(&estado, ARCH_PARTIDAS);
////            guardarIndiceJugadores(&indiceJugadores, ARCH_INDICE_JUGADORES);
////
////            mostrarPantallaFinPartida(&estado);
////        }
////
////        if(opcion == TABLA_DE_PUNTAJES)
////            mostrarRanking();
////
////        system("cls");
////        opcion = menuPrincipal(MENSAJE_MENU_PRINCIPAL, OPCIONES_MENU_PRINCIPAL);
////    }
//
//    vaciarArbol(&indiceJugadores);
//    //si sale del while es porque el jugador eligio salir del juego
//    system("cls");
//    puts("Muchas gracias por jugar! Vuelva pronto!");
//}

void iniciarCaravanaDelDesierto(tSDLCtx* ctx)
{
    tListaDobCirc tablero;
    tConfig configuracion;
    tArbol indiceJugadores = NULL;
    tCola colaMov;
    tEstadoJuego estado;
    crearLista(&tablero);
    crearArbol(&indiceJugadores);
    crearCola(&colaMov);

    estado.tablero = &tablero;
    estado.colaMovimientos = &colaMov;

    // Se baja el indice de jugadores
    cargarIndiceJugadores(&indiceJugadores, ARCH_INDICE_JUGADORES, sizeof(tIndice));

    //Usamos la interfaz de SDL para el menú, pero guardando la opción ingresada

    char opcion = sdl_menu(ctx);

    while(opcion != SALIR)
    {
        if(opcion == NUEVA_PARTIDA)
        {
            // Se limpia el tablero de la partida anterior y la cola de movimientos
            VaciarListaDobCirc(&tablero);
            vaciarCola(&colaMov);

            if(creacionArchivoCaravana(ARCH_CARAVANA, estado.tablero, &configuracion) == ERROR_ARCHIVO)
            {
                //Mostramos el error en la ventana de SDL
                sdl_mostrarMensajeContextual(ctx, "ERROR CRITICO: CONFIG.TXT",
                                             "Falta el archivo de configuracion. Cree config.txt para jugar. Pulse Esc", 1);

                // Forzamos un renderizado rápido para asegurarnos de que el cartel se pinte antes de salir
                sdl_limpiar(ctx);
                sdl_renderizarOverlayEvento(ctx);
                sdl_presentar(ctx);

                // Esperamos que el usuario asimile el error y toque una flecha/tecla para salir de la partida
                sdl_esperarDireccion(ctx);

                // Vaciamos el árbol para no dejar fugas de memoria y abortamos la función de forma limpia
                vaciarArbol(&indiceJugadores);
                return;
            }
//            if(creacionArchivoCaravana(ARCH_CARAVANA, estado.tablero, &configuracion) == ERROR_ARCHIVO)
//            {
//                printf("\n Para jugar se necesita contar si o si con un archivo config.txt! Por favor, para intentar jugar cree dicho archivo con los siguientes parametros (en orden) :\n 1- cantidad_posiciones \n 2- vidas_inicio \n 3- maximo_bandidos \n 4- maxmo_premios \n 5- maximo_vidas_extra \n 6- maximo_oasis \n 7- maximo_tormentas");
//                getch(); // Asumimos que la consola de fondo sigue activa para esto
//                return;
//            }

            procesarInicioNuevaPartida(&estado, &configuracion, &indiceJugadores, ctx);

            // Pasamos el contexto SDL al bucle para que allí se dibuje y se lean teclas
            bucleJuego(&estado, &configuracion, ctx);

            actualizarJugador(&estado, ARCH_JUGADORES);
            guardarPartida(&estado, ARCH_PARTIDAS);
            guardarIndiceJugadores(&indiceJugadores, ARCH_INDICE_JUGADORES);

            // Fin de partida gráfico con SDL
            // Evaluamos si ganó chequeando si le quedaron vidas
            int gano = (estado.vidasActuales > 0) ? 1 : 0;
            sdl_pantallaFin(ctx, estado.jugador.nombre, estado.puntosActuales, estado.vidasActuales, estado.turnosJugados, gano);

            //Mostramos el historial antes de volver al menú
            sdl_mostrarHistorial(ctx, ARCH_MOVIMIENTOS);
        }

        if(opcion == TABLA_DE_PUNTAJES)
        {
            mostrarRanking(ctx);
        }

        // Ya no hacemos system("cls") acá porque el renderer de SDL limpia la pantalla solo
        opcion = sdl_menu(ctx);
    }

    vaciarArbol(&indiceJugadores);
    system("cls");
    puts("Muchas gracias por jugar! Vuelva pronto!");
}


void aplicarMovimientoTablero(tEstadoJuego* estado, tMovimiento* mov,tPosiciones* pos)
{
    tNodoDob*nodoActual,* nodoDestino,*aux;
    int offset,i,nuevaPos;
    char entidadBuscada;

    if (!estado->tablero || !*(estado->tablero)) // nos pasaron un null o no existe el tablero
        return;

    //asignacion de contenido a variables;
    nodoActual=NULL;
    offset=0;
    i=0;
    nuevaPos=0;

    if (mov->tipoEntidad == 'J')
    {   entidadBuscada='J';
        if(!(nodoActual = buscarNodoEntidad(estado->tablero, &entidadBuscada,cmpCaracteres)))
            return;

    }
    else
    {
        offset = mov->id_entidad - 1;
        nodoActual = *(estado->tablero); //se le asigna al primer nodo de la lista
        while(i<mov->posOrigen)// te posicionas en donde esta el bandido
        {
            nodoActual = nodoActual->sig;
            i++;
        }
    }



    if (mov->tipoEntidad == 'B' && *(char*)(nodoActual->info) != 'B')
    {
        return;
    }

    nodoDestino = calcularNodoDestino(estado,mov,nodoActual);

    // se actualiza la memoria de los bandidos
    if (mov->tipoEntidad == 'B')
    {

        aux = *(estado->tablero);


        while(aux != nodoDestino)
        {
            aux = aux->sig;
            nuevaPos++;
        }
        *(pos->posBandidos + offset) = nuevaPos;
    }

    // Aplicamos el movimiento visual tanto del nodoDestino como dejar libre el nodoActual
    movimientoVisual(estado,mov,nodoActual,nodoDestino,offset);


}


tNodoDob* calcularNodoDestino(tEstadoJuego* estado, tMovimiento* mov, tNodoDob* nodoActual)
{
    int pasosRestantes = mov->pasos,moviValido = 0;
    char direccionActual = mov->direccion;
    tNodoDob* nodoDestino = nodoActual,*posibleDestino;

    while (pasosRestantes > 0 && moviValido == 0)
    {
        if (toUpper(direccionActual) == 'F')
            posibleDestino = nodoDestino->sig;
        else
            posibleDestino = nodoDestino->ant;

        // LOGICA DE JUGADOR
        if (mov->tipoEntidad == 'J')
        {

            if (toUpper(direccionActual) == 'F' && posibleDestino == *(estado->tablero))
            {
                direccionActual = 'B';
                posibleDestino = nodoDestino->ant;
            }
            else if (toUpper(direccionActual) == 'B' && posibleDestino == (*(estado->tablero))->ant)
            {
                moviValido = 1;
            }
        }
        // Si el movimiento es válido, avanza un casillero
        if (moviValido == 0)
        {
            nodoDestino = posibleDestino;
            pasosRestantes--;
        }
    }
    return nodoDestino;
}

void movimientoVisual(tEstadoJuego* estado,tMovimiento* mov,tNodoDob* nodoActual,tNodoDob* nodoDestino, int offset)
{
    if (mov->tipoEntidad == 'J')
    {
        *(char*)(nodoActual->info) = estado->terrenoBajoJugador;

        if (*(char*)(nodoDestino->info) == 'B')
        {
            estado->colisionBandido = 1;
            estado->terrenoBajoJugador = '.';
        }
        else
        {
            estado->terrenoBajoJugador = *(char*)(nodoDestino->info);
        }
        *(char*)(nodoDestino->info) = 'J';
    }
    else
    {
        if (nodoDestino != nodoActual)
        {

            if (*(char*)(nodoDestino->info) == 'J')
            {
                estado->colisionBandido = 1;
                *(char*)(nodoActual->info)= *(estado->terrenoBajoBandido + offset);
                *(estado->terrenoBajoBandido + offset)= estado->terrenoBajoJugador;
            }
            else
            {
                // Limpia la huella
                *(char*)(nodoActual->info) = *(estado->terrenoBajoBandido + offset);

                if (*(char*)(nodoDestino->info) != 'B')
                {
                    *(estado->terrenoBajoBandido + offset) = *(char*)(nodoDestino->info);
                }
                else
                {
                    *(estado->terrenoBajoBandido + offset) = '.';
                }

                // Se dibuja en el nuevo casillero
                *(char*)(nodoDestino->info) = 'B';
            }
        }
    }
}

int verificarEstadoTurno(tEstadoJuego* estado, int jugadorSeMovio, tPosiciones* pos, tSDLCtx* ctx)
{
    int indiceChoque,i=0,flagMarca=0;
    tNodoDob* nodoActual,*aux,*nodoInicio;
    char entidadBuscada;
    char msgAux[128];

    if (estado->terrenoBajoJugador == 'S')
        return 1;

    if (estado->colisionBandido)
    {
        estado->colisionBandido = 0;

        if (estado->jugadorProtegido)
        {
            sdl_mostrarMensajeContextual(ctx, "PROTECCION DIVINA", "Un bandido intento atraparte, pero el Oasis te salvo!", 0);
        }
        else
        {
            estado->vidasActuales--;
            sprintf(msgAux, "Un bandido te ha interceptado! Te quedan %d vidas.", estado->vidasActuales);
            sdl_mostrarMensajeContextual(ctx, "ATAQUE BANDIDO", msgAux, 1);

            entidadBuscada='J';
            nodoActual = buscarNodoEntidad((estado->tablero),&entidadBuscada,cmpCaracteres);
            if(nodoActual != NULL)
            {
                indiceChoque = 0;
                aux = *(estado->tablero);
                while (aux != nodoActual)
                {
                    aux = aux->sig;
                    indiceChoque++;
                }

                while(i<pos->cantBandidos && flagMarca==0)
                {
                    if (*(pos->posBandidos + i) == indiceChoque)
                    {
                        *(pos->posBandidos + i) = -1;
                        flagMarca=1;
                    }
                    i++;
                }
                moverJugadorAInicio(estado->tablero, nodoActual, estado);
            }
            else
            {
                nodoInicio = *(estado->tablero);
                estado->terrenoBajoJugador = *(char*)(nodoInicio->info);
                *(char*)(nodoInicio->info) = 'J';
            }
        }
    }

    if (jugadorSeMovio)
    {
        if (estado->terrenoBajoJugador == 'T')
        {
            if (estado->jugadorProtegido)
                sdl_mostrarMensajeContextual(ctx, "TORMENTA DE ARENA", "Atraviesas una tormenta, pero tu escudo te protege.", 0);
            else
            {
                sdl_mostrarMensajeContextual(ctx, "TORMENTA DE ARENA", "Has caido en una tormenta! Pierdes tu proximo turno.", 1);
                estado->perdioTurno = 1;
            }
        }
        if (estado->terrenoBajoJugador == 'P')
        {
            estado->puntosActuales++;
            sprintf(msgAux, "Has capturado un Premio! Puntos actuales: %d.", estado->puntosActuales);
            sdl_mostrarMensajeContextual(ctx, "PREMIO ENCONTRADO", msgAux, 0);
            estado->terrenoBajoJugador = '.';
        }
        if (estado->terrenoBajoJugador == 'V')
        {
            estado->vidasActuales++;
            sprintf(msgAux, "Has capturado una Vida Extra! Vidas actuales: %d.", estado->vidasActuales);
            sdl_mostrarMensajeContextual(ctx, "VIDA EXTRA", msgAux, 0);
            estado->terrenoBajoJugador = '.';
        }
        if (estado->terrenoBajoJugador == 'O')
        {
            sdl_mostrarMensajeContextual(ctx, "OASIS", "Has llegado a un Oasis! Obtienes inmunidad el proximo turno.", 0);
            estado->jugadorProtegido = 1;
        }
        else
            estado->jugadorProtegido = 0;
    }
    else
        estado->jugadorProtegido = 0;

    if (estado->vidasActuales <= 0) return TERMINO_PARTIDA;

    return SIGUE_PARTIDA;
}

void mostrarHistorialMovimientos()
{
    printf("\n--- REGISTRO DE MOVIMIENTOS ---\n");
    FILE* arch = fopen(ARCH_MOVIMIENTOS, "rt");
    if (!arch)
    {
        printf("No hay movimientos registrados.\n");
        return;
    }

    char linea[TAM_LINEA_MOV];
    while (fgets(linea, TAM_LINEA_MOV-1, arch))
    {
        printf("%s", linea);
    }
    fclose(arch);
    printf("-------------------------------\n");
}

//void bucleJuego(tEstadoJuego* estado, tConfig* config)
//{
//    int finPartida = 0,jugadorSeMovio, dadoJ,huboError=0,indiceCasillero;
//    char dirJ;
//    tMovimiento movJugador, movActual;
//    tPosiciones pos;
//    tNodoDob* buscador;
//    if (definirPosiciones(&pos, ARCH_CONFIG) != EXITO)
//    {
//        printf("Error al cargar memoria para los bandidos.\n");
//        return;
//    }
//     escaner inicial para los bandidos
//    actualizarPosiciones(estado->tablero, &pos);
//
//    while (!finPartida)
//    {
//        jugadorSeMovio = 0;
//
//         ENCOLAMOS LOS MOVIMIENTOS
//        if (estado->perdioTurno)
//        {
//            printf("\nPierdes este turno debido a la tormenta de arena.\n");
//            estado->perdioTurno = 0;
//            jugadorSeMovio = 0;
//        }
//
//        else
//        {
//
//            dadoJ = tirar_dado(CARAS_DADO);
//            do
//            {
//                if (huboError == 1)
//                {
//                    system("cls");
//                    mostrarTablero(estado,&pos); // Vuelve a imprimir el mapa y el HUD actualizados
//                    printf("\n [!] Error: Direccion invalida. Por favor, ingresa 'F' o 'B'.\n");
//                }
//                printf("\nSacaste un %d. Hacia donde te moves? (F: Adelante, B: Atras): ", dadoJ);
//                fflush(stdin);
//                scanf(" %c", &dirJ);
//                dirJ= toUpper(dirJ); // si es minuscula la pasa a mayuscula y si es mayuscula la mantiene
//
//                if (dirJ != 'F' && dirJ != 'B')
//                    huboError=1;
//                else
//                    huboError=0;
//
//            }while(dirJ!='F' && dirJ != 'B');
//
//            movJugador.tipoEntidad = 'J';
//            movJugador.id_entidad = 0;
//
//
//            movJugador.direccion = dirJ;
//            movJugador.pasos = dadoJ;
//
//            aColar(estado->colaMovimientos, &movJugador, sizeof(tMovimiento));
//            jugadorSeMovio = 1;
//        }
//
//        buscador = *(estado->tablero);
//        indiceCasillero = 0;
//        do
//        {
//            if (*(char*)(buscador->info) == 'J')
//                pos.posJugador = indiceCasillero;
//            buscador = buscador->sig;
//            indiceCasillero++;
//        }while (buscador != *(estado->tablero));
//
//        calcularMovimientos(&pos, estado->colaMovimientos);
//
//         DESENCOLAMOS Y MOVEMOS
//        while (colaVacia(estado->colaMovimientos) == COLA_NO_VACIA)
//        {
//
//            outCola(estado->colaMovimientos, &movActual, sizeof(tMovimiento));
//            aplicarMovimientoTablero(estado, &movActual,&pos);
//
//            if (movActual.tipoEntidad == 'J')
//            {
//                registrarMovimientoHistorial(estado, movActual.direccion, movActual.pasos);
//                estado->turnosJugados++;
//            }
//        }
//        sincronizarBandidosApilados(estado,&pos);
//         VERIFICAMOS RESULTADO FINAL
//        finPartida = verificarEstadoTurno(estado, jugadorSeMovio,&pos);
//
//         ACTUALIZAMOS PANTALLA
//        if (finPartida==SIGUE_PARTIDA)
//        {
//            printf("\nPresiona cualquier tecla para continuar...\n");
//            getch();
//            system("cls");
//            mostrarTablero(estado,&pos);
//        }
//    }
//
//    mostrarHistorialMovimientos();
//    free(pos.posBandidos);
//}

void sincronizarBandidosApilados(tEstadoJuego* estado, tPosiciones* pos)
{
    int *pBandido,*pFinBandidos,posActual = 0;
    char* pTerreno;
    tNodoDob* actual;

    if (!estado->tablero || !*(estado->tablero))
        return;

    pBandido = pos->posBandidos;
    pFinBandidos = (pos->posBandidos + pos->cantBandidos);
    pTerreno = estado->terrenoBajoBandido;

    while (pBandido < pFinBandidos)
    {
        if(*pBandido != -1)
        {
            actual = *(estado->tablero);
            posActual = 0;
            while (posActual < *pBandido)
            {
                actual = actual->sig;
                posActual++;
            }

            if (*(char*)(actual->info) != 'B' && *(char*)(actual->info)!= 'J')
            {
                *pTerreno = *(char*)(actual->info);
                *(char*)(actual->info) = 'B';
            }

        }
        pBandido++;
        pTerreno++;
    }
}

void bucleJuego(tEstadoJuego* estado, tConfig* config, tSDLCtx* ctx)
{
    int finPartida = 0, jugadorSeMovio, dadoJ, indiceCasillero;
    char dirJ;
    tMovimiento movJugador, movActual;
    tPosiciones pos;
    tNodoDob* buscador;

    if (definirPosiciones(&pos, ARCH_CONFIG) != EXITO) return;

    actualizarPosiciones(estado->tablero, &pos);

    // Dibujo inicial
    sdl_limpiar(ctx);
    mostrarTablero(estado, &pos, ctx);
    sdl_presentar(ctx);

    while (!finPartida)
    {
        jugadorSeMovio = 0;

        if (estado->perdioTurno)
        {
            sdl_mostrarMensajeContextual(ctx, "INMOVILIZADO", "Pierdes este turno debido a la tormenta.", 1);
            estado->perdioTurno = 0;
            jugadorSeMovio = 0;

            // Dibuja el cartel de perder turno y espera un poquito

        }
        else
        {
            dadoJ = tirar_dado(CARAS_DADO);

            // Dibuja tablero, cartel de dado, y si había un evento (ej. ganar vida) lo muestra acá
            sdl_limpiar(ctx);
            mostrarTablero(estado, &pos, ctx);
            sdl_dibujarMensajeDado(ctx, dadoJ);
            sdl_renderizarOverlayEvento(ctx);
            sdl_presentar(ctx);

            dirJ = sdl_esperarDireccion(ctx); // Al elegir flecha, el cartel se apaga solo

            if (dirJ == 'Q') {
                finPartida = TERMINO_PARTIDA;
                break;
            }

            movJugador.tipoEntidad = 'J';
            movJugador.id_entidad = 0;
            movJugador.direccion = dirJ;
            movJugador.pasos = dadoJ;

            aColar(estado->colaMovimientos, &movJugador, sizeof(tMovimiento));
            jugadorSeMovio = 1;
        }

        buscador = *(estado->tablero);
        indiceCasillero = 0;
        do {
            if (*(char*)(buscador->info) == 'J') pos.posJugador = indiceCasillero;
            buscador = buscador->sig;
            indiceCasillero++;
        } while (buscador != *(estado->tablero));

        calcularMovimientos(&pos, estado->colaMovimientos);

        while (colaVacia(estado->colaMovimientos) == COLA_NO_VACIA)
        {
            outCola(estado->colaMovimientos, &movActual, sizeof(tMovimiento));
            aplicarMovimientoTablero(estado, &movActual, &pos);

            if (movActual.tipoEntidad == 'J') {
                registrarMovimientoHistorial(estado, movActual.direccion, movActual.pasos);
                estado->turnosJugados++;
            }
        }
        sincronizarBandidosApilados(estado, &pos);

        // Acá evalúa todo y arma los carteles si pasó algo
        finPartida = verificarEstadoTurno(estado, jugadorSeMovio, &pos, ctx);

        // Actualizamos para que se vea el movimiento del tablero + el cartel nuevo
        sdl_limpiar(ctx);
        mostrarTablero(estado, &pos, ctx);
        sdl_renderizarOverlayEvento(ctx);
        sdl_presentar(ctx);

        if (finPartida == SIGUE_PARTIDA)
        {
            // Si el jugador no se movió, significa que estuvo inmovilizado
            // Le damos mucho más tiempo de pausa para que lea el cartel y vea qué hicieron los enemigos.
            if (jugadorSeMovio == 0) {
                SDL_Delay(2500); // 2.5 segundos
            } else {
                SDL_Delay(800);  // Turno normal
            }
        }
    }

    free(pos.posBandidos);
}
//
//void procesarInicioNuevaPartida(tEstadoJuego* estado, tConfig* config, tArbol* jugadores, tSDLCtx* ctx)
//{
//    int posicion,i=0,estadoBusqueda;
//    tJugador jugadorTmp;
//    tIndice nuevoIndice;
//    FILE* fReg;
//    char* pTerreno;
//    tNodoDob* nodoInicio;
//
//    memset(&jugadorTmp, 0, sizeof(tJugador)); // setea al jugador todo en 0
//
//    printf("\n Ingrese su nombre (alias): ");
//    fflush(stdin);
//    scanf("%s", jugadorTmp.nombre);
//    estadoBusqueda = BuscarNombreJugador(jugadorTmp.nombre, jugadores, cmp, &posicion);
//
//    if(estadoBusqueda == NO_EXISTE)
//    {
//        jugadorTmp.id = contarNodosEnIndice(jugadores) + 1;
//        jugadorTmp.partidasJugadas = 0;
//        guardarJugador(&jugadorTmp, ARCH_JUGADORES,jugadores,cmp);
//
//        strcpy(nuevoIndice.clave, jugadorTmp.nombre);
//        nuevoIndice.pos = jugadorTmp.id - 1;
//
//        insertarEnIndice(jugadores, (char*)&nuevoIndice, sizeof(tIndice), cmp);
//    }
//    else
//    {
//        if(!buscarJugadorEnArchivo(ARCH_JUGADORES, posicion, &jugadorTmp))
//        {
//            printf("Error al buscar jugador en archivo.\n");
//            return;
//        }
//    }
//
//    // CARGAMOS EL ESTADO DE INICIO
//    estado->jugador = jugadorTmp; //se guardan los datos del jugador
//    estado->vidasActuales = config->vidasInicio; //se asignan las vidas desde el archivo config.txt
//    estado->puntosActuales = 0;
//    estado->turnosJugados = 0;
//    estado->jugadorProtegido = 0;
//    estado->perdioTurno = 0;
//    estado->terrenoBajoJugador = 'I'; //el jugador inicia sobre el campamento inicial (I)
//    estado->colisionBandido = 0;
//
//    pTerreno = estado->terrenoBajoBandido;
//
//    while (i <  MAX_BANDIDOS)
//    {
//        *pTerreno = '.'; //guardamos el punto en la dirección actual
//        pTerreno++;      //avanzamos a la siguiente dirección de memoria
//        i++;
//    }
//
//    //nos posicionamos en el primer nodo (el Campamento Inicial)
//    nodoInicio = *(estado->tablero);
//    if (nodoInicio)
//    {
//        *(char*)(nodoInicio->info) = 'J';
//    }
//
//    system("cls");
//    printf("\n -------------------------------CARAVANA DEL DESIERTO-------------------------------\n");
//    printf("\nBienvenido %s, tu id es %d y has jugado %d partidas.\n", estado->jugador.nombre, estado->jugador.id, estado->jugador.partidasJugadas);
//    printf("\nOprime cualquier tecla para comenzar a jugar...\n");
//    getch();
//    system("cls");
//
//    mostrarTablero(estado, NULL, ctx);
//
//    //limpiamos el registro de movimientos de partidas anteriores
//    fReg = fopen(ARCH_MOVIMIENTOS, "wt");
//    if(fReg)
//        fclose(fReg);
//}

void procesarInicioNuevaPartida(tEstadoJuego* estado, tConfig* config, tArbol* jugadores, tSDLCtx* ctx)
{
    int posicion,i=0,estadoBusqueda;
    tJugador jugadorTmp;
    tIndice nuevoIndice;
    FILE* fReg;
    char* pTerreno;
    tNodoDob* nodoInicio;

    memset(&jugadorTmp, 0, sizeof(tJugador)); // setea al jugador todo en 0

    //Ingreso de texto en modo grafico
    sdl_pedirNombre(ctx, jugadorTmp.nombre, sizeof(jugadorTmp.nombre));

    estadoBusqueda = BuscarNombreJugador(jugadorTmp.nombre, jugadores, cmp, &posicion);

    if(estadoBusqueda == NO_EXISTE)
    {
        jugadorTmp.id = contarNodosEnIndice(jugadores) + 1;
        jugadorTmp.partidasJugadas = 0;
        guardarJugador(&jugadorTmp, ARCH_JUGADORES,jugadores,cmp);

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

    while (i <  MAX_BANDIDOS)
    {
        *pTerreno = '.'; //guardamos el punto en la dirección actual
        pTerreno++;      //avanzamos a la siguiente dirección de memoria
        i++;
    }

    //nos posicionamos en el primer nodo (el Campamento Inicial)
    nodoInicio = *(estado->tablero);
    if (nodoInicio)
    {
        *(char*)(nodoInicio->info) = 'J';
    }

    // REEMPLAZO 2: Pantalla de Bienvenida Gráfica
    sdl_pantallaBienvenida(ctx, estado->jugador.nombre, estado->jugador.id, estado->jugador.partidasJugadas);

    mostrarTablero(estado, NULL, ctx);

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


