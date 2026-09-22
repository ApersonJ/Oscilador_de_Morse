set terminal pngcairo size 900,600 enhanced font 'Verdana,10'
set output 'results/figuras/experimentos/sensibilidad_condiciones.png'
set title 'Sensibilidad a condiciones iniciales: x0 vs x0+1e-3'
set xlabel 'Tiempo (t)'
set ylabel 'Posicion (x)'
set grid
plot 'results/datos/experimentos/condicion_A.dat' using 1:2 with lines title 'x0', \
     'results/datos/experimentos/condicion_B.dat' using 1:2 with lines title 'x0 + 1e-3'
