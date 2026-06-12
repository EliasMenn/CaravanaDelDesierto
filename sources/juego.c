#include "..\headers\juego.h"


int creacionArchivoCaravana(const char* archCaravana,tListaDobCirc* pldc,tConfig* configuracion);
char extraerElementoAlAzar(int *cantBandidos, int *cantPremios, int *cantVidasExtras, int *cantOasis, int *cantTormentas, int *vacios, int restantes);
void mostrarTablero(tEstadoJuego* estado);


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

    // 1. Buscar desde dónde sale la entidad
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

    // --- PARCHE ANTI-FANTASMAS ---
    if (mov->tipoEntidad == 'B' && *(char*)(nodoActual->info) != 'B') {
        return;
    }

    tNodoDob* nodoDestino = nodoActual;
    int pasosRestantes = mov->pasos;
    char direccionActual = mov->direccion;

    // 2. Física de Muros Estáticos (Paredes sólidas identificadas por puntero)
    // 2. Física Mixta (Rebote en la Meta, Muro en el Inicio)
    while (pasosRestantes > 0) {
        tNodoDob* posibleDestino;
        if (direccionActual == 'F' || direccionActual == 'f') posibleDestino = nodoDestino->sig;
        else posibleDestino = nodoDestino->ant;

        // --- FÍSICA DEL JUGADOR ---
        if (mov->tipoEntidad == 'J') {
            // REBOTE EN LA META ('S'): Si avanza y trata de dar la vuelta cruzando hacia 'I'
            if ((direccionActual == 'F' || direccionActual == 'f') && posibleDestino == *(estado->tablero)) {
                direccionActual = 'B'; // Invierte la dirección
                posibleDestino = nodoDestino->ant; // Empieza a caminar para atrás
            }
            // MURO EN EL INICIO ('I'): Si retrocede y trata de dar la vuelta cruzando hacia 'S'
            else if ((direccionActual == 'B' || direccionActual == 'b') && posibleDestino == (*(estado->tablero))->ant) {
                break; // Frena en seco y pierde los pasos restantes
            }
        }
        // --- FÍSICA DE LOS BANDIDOS ---
        else if (mov->tipoEntidad == 'B') {
            // Los bandidos chocan contra ambas paredes (no dan la vuelta ni rebotan)
            if ((direccionActual == 'F' || direccionActual == 'f') && posibleDestino == *(estado->tablero)) break;
            else if ((direccionActual == 'B' || direccionActual == 'b') && posibleDestino == (*(estado->tablero))->ant) break;

            // Frena para no pisar al otro bandido
            if (*(char*)(posibleDestino->info) == 'B') break;
        }

        nodoDestino = posibleDestino;
        pasosRestantes--;
    }

    // 3. Aplicar el movimiento visual
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
            int dadoJ = tirar_dado(6);
            char dirJ;
            printf("\nSacaste un %d. Hacia donde te moves? (F: Adelante, B: Atras): ", dadoJ);
            fflush(stdin);
            scanf(" %c", &dirJ);

            tMovimiento movJugador;
            movJugador.tipoEntidad = 'J';
            movJugador.id_entidad = 0;
            // Un pequeño truco extra para convertir siempre a mayúscula y evitar problemas:
            if (dirJ == 'f') dirJ = 'F';
            if (dirJ == 'b') dirJ = 'B';
            movJugador.direccion = dirJ;
            movJugador.pasos = dadoJ;

            aColar(estado->colaMovimientos, &movJugador, sizeof(tMovimiento));
            jugadorSeMovio = 1; // Sí se movió
        }

        pos.cantBandidos = 0;
        pos.posActual = 0;
        actualizarPosiciones(estado->tablero, &pos);
        calcularMovimientos(&pos, estado->colaMovimientos);

        // --- FASE 2: DESENCOLAR Y APLICAR ---
        while (colaVacia(estado->colaMovimientos)==COLA_NO_VACIA) {
            tMovimiento movActual;
            outCola(estado->colaMovimientos, &movActual, sizeof(tMovimiento));
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
    int posicion;
    int i=0;
    tJugador jugadorTmp;

    printf("\n Ingrese su nombre (alias) :");
    fflush(stdin);
    scanf("%s", jugadorTmp.nombre);

    // --- BÚSQUEDA SEGURA ---
    int estadoBusqueda = BuscarNombreJugador(jugadorTmp.nombre, jugadores, cmp, &posicion);

    if(estadoBusqueda == NO_EXISTE)
    {
        jugadorTmp.id = contarNodosEnIndice(jugadores) + 1;
        jugadorTmp.partidasJugadas = 0;
        guardarJugador(&jugadorTmp, ARCH_JUGADORES);

        tIndice nuevoIndice;
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
    // --- HACER APARECER AL JUGADOR EN EL TABLERO ---
    // Nos posicionamos en el primer nodo (el Campamento Inicial)
    tNodoDob* nodoInicio = *(estado->tablero);
    if (nodoInicio) {
        // Sobrescribimos la 'I' con la 'J'.
        // (No perdemos la 'I' porque ya la guardamos en estado->terrenoBajoJugador = 'I')
        *(char*)(nodoInicio->info) = 'J';
    }
    // -----------------------------------------------

    system("cls");
    printf("\n -------------------------------CARAVANA DEL DESIERTO-------------------------------\n");
    // ... tus otros printf ...

    mostrarTablero(estado);
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
    cargarIndiceJugadores(&indiceJugadores, ARCH_INDICE_JUGADORES,sizeof(tIndice));

    switch(opcion)
    {
        case NUEVA_PARTIDA:
            creacionArchivoCaravana(ARCH_CARAVANA, estado.tablero, &configuracion);
            procesarInicioNuevaPartida(&estado, &configuracion, &indiceJugadores);
            bucleJuego(&estado, &configuracion);

            // --- GUARDADO DE JUGADOR (MAESTRO) ---
            actualizarJugador(&estado,ARCH_JUGADORES);
            // --- NUEVO: GUARDADO DE PARTIDA (TRANSACCIONAL) ---
            // Creamos el registro de la partida que se acaba de jugar
            guardarPartida(&estado,"archivoPartidas.bin");
            // --------------------------------------------------

            // Guardamos el índice para no perder la memoria
            guardarIndiceJugadores(&indiceJugadores, ARCH_INDICE_JUGADORES);
            break;

        case TABLA_DE_PUNTAJES:
            // Lógica de tabla de puntajes
            break;

        case SALIR:
            // Guardar archivos e índice
            break;
    }
}


/// FUNCIONES DE INTERFAZ

char extraerElementoAlAzar(int *cantBandidos, int *cantPremios, int *cantVidasExtras, int *cantOasis, int *cantTormentas, int *vacios, int restantes)
{
    srand(time(NULL));
    int r = rand() % restantes;

    // Evaluamos y si no es, le restamos el bloque a 'r' y pasamos al siguiente.
    if (r < *cantBandidos)
    {
        (*cantBandidos)--;
        return 'B';
    }
    r -= *cantBandidos;

    if (r < *cantPremios)
    {
        (*cantPremios)--;
        return 'P';
    }
    r -= *cantPremios;

    if (r < *cantVidasExtras)
    {
        (*cantVidasExtras)--;
        return 'V';
    }
    r -= *cantVidasExtras;

    if (r < *cantOasis)
    {
        (*cantOasis)--;
        return 'O';
    }
    r -= *cantOasis;

    if (r < *cantTormentas)
    {
        (*cantTormentas)--;
        return 'T';
    }
    r -= *cantTormentas;

    // Si no cayo en ninguno de los anteriores, es un espacio vacio
    (*vacios)--;
    return '.';
}

int creacionArchivoCaravana(const char* archCaravana,tListaDobCirc* pldc,tConfig* configuracion)
{
    FILE* pCaravana;
    char caracterAInsertar;
    int cantPosiciones=0,cantBandidos=0,
        cantTormentas=0,cantOasis=0,
        cantPremios=0,cantVidasExtras=0,
        cantEspeciales,cantEspaciosVacios,posIterativa=1;

    if(!cargarConfiguracion(configuracion,"config.txt"))
        return ERROR_ARCHIVO;
    if(configuracion->cantPosiciones<2) // solo tiene posicion para inicio y fin, no se puede jugar
        return ARCH_CONFIG_MAL_FORMADO; //no se puede jugar

    /// carga de cantidades auxiliares
    cantPosiciones=configuracion->cantPosiciones -2 ;// eliminamos de la cuenta al inicio (I:Inicio) y al fin (S:Salida)
    cantBandidos=configuracion->maximoBandidos;
    cantVidasExtras=configuracion->maximoVidasExtras;
    cantOasis=configuracion->maximoOasis;
    cantPremios=configuracion->maximoPremios;
    cantTormentas=configuracion->maximoTormentas;

    cantEspeciales= cantBandidos + cantOasis + cantPremios + cantTormentas + cantVidasExtras;
    cantEspaciosVacios= cantPosiciones - cantEspeciales;

    if(cantEspaciosVacios <0)
        return ARCH_CONFIG_MAL_FORMADO;

    pCaravana=fopen(archCaravana,"wt");
    if(!pCaravana)
        return ERROR_ARCHIVO;

    caracterAInsertar='I';
    insUltListDobCirc(pldc,&caracterAInsertar,sizeof(char)); //siempre primero cargo el inicio
    fprintf(pCaravana,"%02d:[%c J]\n",posIterativa,caracterAInsertar);
    posIterativa++;
    while(cantPosiciones>0)
    {
        caracterAInsertar=extraerElementoAlAzar(&cantBandidos,&cantPremios,&cantVidasExtras,&cantOasis,&cantTormentas,&cantEspaciosVacios,cantPosiciones);

        insUltListDobCirc(pldc,&caracterAInsertar,sizeof(char));
        fprintf(pCaravana,"%02d:%c\n",posIterativa,caracterAInsertar);
        cantPosiciones--;
        posIterativa++;

    }
    caracterAInsertar='S';
    insUltListDobCirc(pldc,&caracterAInsertar,sizeof(char));
    fprintf(pCaravana,"%02d:%c\n",posIterativa,caracterAInsertar);
    fclose(pCaravana);
    return 1;
}



void mostrarTablero(tEstadoJuego* estado) {
    tNodoDob* actual = *(estado->tablero);
    int posicion = 1;

    if (!actual) return;

    do {
        char c = *(char*)(actual->info);

        // Si hay que imprimir al jugador, armamos el casillero compuesto
        if (c == 'J') {
            // Si pisamos tierra vacía ('.'), premio consumido o inicio, mostramos solo [J] o [I J]
            if (estado->terrenoBajoJugador == '.' || estado->terrenoBajoJugador == 'I') {
                if (estado->terrenoBajoJugador == 'I') printf("%02d:[I J]\n", posicion);
                else printf("%02d:[J]\n", posicion);
            }
            // Si pisamos Oasis o Tormenta, mostramos ambas letras combinadas
            else {
                printf("%02d:[%c J]\n", posicion, estado->terrenoBajoJugador);
            }
        }
        // Si no es el jugador, imprimimos la letra suelta (O, T, P, V, B, .)
        else {
            printf("%02d:%c\n", posicion, c);
        }

        actual = actual->sig;
        posicion++;
    } while (actual != *(estado->tablero));

    // IMPRESIÓN DE ESTADO (HUD)
    printf("\n======================================================\n");
    printf(" JUGADOR: %s | VIDAS: %d | PUNTOS: %d | TURNOS: %d\n",
           estado->jugador.nombre,
           estado->vidasActuales,
           estado->puntosActuales,
           estado->turnosJugados);

    // Indicador visual si está protegido por el oasis o atrapado en tormenta
    if (estado->jugadorProtegido) printf(" [ESTADO: Protegido por el Oasis]\n");
    if (estado->perdioTurno) printf(" [ESTADO: Atrapado en Tormenta de Arena]\n");
    printf("======================================================\n");
}


///FUNCIONES UTILES

tNodoDob* buscarNodoEntidad(tListaDobCirc* tablero, char entidad, int id_entidad) {
    if (!(*tablero)) return NULL;

    tNodoDob* actual = *tablero;
    int contadorBandidos = 0;

    // Recorremos la lista circular
    do {
        char caracterEnNodo = *(char*)(actual->info);

        if (caracterEnNodo == entidad) {
            // Si buscamos a la 'J', devolvemos el primero que encontremos
            if (entidad == 'J') {
                return actual;
            }
            // Si buscamos a la 'B', tenemos que contar para devolver el bandido correcto
            else if (entidad == 'B') {
                contadorBandidos++;
                if (contadorBandidos == id_entidad) {
                    return actual;
                }
            }
        }
        actual = actual->sig;
    } while (actual != *tablero);

    return NULL; // No se encontró la entidad en el tablero
}


int contieneEntidad(tNodoDob* nodo, char entidad) {
    if (!nodo || !nodo->info) return 0;

    // Comparamos si el caracter del nodo coincide con el que buscamos
    return (*(char*)(nodo->info) == entidad);
}


void removerEntidadDeNodo(tNodoDob* nodo, char entidad, int id_entidad) {
    if (!nodo || !nodo->info) return;

    // Si efectivamente está la entidad en este nodo, la borramos
    if (*(char*)(nodo->info) == entidad) {
        // El documento establece que '.' representa una posición vacía o ruta despejada [cite: 76]
        *(char*)(nodo->info) = '.';
    }
}




void agregarEntidadANodo(tNodoDob* nodo, char entidad, int id_entidad) {
    if (!nodo || !nodo->info) return;

    // Sobrescribimos el casillero con la nueva entidad
    *(char*)(nodo->info) = entidad;
}

void moverJugadorAInicio(tListaDobCirc* tablero, tNodoDob* nodoActual, tEstadoJuego* estado) {
    // 1. Dejamos el casillero actual despejado (el bandido ya se consumió)
    *(char*)(nodoActual->info) = estado->terrenoBajoJugador;

    // 2. Nos movemos al nodo de inicio
    tNodoDob* nodoInicio = *tablero;

    // 3. Volvemos a memorizar la 'I' y nos posamos
    estado->terrenoBajoJugador = *(char*)(nodoInicio->info);
    *(char*)(nodoInicio->info) = 'J';
}


void registrarMovimientoHistorial(tEstadoJuego* estado, char direccion, int pasos) {
    // El trabajo práctico pide llevar un registro de los movimientos realizados por el jugador[cite: 138].
    // Al finalizar la partida se debe mostrar el registro con el formato FX o BX[cite: 139, 140].

    // La forma más segura y prolija de hacer esto sin complicar tu memoria es guardar todo
    // en un archivo de texto temporal (modo "at" para hacer append por cada turno).
    FILE* arch = fopen("registro_movimientos.txt", "at");
    if (arch) {
        fprintf(arch, "%c | %d\n", direccion, pasos);
        fclose(arch);
    }
}



/// FUNCIONES DE tPartida

int guardarPartida(tEstadoJuego* juegoActual,const char* nomArch)
{
    FILE* pPartidas;
    tPartida partidaActual;
    partidaActual.idJugador=juegoActual->jugador.id;
    partidaActual.puntosObtenidos=juegoActual->puntosActuales;

    pPartidas=fopen(nomArch,"ab");
    if(!pPartidas)
        return ERROR_ARCHIVO;

    fwrite(&partidaActual,sizeof(tPartida),1,pPartidas);

    fclose(pPartidas);
    return 1;
}


int actualizarJugador(tEstadoJuego* estado,const char* nomArch)
{
    estado->jugador.partidasJugadas++;

    FILE* pf = fopen(nomArch, "r+b");
    if (!pf)
        return ERROR_ARCHIVO;
    fseek(pf, (estado->jugador.id - 1) * sizeof(tJugador), SEEK_SET);
    fwrite(&estado->jugador, sizeof(tJugador), 1, pf);
    fclose(pf);

    return EXITO;
}
