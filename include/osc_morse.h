#ifndef OSC_MORSE_H_
#define OSC_MORSE_H_

// ---------------------- Clase Osc_Morse ----------------------
// Representa un oscilador de Morse: EDO no lineal, autonoma y
// conservativa. El estado del ejemplar es (x, v) y P almacena la
// energia potencial evaluada en el x actual.
class Osc_Morse {
 private:
    double x, v;
    double P;

 public:
    void Inicie(double x0, double v0,
                double D, double a, double xe);

    double calcularAceleracion(double x,
                                double &D, double &a, double &xe, double &m) const;

    void calcularPotencial(double x,
                            double &D, double &a, double &xe);

    // Metodos de integracion: todos llaman a la misma ley fisica
    // (calcularAceleracion), solo cambia la manera de avanzar en t.
    void MuevaseEuler(double t, double dt, double &D, double &a, double &xe, double &m);
    void MuevaseEulerCromer(double t, double dt, double &D, double &a, double &xe, double &m);
    void MuevaseRK2(double t, double dt, double &D, double &a, double &xe, double &m);
    void MuevaseRK3(double t, double dt, double &D, double &a, double &xe, double &m);
    void MuevaseRK4(double t, double dt, double &D, double &a, double &xe, double &m);

    // Genera y ejecuta los scripts de Gnuplot para la simulacion principal.
    void generarScriptsGnuplot();

    double Getx() const { return x; }
    double Getv() const { return v; }
    double GetP() const { return P; }

    // Energia mecanica total E = T + V. El sistema es conservativo,
    // por lo que E deberia mantenerse aproximadamente constante.
    double GetEnergia(double m) const { return 0.5 * m * v * v + P; }
};

#endif  // OSC_MORSE_H_
