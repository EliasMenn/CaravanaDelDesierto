#include "../headers/menu.h"

//muestra el menu y devuelve la opcion elegida
char menuPrincipal(const char *mensaje, const char *opciones){
    char opc;

    puts(mensaje);
    printf("\nSeleccione una opcion: ");
    fflush(stdin);
    scanf("%c", &opc);
    while(strchr(opciones, opc) == NULL){
        printf("Error! Opcion no valida. Intente nuevamente: ");
        fflush(stdin);
        scanf("%c", &opc);
    }

    system("cls");
    return opc;
}
