#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "headers/dado.h"

int main()
{
    srand(time(NULL));

    int dado = tirar_dado(6);

    printf("%d\n", dado);

    return 0;
}
