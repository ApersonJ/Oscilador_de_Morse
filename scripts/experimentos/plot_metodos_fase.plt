set terminal pngcairo size 900,600 enhanced font 'Verdana,10'
set output 'results/figuras/experimentos/comparacion_metodos_fase.png'
set title 'Comparacion entre metodos: Espacio de Fase'
set xlabel 'Posicion (x)'
set ylabel 'Velocidad (v)'
set grid
plot 'results/datos/experimentos/metodo_euler.dat' using 2:3 with lines title 'Euler', \
     'results/datos/experimentos/metodo_euler_cromer.dat' using 2:3 with lines title 'Euler-Cromer', \
     'results/datos/experimentos/metodo_rk2.dat' using 2:3 with lines title 'RK2', \
     'results/datos/experimentos/metodo_rk3.dat' using 2:3 with lines title 'RK3', \
     'results/datos/experimentos/metodo_rk4.dat' using 2:3 with lines title 'RK4' linecolor rgb 'black'
