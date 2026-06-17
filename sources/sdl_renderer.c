#include "../headers/sdl_renderer.h"
#include <stdio.h>
#include <string.h>

/* =========================================================
   COLORES
   ========================================================= */
#define COL_FONDO       {  26,  18,   9, 255 }
#define COL_HUD_BG      {  17,  24,  34, 255 }
#define COL_HUD_BORDER  {  42,  58,  42, 255 }
#define COL_TEXTO       { 220, 210, 190, 255 }
#define COL_TEXTO_DIM   { 100,  90,  80, 255 }

/* colores de fondo por tipo de casillero */
static SDL_Color bgColor(char tipo) {
    switch(tipo) {
        case 'I': return (SDL_Color){14, 61, 26, 255};
        case 'S': return (SDL_Color){10, 31, 64, 255};
        case 'J': return (SDL_Color){61, 48,  0, 255};
        case 'B': return (SDL_Color){61, 10, 10, 255};
        case 'O': return (SDL_Color){10, 37, 64, 255};
        case 'T': return (SDL_Color){42, 13, 64, 255};
        case 'P': return (SDL_Color){61, 46,  0, 255};
        case 'V': return (SDL_Color){61, 15, 15, 255};
        default:  return (SDL_Color){44, 36, 22, 255};
    }
}

static SDL_Color borderColor(char tipo) {
    switch(tipo) {
        case 'J': return (SDL_Color){232, 212, 139, 255};
        case 'B': return (SDL_Color){192,  57,  43, 255};
        case 'I': return (SDL_Color){ 30, 132,  73, 255};
        case 'S': return (SDL_Color){ 26,  82, 118, 255};
        case 'O': return (SDL_Color){ 41, 128, 185, 255};
        case 'T': return (SDL_Color){125,  60, 152, 255};
        case 'P': return (SDL_Color){241, 196,  15, 255};
        case 'V': return (SDL_Color){231,  76,  60, 255};
        default:  return (SDL_Color){ 70,  60,  40, 255};
    }
}

/* =========================================================
   HELPERS DE DIBUJO
   ========================================================= */
static void setColor(SDL_Renderer* r, SDL_Color c) {
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
}

static void fillRect(SDL_Renderer* r, int x, int y, int w, int h, SDL_Color c) {
    setColor(r, c);
    SDL_FRect rect = {(float)x, (float)y, (float)w, (float)h};
    SDL_RenderFillRect(r, &rect);
}

static void drawRect(SDL_Renderer* r, int x, int y, int w, int h, SDL_Color c) {
    setColor(r, c);
    SDL_FRect rect = {(float)x, (float)y, (float)w, (float)h};
    SDL_RenderRect(r, &rect);
}

static void drawLine(SDL_Renderer* r, int x1, int y1, int x2, int y2, SDL_Color c) {
    setColor(r, c);
    SDL_RenderLine(r, (float)x1, (float)y1, (float)x2, (float)y2);
}

static void fillCircle(SDL_Renderer* r, int cx, int cy, int radio, SDL_Color c) {
    int dy, dx;

    setColor(r, c);
    for (dy = -radio; dy <= radio; dy++) {
        dx = (int)SDL_sqrt((double)(radio*radio - dy*dy));
        SDL_RenderLine(r, (float)(cx-dx), (float)(cy+dy), (float)(cx+dx), (float)(cy+dy));
    }
}

static void drawText(SDL_Renderer* r, TTF_Font* f, const char* txt, int x, int y, SDL_Color c, int centrado) {
    SDL_Surface* surf;
    SDL_Texture* tex;

    if (!f || !txt || txt[0] == '\0')
        return;

    surf = TTF_RenderText_Blended(f, txt, 0, c);
    if (!surf)
        return;

    tex = SDL_CreateTextureFromSurface(r, surf);
    if (tex) {
        float w = (float)surf->w, h = (float)surf->h;
        SDL_FRect dst = { centrado ? (float)(x - surf->w/2) : (float)x, (float)(y - surf->h/2), w, h };
        SDL_RenderTexture(r, tex, NULL, &dst);
        SDL_DestroyTexture(tex);
    }

    SDL_DestroySurface(surf);
}

/* =========================================================
   DIBUJOS DE ENTIDADES
   ========================================================= */

/* Carruaje (jugador) */
static void dibujarCarruaje(SDL_Renderer* r, int cx, int cy) {
    SDL_Color marron      = {200, 146,  42, 255};
    SDL_Color marronOsc   = {139,  96,  16, 255};
    SDL_Color amarillo    = {232, 184,  75, 255};
    SDL_Color ventana     = {126, 200, 227, 255};
    SDL_Color rueda       = { 90,  58,  16, 255};
    SDL_Color camello     = {200, 160,  96, 255};

    int bx = cx - 18, by = cy - 28, i;

    /* techo arqueado */
    for (i = 0; i < 5; i++)
        drawLine(r, bx+i*2, by - 10 + i, bx+36-i*2, by - 10 + i, amarillo);

    /* cuerpo */
    fillRect(r, bx, by, 36, 22, marron);
    drawRect(r, bx, by, 36, 22, marronOsc);

    /* ventanas */
    fillRect(r, bx+4,  by+3, 9, 7, ventana);
    fillRect(r, bx+22, by+3, 9, 7, ventana);

    /* puerta */
    fillRect(r, bx+13, by+10, 10, 12, marronOsc);
    fillCircle(r, bx+21, by+16, 2, amarillo);

    /* eje */
    drawLine(r, bx+4, by+22, bx+32, by+22, rueda);

    /* rueda izquierda */
    fillCircle(r, bx+8,  by+30, 8, rueda);
    fillCircle(r, bx+8,  by+30, 3, marronOsc);
    drawLine(r, bx+8, by+22, bx+8, by+38, marronOsc);
    drawLine(r, bx,   by+30, bx+16, by+30, marronOsc);

    /* rueda derecha */
    fillCircle(r, bx+28, by+30, 8, rueda);
    fillCircle(r, bx+28, by+30, 3, marronOsc);
    drawLine(r, bx+28, by+22, bx+28, by+38, marronOsc);
    drawLine(r, bx+20, by+30, bx+36, by+30, marronOsc);

    /* vara y camello */
    drawLine(r, bx, by+20, bx-10, by+26, rueda);
    fillCircle(r, bx-14, by+24, 5, camello);
    fillCircle(r, bx-14, by+18, 4, camello);
}

/* Stickman bandido */
static void dibujarBandido(SDL_Renderer* r, int cx, int cy) {
    SDL_Color piel = {212, 160, 112, 255};
    SDL_Color sombr = { 61,  34,   0, 255};
    SDL_Color banda = {139,   0,   0, 255};
    SDL_Color panyo = {204,  34,   0, 255};
    SDL_Color cuerpo = { 90,  51,   0, 255};
    SDL_Color pistola = { 51,  51,  51, 255};

    int hx = cx, hy = cy - 32, i;

    /* pa�uelo */
    SDL_FPoint pts[4] = {
        {(float)(hx-8), (float)(hy+13)},
        {(float)(hx+8), (float)(hy+13)},
        {(float)(hx+6), (float)(hy+22)},
        {(float)(hx-6), (float)(hy+22)}
    };

    /* sombrero ala */
    fillRect(r, hx-14, hy-2, 28, 4, sombr);
    /* sombrero copa */
    fillRect(r, hx-9,  hy-14, 18, 12, sombr);
    /* banda */
    fillRect(r, hx-9,  hy-4,  18, 3,  banda);

    /* cabeza */
    fillCircle(r, hx, hy+6, 8, piel);

    setColor(r, panyo);
    for (i = 0; i < 4; i++)
        SDL_RenderLine(r, pts[i].x, pts[i].y, pts[(i+1)%4].x, pts[(i+1)%4].y);

    /* cuerpo */
    drawLine(r, hx, hy+14, hx, hy+34, cuerpo);

    /* brazo izquierdo levantado con pistola */
    drawLine(r, hx, hy+20, hx-14, hy+12, cuerpo);
    fillRect(r, hx-22, hy+8,  9, 5, pistola);
    fillRect(r, hx-19, hy+13, 3, 5, pistola);

    /* brazo derecho */
    drawLine(r, hx, hy+20, hx+12, hy+28, cuerpo);

    /* piernas */
    drawLine(r, hx, hy+34, hx-9,  hy+48, cuerpo);
    drawLine(r, hx, hy+34, hx+9,  hy+48, cuerpo);
}

/* Oasis con palmera */
static void dibujarOasis(SDL_Renderer* r, int cx, int cy) {
    SDL_Color agua    = { 41, 128, 185, 180};
    SDL_Color tronco  = {125,  96,   8, 255};
    SDL_Color hoja    = { 30, 150,  70, 255};
    SDL_Color hojaOsc = { 20, 110,  50, 255};

    int bx = cx, by = cy + 10, i;
    int tx = bx+1, ty = by-42; /* punta del tronco */

    /* laguna */
    for (i = 0; i < 6; i++)
        fillRect(r, bx-14+i, by+2-i/3, 28-i*2, 4, agua);

    /* tronco curvo (segmentos) */
    drawLine(r, bx,   by,    bx-1, by-10, tronco);
    drawLine(r, bx-1, by-10, bx-3, by-20, tronco);
    drawLine(r, bx-3, by-20, bx-2, by-32, tronco);
    drawLine(r, bx-2, by-32, bx+1, by-42, tronco);
    /* grosor extra */
    drawLine(r, bx+1, by,    bx,   by-10, tronco);
    drawLine(r, bx,   by-10, bx-2, by-20, tronco);
    drawLine(r, bx-2, by-20, bx-1, by-32, tronco);
    drawLine(r, bx-1, by-32, bx+2, by-42, tronco);

    /* hojas: 5 ramas */
    /* izquierda larga */
    for (i = 0; i < 3; i++)
        drawLine(r, tx+i, ty+i, tx-18+i, ty-10+i, hojaOsc);
    for (i = 0; i < 3; i++)
        drawLine(r, tx+i, ty+i, tx-18+i, ty-8+i,  hoja);

    /* izquierda media */
    for (i = 0; i < 3; i++)
        drawLine(r, tx+i, ty+i, tx-12+i, ty-18+i, hojaOsc);
    for (i = 0; i < 3; i++)
        drawLine(r, tx+i, ty+i, tx-10+i, ty-17+i, hoja);

    /* arriba */
    for (i = 0; i < 3; i++)
        drawLine(r, tx+i, ty, tx+i, ty-16, hojaOsc);
    for (i = 0; i < 3; i++)
        drawLine(r, tx+i, ty, tx+2+i, ty-16, hoja);

    /* derecha media */
    for (i = 0; i < 3; i++)
        drawLine(r, tx+i, ty+i, tx+14+i, ty-16+i, hojaOsc);
    for (i = 0; i < 3; i++)
        drawLine(r, tx+i, ty+i, tx+14+i, ty-14+i, hoja);

    /* derecha larga */
    for (i = 0; i < 3; i++)
        drawLine(r, tx+i, ty+i, tx+20+i, ty-8+i, hojaOsc);
    for (i = 0; i < 3; i++)
        drawLine(r, tx+i, ty+i, tx+20+i, ty-6+i, hoja);
}

/* Tornado */
static void dibujarTornado(SDL_Renderer* r, int cx, int cy) {
    SDL_Color cols[5] = {
        {125,  60, 152, 220},
        {142,  68, 173, 200},
        {155,  89, 182, 180},
        {165, 105, 189, 160},
        {187, 143, 206, 140}
    };
    int radios[5] = {20, 15, 10, 6, 3}, i, dy, dx;
    int ys[5] = {cy-30, cy-18, cy-6, cy+6, cy+16};

    for (i = 0; i < 5; i++) {
        for (dy = -3; dy <= 3; dy++) {
            dx = (int)SDL_sqrt((double)(radios[i]*radios[i] - (dy < radios[i] ? dy*dy : radios[i]*radios[i]-1)));
            fillRect(r, cx-dx, ys[i]+dy, dx*2, 1, cols[i]);
        }
    }
    /* espiral */
    SDL_Color espiral = {210, 180, 220, 120};
    drawLine(r, cx-18, ys[0], cx-8,  ys[1], espiral);
    drawLine(r, cx-8,  ys[1], cx+6,  ys[2], espiral);
    drawLine(r, cx+6,  ys[2], cx-2,  ys[3], espiral);
    drawLine(r, cx-2,  ys[3], cx+2,  ys[4], espiral);
}

/* Premio (estrella) */
static void dibujarPremio(SDL_Renderer* r, int cx, int cy) {
    int i, j;
    float ang, rad;

    SDL_Color oro     = {241, 196,  15, 255};
    SDL_Color oroOsc  = {183, 149,  11, 255};

    /* estrella de 5 puntas con lineas */
    float angBase = -SDL_PI_F / 2.0f;
    float ext = 20.0f, inn = 9.0f;
    SDL_FPoint pts[10];

    for (i = 0; i < 10; i++) {
        ang = angBase + i * SDL_PI_F / 5.0f;
        rad = (i % 2 == 0) ? ext : inn;
        pts[i].x = cx + rad * SDL_cosf(ang);
        pts[i].y = cy - 10 + rad * SDL_sinf(ang);
    }

    setColor(r, oro);
    for (i = 0; i < 10; i++) {
        j = (i+1) % 10;
        SDL_RenderLine(r, pts[i].x, pts[i].y, pts[j].x, pts[j].y);
        SDL_RenderLine(r, pts[i].x, pts[i].y, (float)cx, (float)(cy-10));
    }

    /* base trofeo */
    fillRect(r, cx-8,  cy+12, 16, 5,  oroOsc);
    fillRect(r, cx-12, cy+17, 24, 5,  oroOsc);
}

/* Corazon (vida extra) */
static void dibujarCorazon(SDL_Renderer* r, int cx, int cy) {
    int ancho;

    SDL_Color rojo    = {231,  76,  60, 255};
    SDL_Color rojoCla = {241, 148, 138, 100};

    /* dos circulos + triangulo */
    fillCircle(r, cx-7, cy-8, 9, rojo);
    fillCircle(r, cx+7, cy-8, 9, rojo);

    /* triangulo inferior */
    for (int dy = 0; dy < 18; dy++) {
        ancho = 16 - dy;
        if (ancho < 0) ancho = 0;
            fillRect(r, cx-ancho, cy-2+dy, ancho*2, 1, rojo);
    }

    /* brillo */
    fillCircle(r, cx-5, cy-12, 3, rojoCla);
}

/* Casillero inicio/salida */
static void dibujarLetra(SDL_Renderer* r, int cx, int cy, char letra, SDL_Color c) {
    /* dibujamos un rectangulo con la letra - se superpone el texto encima */
    fillRect(r, cx-16, cy-20, 32, 34, c);
    drawRect(r, cx-16, cy-20, 32, 34, (SDL_Color){255,255,255,60});
}

/* =========================================================
   CASILLERO
   ========================================================= */
static void dibujarCasillero(SDL_Renderer* r, TTF_Font* fCell, int x, int y, char tipo, int numero, char terrenoBajo) {
    char numStr[8];

    SDL_Color bg  = bgColor(tipo);
    SDL_Color brd = borderColor(tipo);

    /* icono centrado */
    int iconCX = x + CELL_W/2;
    int iconCY = y + CELL_H/2 - 4;

    /* letra identificadora abajo */
    char letraStr[3] = {tipo, '\0', '\0'};
    SDL_Color letraCol;

    /* fondo */
    fillRect(r, x, y, CELL_W, CELL_H, bg);

    /* borde */
    drawRect(r, x, y, CELL_W, CELL_H, brd);

    /* numero arriba */
    SDL_snprintf(numStr, sizeof(numStr), "%02d", numero);
    SDL_Color dimCol = (SDL_Color){100, 90, 80, 255};
    drawText(r, fCell, numStr, x + CELL_W/2, y + 10, dimCol, 1);

    switch(tipo) {
        case 'J': dibujarCarruaje(r, iconCX, iconCY);  break;
        case 'B': dibujarBandido(r,  iconCX, iconCY);  break;
        case 'O': dibujarOasis(r,    iconCX, iconCY);  break;
        case 'T': dibujarTornado(r,  iconCX, iconCY);  break;
        case 'P': dibujarPremio(r,   iconCX, iconCY);  break;
        case 'V': dibujarCorazon(r,  iconCX, iconCY);  break;
        case 'I': dibujarLetra(r, iconCX, iconCY, 'I', (SDL_Color){30,132,73,255});  break;
        case 'S': dibujarLetra(r, iconCX, iconCY, 'S', (SDL_Color){26,82,118,255});  break;
        default:
            fillCircle(r, iconCX, iconCY+8, 4, (SDL_Color){80,70,55,255});
            break;
    }

    switch(tipo) {
        case 'J': letraCol = (SDL_Color){232,212,139,255}; break;
        case 'B': letraCol = (SDL_Color){231, 76, 60,255}; break;
        case 'O': letraCol = (SDL_Color){126,200,227,255}; break;
        case 'T': letraCol = (SDL_Color){195,155,211,255}; break;
        case 'P': letraCol = (SDL_Color){241,196, 15,255}; break;
        case 'V': letraCol = (SDL_Color){231, 76, 60,255}; break;
        case 'I': letraCol = (SDL_Color){169,223,191,255}; break;
        case 'S': letraCol = (SDL_Color){174,214,241,255}; break;
        default:  letraCol = (SDL_Color){ 80, 70, 55,255}; break;
    }
    drawText(r, fCell, letraStr, x + CELL_W/2, y + CELL_H - 10, letraCol, 1);
}


int sdl_iniciar(tSDLCtx* ctx) {
    ctx->inicializado = 0;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init error: %s", SDL_GetError());
        return 0;
    }

    if (!TTF_Init()) {
        SDL_Log("TTF_Init error: %s", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    ctx->ventana = SDL_CreateWindow("Caravana del Desierto", VENTANA_W, VENTANA_H, 0);
    if (!ctx->ventana) {
        SDL_Log("CreateWindow error: %s", SDL_GetError());
        TTF_Quit(); SDL_Quit();
        return 0;
    }

    ctx->renderer = SDL_CreateRenderer(ctx->ventana, NULL);
    if (!ctx->renderer) {
        SDL_Log("CreateRenderer error: %s", SDL_GetError());
        SDL_DestroyWindow(ctx->ventana);
        TTF_Quit(); SDL_Quit();
        return 0;
    }

    ctx->fuenteHud  = TTF_OpenFont(FONT_PATH, FONT_SIZE_HUD);
    ctx->fuenteCell = TTF_OpenFont(FONT_PATH, FONT_SIZE_CELL);

    if (!ctx->fuenteHud || !ctx->fuenteCell) {
        SDL_Log("TTF_OpenFont error: %s", SDL_GetError());
        SDL_DestroyRenderer(ctx->renderer);
        SDL_DestroyWindow(ctx->ventana);
        TTF_Quit(); SDL_Quit();
        return 0;
    }

    ctx->inicializado = 1;
    return 1;
}

void sdl_cerrar(tSDLCtx* ctx) {
    if (!ctx->inicializado) return;
    if (ctx->fuenteHud)  TTF_CloseFont(ctx->fuenteHud);
    if (ctx->fuenteCell) TTF_CloseFont(ctx->fuenteCell);
    if (ctx->renderer)   SDL_DestroyRenderer(ctx->renderer);
    if (ctx->ventana)    SDL_DestroyWindow(ctx->ventana);
    TTF_Quit();
    SDL_Quit();
    ctx->inicializado = 0;
}

void sdl_limpiar(tSDLCtx* ctx) {
    SDL_Color fondo = (SDL_Color)COL_FONDO;
    setColor(ctx->renderer, fondo);
    SDL_RenderClear(ctx->renderer);
}

void sdl_presentar(tSDLCtx* ctx) {
    SDL_RenderPresent(ctx->renderer);
}

void sdl_renderizarCasilleroIndividual(tSDLCtx* ctx, char tipo, int idx, int esJugador, int esBandido, int totalCasilleros) {
    // Calculamos el espacio total y el margen inicial
    int anchoTotalTablero = totalCasilleros * CELL_W + (totalCasilleros - 1) * CELL_GAP;
    int startX = (VENTANA_W - anchoTotalTablero) / 2;

    // centrado
    int x = startX + idx * (CELL_W + CELL_GAP);
    int y = BOARD_Y;

    int iconCX = x + CELL_W/2;
    int iconCY = y + CELL_H/2 - 4;

    char terrenoFondo = tipo, entidad, label[16];

    if (tipo == '.') {
        if (esJugador)
            terrenoFondo = 'J';
        else if (esBandido)
            terrenoFondo = 'B';
    }

    dibujarCasillero(ctx->renderer, ctx->fuenteCell, x, y, terrenoFondo, idx, '.');

    if (esJugador) {
        dibujarCarruaje(ctx->renderer, iconCX, iconCY);
    }
    else if (esBandido) {
        dibujarBandido(ctx->renderer, iconCX, iconCY);
    }

    if ((esJugador || esBandido) && tipo != '.') {
        entidad = esJugador ? 'J' : 'B';
        SDL_snprintf(label, sizeof(label), "[%c %c]", tipo, entidad);
        drawText(ctx->renderer, ctx->fuenteCell, label, x + CELL_W/2, y + CELL_H + 15, (SDL_Color){220, 200, 150, 255}, 1);
    }
}

void sdl_renderizarHUD(tSDLCtx* ctx, const char* nombre, int vidas, int puntos, int turnos, int protegido, int perdioTurno) {
    SDL_Color hudBg     = (SDL_Color)COL_HUD_BG;
    SDL_Color hudBorder = (SDL_Color)COL_HUD_BORDER;
    SDL_Color textoCol  = (SDL_Color)COL_TEXTO;
    SDL_Color verdeCla  = {168, 230, 168, 255};
    SDL_Color amarillo  = {232, 212, 139, 255};
    SDL_Color rojo      = {231,  76,  60, 255};

    int cardW = 280, cardH = 80, gap = 20;
    int totalW = 4*cardW + 3*gap;
    int startX = (VENTANA_W - totalW) / 2;
    int cardY  = HUD_Y + 20;

    const char* labels[4] = {"JUGADOR", "VIDAS", "PUNTOS", "TURNO"};
    char valStr[64];
    int i, cx;

    /* fondo HUD */
    fillRect(ctx->renderer, 0, HUD_Y, VENTANA_W, VENTANA_H - HUD_Y, hudBg);
    drawLine(ctx->renderer, 0, HUD_Y, VENTANA_W, HUD_Y, hudBorder);

    for (i = 0; i < 4; i++) {
        cx = startX + i*(cardW+gap);
        fillRect(ctx->renderer, cx, cardY, cardW, cardH,
                 (SDL_Color){13,17,23,255});
        drawRect(ctx->renderer, cx, cardY, cardW, cardH,
                 (SDL_Color){30,45,30,255});

        /* label */
        drawText(ctx->renderer, ctx->fuenteCell, labels[i],
                 cx + cardW/2, cardY + 18, verdeCla, 1);

        /* valor */
        SDL_Color valCol = textoCol;
        switch(i) {
            case 0:
                SDL_snprintf(valStr, sizeof(valStr), "%s", nombre);
                valCol = amarillo;
                break;
            case 1: {
                valStr[0] = '\0';
                for (int v = 0; v < vidas && v < 10; v++)
                    SDL_strlcat(valStr, "\xe2\x9d\xa4 ", sizeof(valStr));
                valCol = rojo;
                break;
            }
            case 2:
                SDL_snprintf(valStr, sizeof(valStr), "%d", puntos);
                valCol = verdeCla;
                break;
            case 3:
                SDL_snprintf(valStr, sizeof(valStr), "%d", turnos);
                valCol = textoCol;
                break;
        }
        drawText(ctx->renderer, ctx->fuenteHud, valStr, cx + cardW/2, cardY + 55, valCol, 1);
    }

    /* estado */
    if (protegido) {
        SDL_Color azul = {41, 128, 185, 255};
        drawText(ctx->renderer, ctx->fuenteCell, "[ Protegido por el Oasis ]", VENTANA_W/2, HUD_Y + 115, azul, 1);
    }
    if (perdioTurno) {
        SDL_Color morado = {155, 89, 182, 255};
        drawText(ctx->renderer, ctx->fuenteCell, "[ Atrapado en Tormenta - Turno perdido ]", VENTANA_W/2, HUD_Y + 115, morado, 1);
    }
}

void sdl_dibujarMensajeDado(tSDLCtx* ctx, int dado) {
    SDL_Color amarillo = {232, 212, 139, 255};
    SDL_Color dimCol   = {160, 150, 130, 255};

    char msg[64];
    SDL_snprintf(msg, sizeof(msg), "Sacaste un %d  �  <- Atras    Adelante ->", dado);

    // Dibujamos el cartelito sobre el HUD
    fillRect(ctx->renderer, 0, HUD_Y - 36, VENTANA_W, 30, (SDL_Color){26,18,9,255});
    drawText(ctx->renderer, ctx->fuenteHud, msg, VENTANA_W/2, HUD_Y - 22, amarillo, 1);
    drawText(ctx->renderer, ctx->fuenteCell, "[ ESC para salir ]", VENTANA_W/2, HUD_Y - 8, dimCol, 1);
}

char sdl_esperarDireccion(tSDLCtx* ctx) {
    SDL_Event e;
    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) return 'Q';
            if (e.type == SDL_EVENT_KEY_DOWN) {
                switch (e.key.key) {
                    case SDLK_RIGHT: ctx->mostrarEvento = 0; return 'F'; // Apaga el cartel
                    case SDLK_LEFT:  ctx->mostrarEvento = 0; return 'B'; // Apaga el cartel
                    case SDLK_ESCAPE: ctx->mostrarEvento = 0; return 'Q';
                    default: break;
                }
            }
        }
        SDL_Delay(16);
    }
}

char sdl_menu(tSDLCtx* ctx) {
    SDL_Color fondo   = (SDL_Color)COL_FONDO;
    SDL_Color titulo  = {232, 212, 139, 255};
    SDL_Color normal  = {200, 190, 170, 255};
    SDL_Color hover   = {255, 230, 100, 255};

    const char* opciones[3] = {
        "1.  Nueva Partida",
        "2.  Ver Ranking",
        "3.  Salir"
    };
    const char teclas[3] = {'1', '2', '3'};
    int sel = 0;

    while (1) {
        setColor(ctx->renderer, fondo);
        SDL_RenderClear(ctx->renderer);

        drawText(ctx->renderer, ctx->fuenteHud, "CARAVANA DEL DESIERTO", VENTANA_W/2, 200, titulo, 1);
        drawLine(ctx->renderer, VENTANA_W/2 - 160, 225, VENTANA_W/2 + 160, 225, (SDL_Color){100,90,70,255});

        for (int i = 0; i < 3; i++) {
            SDL_Color c = (i == sel) ? hover : normal;
            char textoOpcion[64];

            // Si es la opcion seleccionada, le dibujamos flechas a los costados
            if (i == sel) {
                SDL_snprintf(textoOpcion, sizeof(textoOpcion), "->  %s  <-", opciones[i]);
            } else {
                SDL_snprintf(textoOpcion, sizeof(textoOpcion), "    %s    ", opciones[i]);
            }

            drawText(ctx->renderer, ctx->fuenteHud, textoOpcion, VENTANA_W/2, 280 + i*60, c, 1);
        }
        drawText(ctx->renderer, ctx->fuenteCell,
                 "Usa las flechas arriba/abajo y Enter para confirmar",
                 VENTANA_W/2, 480, (SDL_Color){100,90,70,255}, 1);

        SDL_RenderPresent(ctx->renderer);

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) return '3';
            if (e.type == SDL_EVENT_KEY_DOWN) {
                switch (e.key.key) {
                    case SDLK_UP:
                        sel = (sel + 2) % 3;
                        break;
                    case SDLK_DOWN:
                        sel = (sel + 1) % 3;
                        break;
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        return teclas[sel];
                    case SDLK_1: return '1';
                    case SDLK_2: return '2';
                    case SDLK_3: return '3';
                    case SDLK_ESCAPE: return '3';
                    default:
                        break;
                }
            }
        }
        SDL_Delay(16);
    }
}

void sdl_pantallaFin(tSDLCtx* ctx, const char* nombre, int puntos, int vidas, int turnos, int gano) {
    char linea[64];
    const char* tituloStr = gano ? "LLEGASTE A LA CIUDAD REFUGIO!" : "LA CARAVANA FUE DERROTADA";

    SDL_Color fondo   = (SDL_Color)COL_FONDO;
    SDL_Color titulo  = gano ? (SDL_Color){46,204,113,255}
                              : (SDL_Color){231,76,60,255};
    SDL_Color normal  = (SDL_Color)COL_TEXTO;
    SDL_Color dim     = (SDL_Color)COL_TEXTO_DIM;

    setColor(ctx->renderer, fondo);
    SDL_RenderClear(ctx->renderer);

    drawText(ctx->renderer, ctx->fuenteHud, tituloStr, VENTANA_W/2, 200, titulo, 1);

    SDL_snprintf(linea, sizeof(linea), "Jugador : %s", nombre);
    drawText(ctx->renderer, ctx->fuenteHud, linea, VENTANA_W/2, 290, normal, 1);

    SDL_snprintf(linea, sizeof(linea), "Puntos  : %d", puntos);
    drawText(ctx->renderer, ctx->fuenteHud, linea, VENTANA_W/2, 340, normal, 1);

    SDL_snprintf(linea, sizeof(linea), "Vidas   : %d", vidas);
    drawText(ctx->renderer, ctx->fuenteHud, linea, VENTANA_W/2, 390, normal, 1);

    SDL_snprintf(linea, sizeof(linea), "Turnos  : %d", turnos);
    drawText(ctx->renderer, ctx->fuenteHud, linea, VENTANA_W/2, 440, normal, 1);

    drawText(ctx->renderer, ctx->fuenteCell,
             "Presiona cualquier tecla para continuar...",
             VENTANA_W/2, 530, dim, 1);

    SDL_RenderPresent(ctx->renderer);

    SDL_Event e;
    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT ||
                e.type == SDL_EVENT_KEY_DOWN) return;
        }
        SDL_Delay(16);
    }
}

void sdl_pedirNombre(tSDLCtx* ctx, char* buffer, int maxLen) {
    int listo = 0;
    int len = 0;
    buffer[0] = '\0';

    // Le decimos a SDL que empiece a escuchar el teclado como texto (maneja may�sculas, etc.)
    SDL_StartTextInput(ctx->ventana);

    SDL_Color titulo = {232, 212, 139, 255};
    SDL_Color texto = {220, 210, 190, 255};
    SDL_Color cajaBg = {17, 24, 34, 255};
    SDL_Color cajaBorde = {42, 58, 42, 255};
    SDL_Color fondo = (SDL_Color)COL_FONDO;

    while (!listo) {
        setColor(ctx->renderer, fondo);
        SDL_RenderClear(ctx->renderer);

        drawText(ctx->renderer, ctx->fuenteHud, "INGRESA TU ALIAS PARA LA CARAVANA:", VENTANA_W/2, VENTANA_H/2 - 60, titulo, 1);

        // Dibujamos la "caja de texto"
        fillRect(ctx->renderer, VENTANA_W/2 - 200, VENTANA_H/2 - 20, 400, 60, cajaBg);
        drawRect(ctx->renderer, VENTANA_W/2 - 200, VENTANA_H/2 - 20, 400, 60, cajaBorde);

        // Si ya escribi� algo, lo dibujamos en el centro de la caja
        if (len > 0) {
            drawText(ctx->renderer, ctx->fuenteHud, buffer, VENTANA_W/2, VENTANA_H/2 + 10, texto, 1);
        }

        drawText(ctx->renderer, ctx->fuenteCell, "[ Presiona ENTER para confirmar ]", VENTANA_W/2, VENTANA_H/2 + 80, (SDL_Color)COL_TEXTO_DIM, 1);

        SDL_RenderPresent(ctx->renderer);

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                listo = 1;
            } else if (e.type == SDL_EVENT_KEY_DOWN) {
                // Enter confirma el nombre
                if (e.key.key == SDLK_RETURN || e.key.key == SDLK_KP_ENTER) {
                    if (len > 0) listo = 1;
                }
                // Backspace borra la ultima letra
                else if (e.key.key == SDLK_BACKSPACE && len > 0) {
                    len--;
                    buffer[len] = '\0';
                }
            }
            // Esto atrapa las letras reales
            else if (e.type == SDL_EVENT_TEXT_INPUT) {
                if (len + (int)strlen(e.text.text) < maxLen - 1) { // -1 para no pasarnos del l�mite del vector
                    strcat(buffer, e.text.text);
                    len += (int)strlen(e.text.text);
                }
            }
        }
        SDL_Delay(16);
    }
    SDL_StopTextInput(ctx->ventana);
}

void sdl_pantallaBienvenida(tSDLCtx* ctx, const char* nombre, int id, int partidas) {
    char linea[128];
    SDL_Color fondo = (SDL_Color)COL_FONDO;
    SDL_Color amarillo = {232, 212, 139, 255};
    SDL_Color verdeCla = {168, 230, 168, 255};

    setColor(ctx->renderer, fondo);
    SDL_RenderClear(ctx->renderer);

    drawText(ctx->renderer, ctx->fuenteHud, "CARAVANA DEL DESIERTO", VENTANA_W/2, VENTANA_H/2 - 80, amarillo, 1);

    SDL_snprintf(linea, sizeof(linea), "Bienvenido %s, tu ID es %d.", nombre, id);
    drawText(ctx->renderer, ctx->fuenteHud, linea, VENTANA_W/2, VENTANA_H/2 - 10, verdeCla, 1);

    SDL_snprintf(linea, sizeof(linea), "Has jugado %d partidas.", partidas);
    drawText(ctx->renderer, ctx->fuenteHud, linea, VENTANA_W/2, VENTANA_H/2 + 30, verdeCla, 1);

    drawText(ctx->renderer, ctx->fuenteCell, "Presiona cualquier tecla para comenzar la aventura...", VENTANA_W/2, VENTANA_H/2 + 100, (SDL_Color)COL_TEXTO_DIM, 1);

    SDL_RenderPresent(ctx->renderer);

    SDL_Event e;
    int listo = 0;
    while (!listo) {
        while (SDL_PollEvent(&e)) {
            // Sale si aprietan la X o cualquier tecla
            if (e.type == SDL_EVENT_QUIT || e.type == SDL_EVENT_KEY_DOWN) {
                listo = 1;
            }
        }
        SDL_Delay(16);
    }
}

void sdl_mostrarMensajeContextual(tSDLCtx* ctx, const char* titulo, const char* detalle, int esMalo) {
    // Si ocurre un error cr�tico antes de armar el juego, limpiamos el fondo para que el cartel sea legible
    SDL_SetRenderDrawColor(ctx->renderer, 26, 18, 9, 255); // Color marr�n desierto de fondo
    SDL_RenderClear(ctx->renderer);

    // Guarda los textos en el contexto gr�fico
    SDL_snprintf(ctx->eventoTitulo, sizeof(ctx->eventoTitulo), "%s", titulo);
    SDL_snprintf(ctx->eventoDetalle, sizeof(ctx->eventoDetalle), "%s", detalle);
    ctx->eventoEsMalo = esMalo;
    ctx->mostrarEvento = 1; // Encendemos la bandera para que el overlay sepa que debe dibujarse
}

void sdl_renderizarOverlayEvento(tSDLCtx* ctx) {
    int w = 700, h = 100;
    int x = (VENTANA_W - w) / 2;
    int y = 50; // Se dibuja arriba de todo para no tapar al jugador

    // Si no hay evento, no dibuja nada
    if (!ctx->mostrarEvento)
        return;

    SDL_Color bg = {20, 20, 20, 240};
    SDL_Color borde = ctx->eventoEsMalo ? (SDL_Color){231, 76, 60, 255} : (SDL_Color){46, 204, 113, 255};
    SDL_Color texto = {220, 210, 190, 255};

    fillRect(ctx->renderer, x, y, w, h, bg);
    drawRect(ctx->renderer, x, y, w, h, borde);
    drawRect(ctx->renderer, x+1, y+1, w-2, h-2, borde);

    drawText(ctx->renderer, ctx->fuenteHud, ctx->eventoTitulo, VENTANA_W/2, y + 30, borde, 1);
    drawText(ctx->renderer, ctx->fuenteHud, ctx->eventoDetalle, VENTANA_W/2, y + 70, texto, 1);
}

void sdl_mostrarHistorial(tSDLCtx* ctx, const char* ruta_archivo) {
    FILE *arch;
    int startX = 200, startY = 130, colOffset = 300, maxPorColumna = 25, count = 0;
    int listo = 0, col, row;
    char linea[128], *nl;
    SDL_Event e;

    SDL_Color fondo = (SDL_Color)COL_FONDO;
    SDL_Color titulo = {232, 212, 139, 255};
    SDL_Color texto = {200, 190, 170, 255};

    setColor(ctx->renderer, fondo);
    SDL_RenderClear(ctx->renderer);

    drawText(ctx->renderer, ctx->fuenteHud, "REGISTRO DE MOVIMIENTOS", VENTANA_W/2, 60, titulo, 1);
    drawLine(ctx->renderer, VENTANA_W/2 - 250, 90, VENTANA_W/2 + 250, 90, (SDL_Color){100,90,70,255});

    arch = fopen(ruta_archivo, "rt");
    if (!arch) {
        drawText(ctx->renderer, ctx->fuenteHud, "No hay movimientos registrados.", VENTANA_W/2, VENTANA_H/2, texto, 1);
    } else {
        while (fgets(linea, sizeof(linea), arch)) {
            nl = strchr(linea, '\n');
            if(nl) *nl = '\0';
            if(strlen(linea) >= 2){
                col = count / maxPorColumna;
                row = count % maxPorColumna;
                if (col < 4) {
                    drawText(ctx->renderer, ctx->fuenteCell, linea, startX + col * colOffset, startY + row * 22, texto, 0);
                }
                count++;
            }
        }
        fclose(arch);
    }

    drawText(ctx->renderer, ctx->fuenteCell, "[ Presiona ENTER para salir al Menu ]", VENTANA_W/2, VENTANA_H - 50, (SDL_Color){100,100,100,255}, 1);
    SDL_RenderPresent(ctx->renderer);

    while (!listo) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) exit(0);
            if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key == SDLK_RETURN || e.key.key == SDLK_KP_ENTER || e.key.key == SDLK_SPACE || e.key.key == SDLK_ESCAPE) {
                    listo = 1;
                }
            }
        }
        SDL_Delay(16);
    }
}

void sdl_iniciarPantallaRanking(tSDLCtx* ctx) {
    SDL_Color fondo = (SDL_Color)COL_FONDO;
    SDL_Color titulo = {232, 212, 139, 255};
    SDL_Color columnas = {168, 230, 168, 255};

    setColor(ctx->renderer, fondo);
    SDL_RenderClear(ctx->renderer); // Limpiamos pantalla

    drawText(ctx->renderer, ctx->fuenteHud, "RANKING HISTORICO DE JUGADORES", VENTANA_W/2, 60, titulo, 1);
    drawLine(ctx->renderer, VENTANA_W/2 - 300, 90, VENTANA_W/2 + 300, 90, (SDL_Color){100,90,70,255});

    drawText(ctx->renderer, ctx->fuenteCell, "Pos.", 350, 140, columnas, 0);
    drawText(ctx->renderer, ctx->fuenteCell, "Nombre", 480, 140, columnas, 0);
    drawText(ctx->renderer, ctx->fuenteCell, "Puntos Totales", 780, 140, columnas, 0);
    drawText(ctx->renderer, ctx->fuenteCell, "Partidas", 1080, 140, columnas, 0);
    drawLine(ctx->renderer, 350, 165, 1180, 165, (SDL_Color){70,60,50,255});
}

void sdl_dibujarFilaRanking(tSDLCtx* ctx, const char* pos, const char* nom, const char* pts, const char* part, int y) {
    SDL_Color texto = {200, 190, 170, 255};
    drawText(ctx->renderer, ctx->fuenteCell, pos, 350, y, texto, 0);
    drawText(ctx->renderer, ctx->fuenteCell, nom, 480, y, texto, 0);
    drawText(ctx->renderer, ctx->fuenteCell, pts, 780, y, texto, 0);
    drawText(ctx->renderer, ctx->fuenteCell, part, 1080, y, texto, 0);
}

void sdl_finalizarPantallaRanking(tSDLCtx* ctx) {
    int listo;

    drawText(ctx->renderer, ctx->fuenteCell, "[ Presiona ENTER o ESC para volver al Menu Principal ]", VENTANA_W/2, VENTANA_H - 60, (SDL_Color){100,100,100,255}, 1);
    SDL_RenderPresent(ctx->renderer); // Mostramos lo dibujado por el �rbol

    listo = 0;
    SDL_Event e;
    while (!listo) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) exit(0);
            if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key == SDLK_RETURN || e.key.key == SDLK_KP_ENTER || e.key.key == SDLK_ESCAPE || e.key.key == SDLK_SPACE) {
                    listo = 1;
                }
            }
        }
        SDL_Delay(16);
    }
}
