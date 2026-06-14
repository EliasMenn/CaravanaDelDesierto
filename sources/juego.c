#include "..\headers\juego.h"

void iniciarCaravanaDelDesierto()
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

    //se baja el indice de jugadores
    cargarIndiceJugadores(&indiceJugadores, ARCH_INDICE_JUGADORES,sizeof(tIndice));

    char opcion = menuPrincipal(MENSAJE_MENU_PRINCIPAL, OPCIONES_MENU_PRINCIPAL);
    while(opcion != SALIR)
    {
        //se limpia el tablero de la partida anterior

        if(opcion == NUEVA_PARTIDA)
        {
            VaciarListaDobCirc(&tablero);
            vaciarCola(&colaMov);
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

    vaciarArbol(&indiceJugadores);
    //si sale del while es porque el jugador eligio salir del juego
    system("cls");
    puts("Muchas gracias por jugar! Vuelva pronto!");
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

void aplicarMovimientoTablero(tEstadoJuego* estado, tMovimiento* mov,tPosiciones* pos)
{
    tNodoDob*nodoActual,* nodoDestino,*aux;
    int offset,i,nuevaPos;


    if (!estado->tablero || !*(estado->tablero)) // nos pasaron un null o no existe el tablero
        return;

    //asignacion de datos a variables;
    nodoActual=NULL;
    offset=0;
    i=0;
    nuevaPos=0;

    if (mov->tipoEntidad == 'J')
    {
        if(!(nodoActual = buscarNodoEntidad(estado->tablero, 'J')))
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



int verificarEstadoTurno(tEstadoJuego* estado, int jugadorSeMovio,tPosiciones* pos)
{
    int indiceChoque,i=0,flagMarca=0;
    tNodoDob* nodoActual,*aux;
    if (estado->terrenoBajoJugador == 'S')
    {
        printf("\nFelicidades! Has llegado a la Ciudad Refugio.\n");
        return 1;
    }

    // CASO BANDIDO
    if (estado->colisionBandido)
    {
        estado->colisionBandido = 0;

        if (estado->jugadorProtegido)
        {
            printf("\nUn bandido intento atraparte, pero la proteccion del Oasis te salvo!\n");
        }
        else
        {
            estado->vidasActuales--;
            printf("\nUn bandido te ha interceptado! Pierdes una vida. Te quedan %d vidas.\n", estado->vidasActuales);

            nodoActual = buscarNodoEntidad((estado->tablero), 'J');
            if(nodoActual != NULL)
            {

                indiceChoque = 0;
                aux = *(estado->tablero);
                while (aux != nodoActual)
                {
                    aux = aux->sig;
                    indiceChoque++;
                }

                // Lo marcamos con -1 para que el juego sepa que está muerto
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
                // Si por algún error visual la 'J' desapareció, la forzamos a reaparecer en el inicio
                tNodoDob* nodoInicio = *(estado->tablero);
                estado->terrenoBajoJugador = *(char*)(nodoInicio->info);
                *(char*)(nodoInicio->info) = 'J';
            }

            // Identificamos la posicion del bandido a eliminar
            if (estado->vidasActuales <= 0)
                return 1;

            return 0;
        }
    }

    // --- EFECTOS DE TERRENO
    if (jugadorSeMovio)
    {

        if (estado->terrenoBajoJugador == 'T')
        {
            if (estado->jugadorProtegido)
                printf("\nAtraviesas una tormenta, pero tu proteccion anula el efecto.\n");
            else
            {
                printf("\nHas caido en una tormenta de arena! Pierdes tu proximo turno.\n");
                estado->perdioTurno = 1;
            }
        }


        if (estado->terrenoBajoJugador == 'P')
        {
            estado->puntosActuales++;
            printf("\nHas capturado un Premio! Puntos actuales: %d.\n", estado->puntosActuales);
            estado->terrenoBajoJugador = '.';
        }

        if (estado->terrenoBajoJugador == 'V')
        {
            estado->vidasActuales++;
            printf("\nHas capturado una Vida Extra! Vidas actuales: %d.\n", estado->vidasActuales);
            estado->terrenoBajoJugador = '.';
        }


        if (estado->terrenoBajoJugador == 'O')
        {
            printf("\nHas llegado a un Oasis! Obtienes proteccion para el turno siguiente.\n");
            estado->jugadorProtegido = 1;
        }
        else
            estado->jugadorProtegido = 0;
    }
    else
        estado->jugadorProtegido = 0;

    return 0;
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

void bucleJuego(tEstadoJuego* estado, tConfig* config)
{
    int finPartida = 0,jugadorSeMovio, dadoJ,huboError=0,indiceCasillero;
    char dirJ;
    tMovimiento movJugador, movActual;
    tPosiciones pos;
    tNodoDob* buscador;
    if (definirPosiciones(&pos, ARCH_CONFIG) != EXITO)
    {
        printf("Error al cargar memoria para los bandidos.\n");
        return;
    }
    // escaner inicial para los bandidos
    pos.cantBandidos = 0;
    pos.posActual = 0;
    actualizarPosiciones(estado->tablero, &pos);
    while (!finPartida)
    {
        jugadorSeMovio = 0;

        // ENCOLAMOS LOS MOVIMIENTOS
        if (estado->perdioTurno)
        {
            printf("\nPierdes este turno debido a la tormenta de arena.\n");
            estado->perdioTurno = 0;
            jugadorSeMovio = 0;
        }

        else
        {

            dadoJ = tirar_dado(CARAS_DADO);
            do
            {
                if (huboError == 1)
                {
                    system("cls");
                    mostrarTablero(estado,&pos); // Vuelve a imprimir el mapa y el HUD actualizados
                    printf("\n [!] Error: Direccion invalida. Por favor, ingresa 'F' o 'B'.\n");
                }
                printf("\nSacaste un %d. Hacia donde te moves? (F: Adelante, B: Atras): ", dadoJ);
                fflush(stdin);
                scanf(" %c", &dirJ);
                dirJ= toUpper(dirJ); // si es minuscula la pasa a mayuscula y si es mayuscula la mantiene

                if (dirJ != 'F' && dirJ != 'B')
                    huboError=1;
                else
                    huboError=0;

            }while(dirJ!='F' && dirJ != 'B');

            movJugador.tipoEntidad = 'J';
            movJugador.id_entidad = 0;


            movJugador.direccion = dirJ;
            movJugador.pasos = dadoJ;

            aColar(estado->colaMovimientos, &movJugador, sizeof(tMovimiento));
            jugadorSeMovio = 1;
        }

        buscador = *(estado->tablero);
        indiceCasillero = 0;
        do
        {
            if (*(char*)(buscador->info) == 'J')
                pos.posJugador = indiceCasillero;
            if (*(char*)(buscador->info) == 'I')
                pos.posI = indiceCasillero;

            buscador = buscador->sig;
            indiceCasillero++;
        }while (buscador != *(estado->tablero));

        calcularMovimientos(&pos, estado->colaMovimientos);

        // DESENCOLAMOS Y MOVEMOS
        while (colaVacia(estado->colaMovimientos) == COLA_NO_VACIA)
        {

            outCola(estado->colaMovimientos, &movActual, sizeof(tMovimiento));
            aplicarMovimientoTablero(estado, &movActual,&pos);

            if (movActual.tipoEntidad == 'J')
            {
                registrarMovimientoHistorial(estado, movActual.direccion, movActual.pasos);
                estado->turnosJugados++;
            }
        }
        sincronizarBandidosApilados(estado,&pos);
        // VERIFICAMOS RESULTADO FINAL
        finPartida = verificarEstadoTurno(estado, jugadorSeMovio,&pos);

        // ACTUALIZAMOS PANTALLA
        if (!finPartida)
        {
            printf("\nPresiona cualquier tecla para continuar...\n");
            getch();
            system("cls");
            mostrarTablero(estado,&pos);
        }
    }

    mostrarHistorialMovimientos();
    free(pos.posBandidos);
}

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

void procesarInicioNuevaPartida(tEstadoJuego* estado, tConfig* config, tArbol* jugadores)
{
    int posicion,i=0,estadoBusqueda;
    tJugador jugadorTmp;
    tIndice nuevoIndice;
    FILE* fReg;
    char* pTerreno;
    tNodoDob* nodoInicio;

    memset(&jugadorTmp, 0, sizeof(tJugador)); // setea al jugador todo en 0

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

    system("cls");
    printf("\n -------------------------------CARAVANA DEL DESIERTO-------------------------------\n");
    printf("\nBienvenido %s, tu id es %d y has jugado %d partidas.\n", estado->jugador.nombre, estado->jugador.id, estado->jugador.partidasJugadas);
    printf("\nOprime cualquier tecla para comenzar a jugar...\n");
    getch();
    system("cls");

    mostrarTablero(estado,NULL);

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
