set terminal pngcairo size 900,600 enhanced font 'Verdana,10'
set output 'results/figuras/experimentos/variacion_parametro_D.png'
set title 'Efecto de D en el espacio de fase'
set xlabel 'Posicion (x)'
set ylabel 'Velocidad (v)'
set grid
plot 'results/datos/experimentos/parametro_D_0.5.dat' using 2:3 with lines title 'D = 0.5', \
     'results/datos/experimentos/parametro_D_1.dat' using 2:3 with lines title 'D = 1', \
     'results/datos/experimentos/parametro_D_1.5.dat' using 2:3 with lines title 'D = 1.5', \
     'results/datos/experimentos/parametro_D_2.dat' using 2:3 with lines title 'D = 2'
