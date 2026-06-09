#include "..\headers\juego.h"

void procesarInicioNuevaPartida(tListaDobCirc* tablero,tArbol* jugadores) /// jugadores vendria a ser el arbol de indice de jugadores
{
    int posicion;
    tJugador jugador;
    // aca deberiamos pedir el nombre del jugador, y con eso cargar su id y puntos totales a memoria
    //(si es que existe, sino crear un nuevo jugador con ese nombre y puntos totales en 0)

        printf("\n Ingrese su nombre (alias) :");
        fflush(stdin);
        scanf("%s", jugador.nombre);
        posicion=BuscarNombreJugador(jugador.nombre,jugadores);

        if(posicion==NO_EXISTE)
        {
            jugador.id=contarNodosEnIndice(jugadores)+1; // al estar indexado por nombre la unica manera de saber el ult id (sin recorrer el archivo original sino el indice) es contar los nodos
                                                         // O contamos los nodos o abrimos el archivo original, nos posicionamos al final y obtenemos el ultimo id
                                                         // Me parecio una mejor opcion contar los nodos ya que el indice es mas chico y ademas no abrimos innecesariamente el archivo (las funciones de archivos son muy costosas)
            jugador.partidasJugadas=0;
            guardarJugador(&jugador,ARCH_JUGADORES);
            insertarEnIndice(jugadores,jugador.nombre,sizeof(jugador.nombre)); //SOLO GUARDO EL NOMBRE YA QUE ES EL CAMPO POR EL QUE INDEXO
        }
        else
        {
            if(!buscarJugadorEnArchivo(ARCH_JUGADORES,posicion,&jugador)) // con la posicion que me dio el indice busco en el archivo original el id y partidas jugadas del jugador
            {
                printf("Error al buscar jugador en archivo.\n");
                return;
            }
        }
        system("cls");
        printf("\n -------------------------------CARAVANA DEL DESIERTO-------------------------------\n");
        printf("\n Bienvenido %s, tu id es %d y has jugado %d partidas.\n", jugador.nombre, jugador.id, jugador.partidasJugadas);
        printf("\n Oprime cualquier tecla para comenzar a jugar...\n");
        getch();
        system("cls");
        mostrarTablero(tablero); //

    // Falta ahora inicializar el estado del juego (vidas actuales, puntos actuales, turnos jugados, jugador protegido y perdio turno en 0)
}
void iniciarCaravanaDelDesierto()
{
    tListaDobCirc tablero;
    tConfig configuracion;
    tArbol indiceJugadores=NULL;
    char opcion = menuPrincipal(MENSAJE_MENU_PRINCIPAL,OPCIONES_MENU_PRINCIPAL);
    // aca una vez que sabemos que la opcion elegida es valida deberiamos:
    // 1- Bajar el indice de jugadores (es requisito que apenas arranque el juego bajarlo a memoria)
    // 2- cargar la configuracion a memoria (es requisito que apenas arranque el juego cargarla a memoria)
    cargarIndiceJugadores(&indiceJugadores,ARCH_INDICE_JUGADORES);

    switch(opcion)
    {
        case NUEVA_PARTIDA:
            creacionArchivoCaravana(ARCH_CARAVANA,&tablero,&configuracion);
            procesarInicioNuevaPartida(&tablero,&indiceJugadores);
            break;
        case TABLA_DE_PUNTAJES:
            // aca deberiamos mostrar la tabla de puntajes (que se puede generar a partir del indice de jugadores)
            break;
        case SALIR:
            // aca deberiamos guardar todo lo que tenemos en memoria a los archivos correspondientes (jugadores, historicoPartidas e indiceJugadores) y salir del programa
            break;
    }

}
