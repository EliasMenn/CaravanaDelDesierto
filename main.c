#include <SDL3/SDL.h>
#include "./headers/juego.h"
#include "./headers/sdl_renderer.h"

int main(int argc, char* argv[])
{

    tSDLCtx ctx; // contexto de SDL
    if (!sdl_iniciar(&ctx))
        {
        printf("Error critico: No se pudo inicializar SDL3.\n");
        return 1;
        }

    iniciarCaravanaDelDesierto(&ctx);

    sdl_cerrar(&ctx);

    return 0;
}

