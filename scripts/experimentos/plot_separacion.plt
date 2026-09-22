set terminal pngcairo size 900,600 enhanced font 'Verdana,10'
set output 'results/figuras/experimentos/separacion_condiciones.png'
set title 'Separacion |xA(t) - xB(t)|'
set xlabel 'Tiempo (t)'
set ylabel '|xA-xB|'
set logscale y
set grid
plot 'results/datos/experimentos/diferencia_condiciones.dat' using 1:2 with lines title 'Separacion' linecolor rgb 'dark-red'
