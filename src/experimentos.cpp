//   8.1 Variacion del paso de integracion
//   8.2 Comparacion entre metodos
//   8.3 Sensibilidad a condiciones iniciales
//   8.4 Variacion de un parametro de control (D)
//   8.6 Energia como funcion diagnostica 

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "osc_morse.h"

using namespace std;

namespace {

// Parametros base, reproducibles, para todos los experimentos.
const double kD = 1.0;
const double kA = 1.0;
const double kXe = 0.0;
const double kM = 1.0;
const double kX0 = -0.5;
const double kV0 = 0.0;
const double kDt = 0.01;
const double kTmax = 30.0;

// Corre una simulacion completa con el metodo indicado (1..5) y guarda
// columnas t, x, v, P, E, dE/E0 en 'ruta'.
void simular(int metodo, double x0, double v0, double dt, double tmax,
             double D, double a, double xe, double m, const string &ruta) {
    Osc_Morse osc;
    osc.Inicie(x0, v0, D, a, xe);
    double E0 = osc.GetEnergia(m);

    ofstream file(ruta);
    file << fixed << setprecision(6);
    file << setw(12) << "t" << setw(15) << "x" << setw(15) << "v"
         << setw(15) << "P" << setw(15) << "E" << setw(18) << "dE/E0" << "\n";

    for (double t = 0.0; t < tmax; t += dt) {
        osc.calcularPotencial(osc.Getx(), D, a, xe);
        double E = osc.GetEnergia(m);
        double dE_rel = (E0 != 0.0) ? (E - E0) / E0 : 0.0;

        file << setw(12) << t << setw(15) << osc.Getx() << setw(15) << osc.Getv()
             << setw(15) << osc.GetP() << setw(15) << E << setw(18) << dE_rel << "\n";

        if (metodo == 1) osc.MuevaseEuler(t, dt, D, a, xe, m);
        else if (metodo == 2) osc.MuevaseEulerCromer(t, dt, D, a, xe, m);
        else if (metodo == 3) osc.MuevaseRK2(t, dt, D, a, xe, m);
        else if (metodo == 4) osc.MuevaseRK3(t, dt, D, a, xe, m);
        else osc.MuevaseRK4(t, dt, D, a, xe, m);
    }
    file.close();
}

void ejecutarGnuplot(const string &ruta_script) {
    string comando = "gnuplot " + ruta_script;
    system(comando.c_str());
}

// ---------------- 8.1 Variacion del paso de integracion ----------------
void experimentoPaso() {
    cout << "Experimento 8.1: variacion del paso de integracion (RK4)" << endl;

    simular(5, kX0, kV0, kDt, kTmax, kD, kA, kXe, kM,
            "results/datos/experimentos/paso_dt.dat");
    simular(5, kX0, kV0, kDt / 2.0, kTmax, kD, kA, kXe, kM,
            "results/datos/experimentos/paso_dt2.dat");
    simular(5, kX0, kV0, kDt / 4.0, kTmax, kD, kA, kXe, kM,
            "results/datos/experimentos/paso_dt4.dat");

    ofstream script("scripts/experimentos/plot_paso.plt");
    script << "set terminal pngcairo size 900,600 enhanced font 'Verdana,10'\n";
    script << "set output 'results/figuras/experimentos/comparacion_paso.png'\n";
    script << "set title 'Convergencia con el paso de integracion (RK4)'\n";
    script << "set xlabel 'Tiempo (t)'\n";
    script << "set ylabel 'Posicion (x)'\n";
    script << "set grid\n";
    script << "plot 'results/datos/experimentos/paso_dt.dat' using 1:2 with lines title 'dt' linecolor rgb 'red', \\\n";
    script << "     'results/datos/experimentos/paso_dt2.dat' using 1:2 with lines title 'dt/2' linecolor rgb 'blue', \\\n";
    script << "     'results/datos/experimentos/paso_dt4.dat' using 1:2 with lines title 'dt/4' linecolor rgb 'black'\n";
    script.close();
    ejecutarGnuplot("scripts/experimentos/plot_paso.plt");
}

// ---------------- 8.2 y 8.6: Comparacion entre metodos + energia ----------------
void experimentoMetodos() {
    cout << "Experimento 8.2/8.6: comparacion entre los 5 metodos y diagnostico de energia" << endl;

    const vector<pair<int, string>> metodos = {
        {1, "euler"}, {2, "euler_cromer"}, {3, "rk2"}, {4, "rk3"}, {5, "rk4"}};

    for (const auto &met : metodos) {
        simular(met.first, kX0, kV0, kDt, kTmax, kD, kA, kXe, kM,
                "results/datos/experimentos/metodo_" + met.second + ".dat");
    }

    // x(t) para los 5 metodos.
    ofstream script_x("scripts/experimentos/plot_metodos_x.plt");
    script_x << "set terminal pngcairo size 900,600 enhanced font 'Verdana,10'\n";
    script_x << "set output 'results/figuras/experimentos/comparacion_metodos_x.png'\n";
    script_x << "set title 'Comparacion entre metodos: Posicion vs Tiempo'\n";
    script_x << "set xlabel 'Tiempo (t)'\nset ylabel 'Posicion (x)'\nset grid\n";
    script_x << "plot 'results/datos/experimentos/metodo_euler.dat' using 1:2 with lines title 'Euler', \\\n";
    script_x << "     'results/datos/experimentos/metodo_euler_cromer.dat' using 1:2 with lines title 'Euler-Cromer', \\\n";
    script_x << "     'results/datos/experimentos/metodo_rk2.dat' using 1:2 with lines title 'RK2', \\\n";
    script_x << "     'results/datos/experimentos/metodo_rk3.dat' using 1:2 with lines title 'RK3', \\\n";
    script_x << "     'results/datos/experimentos/metodo_rk4.dat' using 1:2 with lines title 'RK4' linecolor rgb 'black'\n";
    script_x.close();
    ejecutarGnuplot("scripts/experimentos/plot_metodos_x.plt");

    // Espacio de fase para los 5 metodos.
    ofstream script_f("scripts/experimentos/plot_metodos_fase.plt");
    script_f << "set terminal pngcairo size 900,600 enhanced font 'Verdana,10'\n";
    script_f << "set output 'results/figuras/experimentos/comparacion_metodos_fase.png'\n";
    script_f << "set title 'Comparacion entre metodos: Espacio de Fase'\n";
    script_f << "set xlabel 'Posicion (x)'\nset ylabel 'Velocidad (v)'\nset grid\n";
    script_f << "plot 'results/datos/experimentos/metodo_euler.dat' using 2:3 with lines title 'Euler', \\\n";
    script_f << "     'results/datos/experimentos/metodo_euler_cromer.dat' using 2:3 with lines title 'Euler-Cromer', \\\n";
    script_f << "     'results/datos/experimentos/metodo_rk2.dat' using 2:3 with lines title 'RK2', \\\n";
    script_f << "     'results/datos/experimentos/metodo_rk3.dat' using 2:3 with lines title 'RK3', \\\n";
    script_f << "     'results/datos/experimentos/metodo_rk4.dat' using 2:3 with lines title 'RK4' linecolor rgb 'black'\n";
    script_f.close();
    ejecutarGnuplot("scripts/experimentos/plot_metodos_fase.plt");

    // Error relativo de energia para los 5 metodos (diagnostico de estabilidad numerica).
    ofstream script_e("scripts/experimentos/plot_metodos_energia.plt");
    script_e << "set terminal pngcairo size 900,600 enhanced font 'Verdana,10'\n";
    script_e << "set output 'results/figuras/experimentos/comparacion_metodos_energia.png'\n";
    script_e << "set title 'Comparacion entre metodos: error relativo de energia'\n";
    script_e << "set xlabel 'Tiempo (t)'\nset ylabel '(E(t)-E0)/E0'\nset grid\n";
    script_e << "plot 'results/datos/experimentos/metodo_euler.dat' using 1:6 with lines title 'Euler', \\\n";
    script_e << "     'results/datos/experimentos/metodo_euler_cromer.dat' using 1:6 with lines title 'Euler-Cromer', \\\n";
    script_e << "     'results/datos/experimentos/metodo_rk2.dat' using 1:6 with lines title 'RK2', \\\n";
    script_e << "     'results/datos/experimentos/metodo_rk3.dat' using 1:6 with lines title 'RK3', \\\n";
    script_e << "     'results/datos/experimentos/metodo_rk4.dat' using 1:6 with lines title 'RK4' linecolor rgb 'black'\n";
    script_e.close();
    ejecutarGnuplot("scripts/experimentos/plot_metodos_energia.plt");
}

// ---------------- 8.3 Sensibilidad a condiciones iniciales ----------------
void experimentoCondiciones() {
    cout << "Experimento 8.3: sensibilidad a condiciones iniciales (RK4)" << endl;

    const double eps = 1e-3;
    simular(5, kX0, kV0, kDt, kTmax, kD, kA, kXe, kM,
            "results/datos/experimentos/condicion_A.dat");
    simular(5, kX0 + eps, kV0, kDt, kTmax, kD, kA, kXe, kM,
            "results/datos/experimentos/condicion_B.dat");

    ifstream fa("results/datos/experimentos/condicion_A.dat");
    ifstream fb("results/datos/experimentos/condicion_B.dat");
    ofstream fd("results/datos/experimentos/diferencia_condiciones.dat");
    string linea_a, linea_b;
    getline(fa, linea_a);
    getline(fb, linea_b); 
    fd << fixed << setprecision(8);
    fd << setw(12) << "t" << setw(18) << "|xA-xB|" << "\n";
    double t, xa, va, pa, ea, dea;
    double tb, xb, vb, pb, eb, deb;
    while (fa >> t >> xa >> va >> pa >> ea >> dea && fb >> tb >> xb >> vb >> pb >> eb >> deb) {
        fd << setw(12) << t << setw(18) << fabs(xa - xb) << "\n";
    }
    fa.close();
    fb.close();
    fd.close();

    ofstream script("scripts/experimentos/plot_condiciones.plt");
    script << "set terminal pngcairo size 900,600 enhanced font 'Verdana,10'\n";
    script << "set output 'results/figuras/experimentos/sensibilidad_condiciones.png'\n";
    script << "set title 'Sensibilidad a condiciones iniciales: x0 vs x0+1e-3'\n";
    script << "set xlabel 'Tiempo (t)'\nset ylabel 'Posicion (x)'\nset grid\n";
    script << "plot 'results/datos/experimentos/condicion_A.dat' using 1:2 with lines title 'x0', \\\n";
    script << "     'results/datos/experimentos/condicion_B.dat' using 1:2 with lines title 'x0 + 1e-3'\n";
    script.close();
    ejecutarGnuplot("scripts/experimentos/plot_condiciones.plt");

    ofstream script2("scripts/experimentos/plot_separacion.plt");
    script2 << "set terminal pngcairo size 900,600 enhanced font 'Verdana,10'\n";
    script2 << "set output 'results/figuras/experimentos/separacion_condiciones.png'\n";
    script2 << "set title 'Separacion |xA(t) - xB(t)|'\n";
    script2 << "set xlabel 'Tiempo (t)'\nset ylabel '|xA-xB|'\nset logscale y\nset grid\n";
    script2 << "plot 'results/datos/experimentos/diferencia_condiciones.dat' using 1:2 with lines "
               "title 'Separacion' linecolor rgb 'dark-red'\n";
    script2.close();
    ejecutarGnuplot("scripts/experimentos/plot_separacion.plt");
}

// ---------------- 8.4 Variacion de un parametro de control (D) ----------------
void experimentoParametro() {
    cout << "Experimento 8.4: variacion del parametro D (profundidad del pozo)" << endl;

    const vector<double> valoresD = {0.5, 1.0, 1.5, 2.0};
    ofstream script("scripts/experimentos/plot_parametro.plt");
    script << "set terminal pngcairo size 900,600 enhanced font 'Verdana,10'\n";
    script << "set output 'results/figuras/experimentos/variacion_parametro_D.png'\n";
    script << "set title 'Efecto de D en el espacio de fase'\n";
    script << "set xlabel 'Posicion (x)'\nset ylabel 'Velocidad (v)'\nset grid\n";
    script << "plot ";
    for (size_t i = 0; i < valoresD.size(); ++i) {
        ostringstream nombre;
        nombre << "results/datos/experimentos/parametro_D_" << valoresD[i] << ".dat";
        simular(5, kX0, kV0, kDt, kTmax, valoresD[i], kA, kXe, kM, nombre.str());

        script << "'" << nombre.str() << "' using 2:3 with lines title 'D = " << valoresD[i] << "'";
        if (i + 1 < valoresD.size()) script << ", \\\n     ";
    }
    script << "\n";
    script.close();
    ejecutarGnuplot("scripts/experimentos/plot_parametro.plt");
}

} 

int main() {
    cout << "Ejecutando experimentos computacionales del oscilador de Morse" << endl;
    cout << "Parametros base: D=" << kD << ", a=" << kA << ", xe=" << kXe
              << ", m=" << kM << ", x0=" << kX0 << ", v0=" << kV0
              << ", dt=" << kDt << ", tmax=" << kTmax << "\n" << endl;

    experimentoPaso();
    experimentoMetodos();
    experimentoCondiciones();
    experimentoParametro();

    cout << "\nExperimentos completados. Datos en 'results/datos/experimentos/' "
                 "y graficas en 'results/figuras/experimentos/'." << endl;
    return 0;
}
