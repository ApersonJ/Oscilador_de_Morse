# Proyecto Morse — Física Computacional 2

Modelamiento computacional del **oscilador de Morse**, un modelo elemental de
vibración molecular anharmónica. Este proyecto corresponde a la fenomenología
3.5 de la tarea *"Modelamiento computacional de sistemas dinámicos no
lineales"*.

## Ecuación diferencial implementada

Potencial de Morse:

```
V(x) = D [1 - e^(-a(x - xe))]^2
```

Ecuación de movimiento:

```
m x'' = -2 D a [ e^(-a(x - xe)) - e^(-2a(x - xe)) ]
```

Es una EDO no lineal (no linealidad exponencial), **autónoma** y
**conservativa**. Reducida a sistema de primer orden:

```
x' = v
v' = -(2 D a / m) [ e^(-a(x - xe)) - e^(-2a(x - xe)) ]
```

## Significado de los parámetros y condiciones iniciales

| Símbolo | Significado                                   |
|---------|------------------------------------------------|
| `D`     | Energía de disociación (profundidad del pozo)  |
| `a`     | Constante de rigidez / anchura del pozo         |
| `xe`    | Posición de equilibrio                          |
| `m`     | Masa reducida                                   |
| `x0`    | Posición inicial                                |
| `v0`    | Velocidad inicial                               |
| `dt`    | Paso de integración                             |
| `tmax`  | Tiempo total de simulación                      |

## Métodos numéricos disponibles

El programa permite elegir, sin cambiar la ecuación diferencial, entre:

1. Euler
2. Euler-Cromer
3. Runge-Kutta orden 2 (método del punto medio)
4. Runge-Kutta orden 3 (formulación clásica de 3 etapas)
5. Runge-Kutta orden 4 (clásico)

Los cinco métodos llaman a la misma función `calcularAceleracion`, que
contiene la única ley física del sistema; solo cambia la manera de avanzar
en el tiempo.

## Estructura de carpetas

```
Proyecto_Morse/
├── include/
│   └── osc_morse.h          Declaración de la clase Osc_Morse
├── src/
│   ├── osc_morse.cpp        Implementación de la clase (física + integradores)
│   ├── main.cpp             Programa interactivo principal
│   └── experimentos.cpp     Programa no interactivo con los experimentos de la Sección 8
├── scripts/                 Scripts de Gnuplot (generados en tiempo de ejecución)
│   └── experimentos/
├── results/
│   ├── datos/                Archivos .dat de la simulación principal
│   │   └── experimentos/     Archivos .dat de cada experimento
│   └── figuras/               Gráficas .png de la simulación principal
│       └── experimentos/      Gráficas .png de cada experimento
├── docs/                     Informe técnico en PDF
├── Makefile
└── README.md
```

## Compilación

Requiere `g++` con soporte C++17 y `gnuplot` instalado en el sistema.

```bash
make          # compila bin/morse y bin/experimentos
make clean    # elimina binarios, datos y gráficas generadas
```

## Ejecución

### Simulación principal (interactiva)

```bash
make run
```

Pide por teclado `x0`, `v0`, `dt`, `tmax`, `D`, `a`, `xe`, `m` y el método
(1 a 5). Al finalizar guarda los datos y genera automáticamente las gráficas
con Gnuplot (equivalente a `make plot`).

**Conjunto de parámetros reproducible sugerido:**

```
x0 = -0.5   v0 = 0.0
dt = 0.01   tmax = 30
D = 1.0     a = 1.0     xe = 0.0     m = 1.0
método = 5 (RK4)
```

### Experimentos computacionales

```bash
make experimentos
```

Ejecuta, con parámetros fijos y reproducibles (`D=1.0, a=1.0, xe=0.0, m=1.0,
x0=-0.5, v0=0.0, dt=0.01, tmax=30`), los siguientes estudios de la Sección 8:

- **8.1 — Variación del paso:** integra con RK4 usando `dt`, `dt/2` y `dt/4`
  y compara `x(t)` para evaluar convergencia.
- **8.2 — Comparación entre métodos:** corre Euler, Euler-Cromer, RK2, RK3 y
  RK4 con los mismos parámetros y compara `x(t)` y el espacio de fase.
- **8.3 — Sensibilidad a condiciones iniciales:** integra `x0` y `x0 + 1e-3`
  con RK4 y grafica tanto las trayectorias como su separación `|xA(t)-xB(t)|`.
- **8.4 — Variación de un parámetro de control:** varía `D` (0.5, 1.0, 1.5,
  2.0) y compara el efecto sobre el espacio de fase.
- **8.6 — Energía como función diagnóstica:** se incluye en cada corrida
  (columnas `E` y `dE/E0` del archivo de datos) y se grafica de forma
  comparativa entre métodos en el experimento 8.2, mostrando que RK4
  conserva la energía mientras que Euler la disipa/gana de forma espuria.

Las secciones **8.5** (transitorio/régimen de largo tiempo) y **8.7**
(muestreo de Poincaré) no se incluyen porque están pensadas para sistemas
forzados periódicamente, y el oscilador de Morse aquí modelado es autónomo
y no forzado.

## Archivos de salida

Cada archivo `.dat` (simulación principal y experimentos) contiene las
columnas:

```
t   x   v   P   E   dE/E0
```

donde `P` es la energía potencial de Morse, `E = 0.5*m*v^2 + P` es la
energía mecánica total y `dE/E0 = (E(t)-E(0))/E(0)` es el error relativo de
energía, útil para diagnosticar la calidad de cada método numérico en un
sistema conservativo.

## Procedimiento para generar las gráficas

Las gráficas se generan automáticamente al ejecutar `make run` o
`make experimentos` (los programas llaman a `gnuplot` mediante `system()`).
Para regenerarlas manualmente sin volver a correr la simulación:

```bash
make plot                                  # gráficas de la simulación principal
gnuplot scripts/experimentos/*.plt         # gráficas de los experimentos
```
