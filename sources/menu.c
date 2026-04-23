#include "../headers/menu.h"

//muestra el men� y devuelve la opci�n elegida
char menuPrincipal(const char *mensaje, const char *opciones){
    char opc;

    puts(mensaje);
    printf("\nSeleccione una opci�n: ");
    fflush(stdin);
    scanf("%c", &opc);
    while(strchr(opciones, opc) == NULL){
        printf("Error! Opci�n no v�lida. Intente nuevamente: ");
        fflush(stdin);
        scanf("%c", &opc);
    }

    system("cls");
    return opc;
}
