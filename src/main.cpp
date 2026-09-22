#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "osc_morse.h"

using namespace std;

// Prototipos de funciones
void solicitarDatos(double &x0, double &v0,
                     double &dt, double &tmax,
                     double &D, double &a, double &xe, double &m, int &metodo);

// --------------------------- funcion principal ------------------------------
int main() {
    Osc_Morse Oscilador;

    double x0, v0;
    double t, dt, tmax;
    // Parametros del sistema.
    double D, a, xe, m;
    int metodo;

    solicitarDatos(x0, v0, dt, tmax, D, a, xe, m, metodo);

    if (metodo == 1)
        cout << "Metodo de integracion          = Euler" << endl;
    else if (metodo == 2)
        cout << "Metodo de integracion          = Euler-Cromer" << endl;
    else if (metodo == 3)
        cout << "Metodo de integracion          = Runge-Kutta 2do Orden (punto medio)" << endl;
    else if (metodo == 4)
        cout << "Metodo de integracion          = Runge-Kutta 3er Orden" << endl;
    else
        cout << "Metodo de integracion          = Runge-Kutta 4to Orden" << endl;

    Oscilador.Inicie(x0, v0, D, a, xe);

    // Energia inicial, usada para el error relativo (E(t)-E0)/E0.
    double E0 = Oscilador.GetEnergia(m);

    ofstream file("results/datos/trayectoria.dat");

    file << fixed << setprecision(6);
    file << setw(12) << "t"
         << setw(15) << "x"
         << setw(15) << "v"
         << setw(15) << "P"
         << setw(15) << "E"
         << setw(18) << "dE/E0" << endl;

    for (t = 0.0; t < tmax; t += dt) {
        Oscilador.calcularPotencial(Oscilador.Getx(), D, a, xe);

        double E = Oscilador.GetEnergia(m);
        double dE_rel = (E0 != 0.0) ? (E - E0) / E0 : 0.0;

        file << setw(12) << t
             << setw(15) << Oscilador.Getx()
             << setw(15) << Oscilador.Getv()
             << setw(15) << Oscilador.GetP()
             << setw(15) << E
             << setw(18) << dE_rel << endl;

        if (metodo == 1)
            Oscilador.MuevaseEuler(t, dt, D, a, xe, m);
        else if (metodo == 2)
            Oscilador.MuevaseEulerCromer(t, dt, D, a, xe, m);
        else if (metodo == 3)
            Oscilador.MuevaseRK2(t, dt, D, a, xe, m);
        else if (metodo == 4)
            Oscilador.MuevaseRK3(t, dt, D, a, xe, m);
        else
            Oscilador.MuevaseRK4(t, dt, D, a, xe, m);
    }

    file.close();

    cout << "\nDatos guardados en 'results/datos/trayectoria.dat'" << endl;

    Oscilador.generarScriptsGnuplot();

    return 0;
}

// --------------- Solicitar los datos -------------------------------
void solicitarDatos(double &x0, double &v0,
                     double &dt, double &tmax,
                     double &D, double &a, double &xe, double &m, int &metodo) {
    cout << "Ingrese las condiciones iniciales" << endl;
    cout << "Posicion inicial x0: "; cin >> x0;
    cout << "Velocidad inicial v0: "; cin >> v0;

    cout << "Ingrese el paso y el tiempo maximo" << endl;
    cout << "Paso dt: "; cin >> dt;
    cout << "Tiempo de simulacion: "; cin >> tmax;

    while (dt < 0 || tmax < 0) {
        cout << "Dato no valido. Ingrese tiempos positivos: ";
        cout << "Paso dt: "; cin >> dt;
        cout << "Tiempo de simulacion: "; cin >> tmax;
    }

    cout << "Ingrese los parametros del oscilador" << endl;
    cout << "Energia de disociacion D: "; cin >> D;
    cout << "Constante de rigidez a: "; cin >> a;
    cout << "Posicion de equilibrio xe: "; cin >> xe;
    cout << "Masa reducida m: "; cin >> m;

    cout << "\nSeleccione el metodo de integracion:" << endl;
    cout << "1. Euler" << endl;
    cout << "2. Euler-Cromer" << endl;
    cout << "3. Runge-Kutta orden 2 (punto medio)" << endl;
    cout << "4. Runge-Kutta orden 3" << endl;
    cout << "5. Runge-Kutta orden 4" << endl;
    cout << "Opcion: ";
    cin >> metodo;

    while (metodo < 1 || metodo > 5) {
        cout << "Opcion no valida. Ingrese un valor entre 1 y 5: ";
        cin >> metodo;
    }
}
