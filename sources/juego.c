#include "..\headers\juego.h"

void aplicarMovimientoTablero(tEstadoJuego* estado, tMovimiento* mov) {
    if (!estado->tablero || !*(estado->tablero)) return;

    tNodoDob* nodoActual = buscarNodoEntidad((estado->tablero), mov->tipoEntidad, mov->id_entidad);
    if (!nodoActual) return; 

    tNodoDob* nodoDestino = nodoActual;
    int pasosRestantes = mov->pasos;
    char direccionActual = mov->direccion;

    while (pasosRestantes > 0) {
        if (direccionActual == 'F') nodoDestino = nodoDestino->sig;
        else nodoDestino = nodoDestino->ant;
        pasosRestantes--;

        if (mov->tipoEntidad == 'J' && contieneEntidad(nodoDestino, 'S') && pasosRestantes > 0) {
            direccionActual = (direccionActual == 'F') ? 'B' : 'F';
        }
    }

    if (mov->tipoEntidad == 'J') {
        *(char*)(nodoActual->info) = estado->terrenoBajoJugador;
        char destino = *(char*)(nodoDestino->info);
        
        // Si el jugador pisa a un bandido
        if (destino == 'B') {
            estado->colisionBandido = 1;
            estado->terrenoBajoJugador = '.'; // El bandido estaba sobre tierra vacía
        } else {
            estado->terrenoBajoJugador = destino;
        }
        *(char*)(nodoDestino->info) = 'J';
    } 
    else { // Movimiento de los Bandidos
        // Calculamos el desplazamiento de memoria (offset) en vez de usarlo como índice
        int offset = mov->id_entidad - 1; 

        // Si el bandido cae exactamente sobre el jugador
        if (*(char*)(nodoDestino->info) == 'J') {
            estado->colisionBandido = 1; 
            
            // Acceso con aritmética de punteros
            *(char*)(nodoActual->info) = *(estado->terrenoBajoBandido + offset); 
        } 
        else {
            // 1. Al irse, restaura el mapa con lo que había guardado
            *(char*)(nodoActual->info) = *(estado->terrenoBajoBandido + offset);
            
            // 2. Al llegar, memoriza lo que hay en el destino 
            *(estado->terrenoBajoBandido + offset) = *(char*)(nodoDestino->info);
            
            // 3. Se para encima visualmente
            *(char*)(nodoDestino->info) = 'B';
        }
    }
}


// Actualizá la firma en tu archivo .h para que acepte el int extra
int verificarEstadoTurno(tEstadoJuego* estado, int jugadorSeMovio) {
    char terreno = estado->terrenoBajoJugador;

    // A. Llegada a la Ciudad Refugio (S)
    if (terreno == 'S') {
        printf("\nFelicidades! Has llegado a la Ciudad Refugio.\n");
        return 1; 
    }

    // B. Intercepción de Bandido (B) usando la nueva bandera
    if (estado->colisionBandido) {
        estado->colisionBandido = 0; // Apagamos la alarma
        
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

    // --- EFECTOS DEL TERRENO (Solo aplican si pisó una casilla NUEVA este turno) ---
    if (jugadorSeMovio) {
        // C. Tormenta de Arena (T) -> PERSISTE
        if (terreno == 'T') {
            if (estado->jugadorProtegido) printf("\nAtraviesas una tormenta, pero tu proteccion anula el efecto.\n");
            else {
                printf("\nHas caido en una tormenta de arena! Pierdes tu proximo turno.\n");
                estado->perdioTurno = 1; 
            }
        }

        // D. Premios (P) -> SE CONSUME
        if (terreno == 'P') {
            estado->puntosActuales++; 
            printf("\nHas capturado un Premio! Puntos actuales: %d.\n", estado->puntosActuales);
            estado->terrenoBajoJugador = '.'; 
        }

        // E. Vidas Extra (V) -> SE CONSUME
        if (terreno == 'V') {
            estado->vidasActuales++; 
            printf("\nHas capturado una Vida Extra! Vidas actuales: %d.\n", estado->vidasActuales);
            estado->terrenoBajoJugador = '.'; 
        }

        // F. Oasis (O) -> PERSISTE
        if (terreno == 'O') {
            printf("\nHas llegado a un Oasis! Obtienes proteccion para el turno siguiente.\n");
            estado->jugadorProtegido = 1; 
        } else {
            estado->jugadorProtegido = 0;
        }
    } 
    else {
        // Si no se movió (ej. por perder el turno), la protección del oasis se agota
        estado->jugadorProtegido = 0;
    }

    return 0;
}

void mostrarHistorialMovimientos() {
    printf("\n--- REGISTRO DE MOVIMIENTOS ---\n");
    FILE* arch = fopen("registro_movimientos.txt", "rt");
    if (!arch) {
        printf("No hay movimientos registrados.\n");
        return;
    }
    
    char linea[10];
    while (fgets(linea, sizeof(linea), arch)) {
        printf("%s", linea);
    }
    fclose(arch);
    printf("-------------------------------\n");
}

void bucleJuego(tEstadoJuego* estado, tConfig* config) {
    int finPartida = 0;
    tPosiciones pos;
    if (definirPosiciones(&pos, "config.txt") != EXITO) {
        printf("Error al cargar memoria para los bandidos.\n");
        return; 
    }

    while (!finPartida) {
        int jugadorSeMovio = 0; // NUEVO: Bandera de turno

        // --- FASE 1: ENCOLAR MOVIMIENTOS ---
        if (estado->perdioTurno) {
            printf("\nPierdes este turno debido a la tormenta de arena.\n");
            estado->perdioTurno = 0; 
            jugadorSeMovio = 0; // No se movió
        } else {
            int dadoJ = tirarDado(); 
            char dirJ;
            printf("\nSacaste un %d. Hacia donde te moves? (F: Adelante, B: Atras): ", dadoJ);
            fflush(stdin);
            scanf(" %c", &dirJ);
            
            tMovimiento movJugador = {'J', 0, dirJ, dadoJ};
            aColar(estado->colaMovimientos, &movJugador, sizeof(tMovimiento));
            jugadorSeMovio = 1; // Sí se movió
        }
        
        pos.cantBandidos = 0; 
        actualizarPosiciones(estado->tablero, &pos);
        calcularMovimientos(&pos, estado->colaMovimientos);

        // --- FASE 2: DESENCOLAR Y APLICAR ---
        while (!colaVacia(estado->colaMovimientos)) {
            tMovimiento movActual;
            desAcolar(estado->colaMovimientos, &movActual, sizeof(tMovimiento));
            aplicarMovimientoTablero(estado, &movActual); 
            
            if (movActual.tipoEntidad == 'J') {
                registrarMovimientoHistorial(estado, movActual.direccion, movActual.pasos); 
                estado->turnosJugados++;
            }
        }

        // --- FASE 3: VERIFICAR EFECTOS ---
        // Le pasamos la bandera para saber si evaluar el terreno
        finPartida = verificarEstadoTurno(estado, jugadorSeMovio);

        // --- FASE 4: ACTUALIZAR PANTALLA ---
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
    int posicion,i=0;
    tJugador jugadorTmp;

    printf("\n Ingrese su nombre (alias) :");
    fflush(stdin);
    scanf("%s", jugadorTmp.nombre);
    
    posicion = BuscarNombreJugador(jugadorTmp.nombre, jugadores);

    if(posicion == NO_EXISTE)
    {
        jugadorTmp.id = contarNodosEnIndice(jugadores) + 1; 
        jugadorTmp.partidasJugadas = 0;
        guardarJugador(&jugadorTmp, ARCH_JUGADORES);
        insertarEnIndice(jugadores, jugadorTmp.nombre, sizeof(jugadorTmp.nombre)); 
    }
    else
    {
        if(!buscarJugadorEnArchivo(ARCH_JUGADORES, posicion, &jugadorTmp)) 
        {
            printf("Error al buscar jugador en archivo.\n");
            return;
        }
    }

    // --- CARGAMOS EL ESTADO INICIAL DEL JUEGO ---
    
    estado->jugador = jugadorTmp;                   // Guardamos los datos persistentes del jugador
    estado->vidasActuales = config->vidasInicio;   // Asignamos las vidas desde el archivo config.txt
    estado->puntosActuales = 0;                     // Arranca sin premios
    estado->turnosJugados = 0;                      // Arranca en el turno 0
    estado->jugadorProtegido = 0;                   // Sin protección inicial
    estado->perdioTurno = 0;                        // Sin penalización inicial
    estado->terrenoBajoJugador='I';              // El jugador inicia sobre el campamento inicial (I)
    estado->colisionBandido = 0;                  // No hay colisión al iniciar 
    // Inicializamos la memoria de los bandidos con '.' USANDO PUNTEROS
    char* pTerreno = estado->terrenoBajoBandido;
    
    while (i <  MAX_BANDIDOS) {
        *pTerreno = '.'; // Guardamos el punto en la dirección actual
        pTerreno++;      // Avanzamos a la siguiente dirección de memoria
        i++;
    }
    // --------------------------------------------

    system("cls");
    printf("\n -------------------------------CARAVANA DEL DESIERTO-------------------------------\n");
    printf("\n Bienvenido %s, tu id es %d y has jugado %d partidas.\n", estado->jugador.nombre, estado->jugador.id, estado->jugador.partidasJugadas);
    printf("\n Oprime cualquier tecla para comenzar a jugar...\n");
    getch();
    system("cls");
    
    mostrarTablero(estado); 
    // Limpiamos el registro de movimientos de partidas anteriores
    FILE* fReg = fopen("registro_movimientos.txt", "wt");
    if(fReg) fclose(fReg);
}


void iniciarCaravanaDelDesierto()
{
    // Declaramos las variables locales
    tListaDobCirc tablero = NULL; 
    tConfig configuracion;
    tArbol indiceJugadores = NULL;
    tCola colaMov;
    
    // Inicializamos la cola usando tu primitiva
    crearCola(&colaMov); 

    // Inicializamos el tEstadoJuego apuntando a nuestras estructuras base
    tEstadoJuego estado;
    estado.tablero = &tablero;
    estado.colaMovimientos = &colaMov;

    char opcion = menuPrincipal(MENSAJE_MENU_PRINCIPAL, OPCIONES_MENU_PRINCIPAL);

    // 1- Bajar el índice de jugadores
    cargarIndiceJugadores(&indiceJugadores, ARCH_INDICE_JUGADORES);

    switch(opcion)
    {
        case NUEVA_PARTIDA:
            // 2- Cargar la configuración y crear el archivo (pasamos el tablero desde el estado)
            creacionArchivoCaravana(ARCH_CARAVANA, estado.tablero, &configuracion);
            
            // 3- Procesar el inicio pasando el estado centralizado
            procesarInicioNuevaPartida(&estado, &configuracion, &indiceJugadores);
            
            // 4- Llamamos al bucle principal del juego que armamos antes
            bucleJuego(&estado, &configuracion); 
            break;
            
        case TABLA_DE_PUNTAJES:
            // Lógica de tabla de puntajes
            break;
            
        case SALIR:
            // Guardar archivos e índice
            break;
    }
}
