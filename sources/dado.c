#include "../headers/dado.h"

int tirar_dado(unsigned lados)
{
    return rand() % lados + 1;
}
