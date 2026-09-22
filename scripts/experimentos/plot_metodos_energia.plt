set terminal pngcairo size 900,600 enhanced font 'Verdana,10'
set output 'results/figuras/experimentos/comparacion_metodos_energia.png'
set title 'Comparacion entre metodos: error relativo de energia'
set xlabel 'Tiempo (t)'
set ylabel '(E(t)-E0)/E0'
set grid
plot 'results/datos/experimentos/metodo_euler.dat' using 1:6 with lines title 'Euler', \
     'results/datos/experimentos/metodo_euler_cromer.dat' using 1:6 with lines title 'Euler-Cromer', \
     'results/datos/experimentos/metodo_rk2.dat' using 1:6 with lines title 'RK2', \
     'results/datos/experimentos/metodo_rk3.dat' using 1:6 with lines title 'RK3', \
     'results/datos/experimentos/metodo_rk4.dat' using 1:6 with lines title 'RK4' linecolor rgb 'black'
