# 🐫 Caravana del Desierto

![C](https://img.shields.io/badge/Lenguaje-C-blue.svg)
![UNLaM](https://img.shields.io/badge/UNLaM-AyED_2026-success.svg)

**Caravana del Desierto** es un juego de supervivencia por turnos desarrollado en lenguaje C para la cátedra de Algoritmos y Estructuras de Datos (3640) de la Universidad Nacional de La Matanza.

## 🎓 Comisión 02-3300

---

## ⚙️ Estructuras de Datos vistas en la materia implementadas (TDAs)

* **Tablero Físico (Lista Doblemente Enlazada Circular):** El mapa del desierto está modelado como un anillo continuo, donde se almacenan una serie de componentes que le dan vida al juego.
* **Sistema de Turnos (Cola Dinámica):** Los movimientos generados por el jugador (lanzamiento de dados) y las decisiones algorítmicas de la IA de los bandidos son encolados y desencolados secuencialmente para procesar el estado del turno y repintar la consola.
* **Motor de Búsqueda e Índices (Árbol Binario de Búsqueda):** La persistencia de jugadores y rankings utiliza árboles binarios para mantener índices en RAM, garantizando una búsqueda eficiente antes de interactuar con el archivo.

## 🎮 Mecánicas de Juego

Una caravana intenta atravesar una antigua ruta comercial para llegar a una Ciudad Refugio antes de perder todas sus vidas. El desierto es un círculo infinito y hostil.

* **El Jugador `J`**: Arroja un dado (1-6) y elige si avanzar o retroceder sobre la lista. Tiene prohibido atravesar el inicio y la salida (paredes fisicas).
* **Bandidos `B`**: Enemigos controlados por la computadora. Calculan tu posición en tiempo real e intentan interceptarte. Los bandidos utilizan aritmética modular para calcular el camino más corto hacia su presa, atravesando el final e inicio de la lista sin restricciones lógicas.
* **Oasis `O`**: Otorga inmunidad total durante el próximo turno.
* **Tormentas `T`**: Inmoviliza al jugador haciéndole perder el turno.
* **Premios `P` y Vidas `V`**: Suman puntos al ranking histórico y aumentan la vida en plena partida.

## 🛠️ Configuración e Inicialización

El escenario se genera dinámicamente al comienzo de cada partida leyendo el archivo `config.txt`. El administrador puede configurar:
* Dimensiones máximas del tablero.
* Cantidad de vidas iniciales.
* Densidad de aparición de Bandidos, Oasis, Tormentas, Premios y Vidas Extras.

--NOTA: todo esto interactuando con el archivo por fuera del lenguaje

Las estadísticas, registros de movimientos e historial de jugadores se persisten automáticamente en archivos binarios (`.dat`/`.bin`) y archivos de texto (`caravana.txt`, `movimientos.txt`).

## 🚀 Compilación y Ejecución

El código fue diseñado para cumplir con los estándares académicos más estrictos y compila garantizando **0 errores y 0 warnings**.

**Dependencias:**
* Compilador GCC / MinGW.
* Entorno sugerido: Code::Blocks.

**Instrucciones (Terminal):**
```bash
# 1. Clonar el repositorio
git clone https://github.com/EliasMenn/CaravanaDelDesierto.git
```
