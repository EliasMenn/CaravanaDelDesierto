#include "../headers/dado.h"

int tirar_dado(unsigned lados)
{
    srand(time(NULL));
    return rand() % lados + 1;
}
