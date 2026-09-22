set terminal pngcairo size 900,600 enhanced font 'Verdana,10'
set output 'results/figuras/experimentos/comparacion_paso.png'
set title 'Convergencia con el paso de integracion (RK4)'
set xlabel 'Tiempo (t)'
set ylabel 'Posicion (x)'
set grid
plot 'results/datos/experimentos/paso_dt.dat' using 1:2 with lines title 'dt' linecolor rgb 'red', \
     'results/datos/experimentos/paso_dt2.dat' using 1:2 with lines title 'dt/2' linecolor rgb 'blue', \
     'results/datos/experimentos/paso_dt4.dat' using 1:2 with lines title 'dt/4' linecolor rgb 'black'
