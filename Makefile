CXX := g++
CXXFLAGS := -std=c++17 -Wall -O2 -Iinclude

BIN_DIR := bin
MORSE_BIN := $(BIN_DIR)/morse
EXP_BIN := $(BIN_DIR)/experimentos

DIRS := $(BIN_DIR) results/datos results/datos/experimentos \
        results/figuras results/figuras/experimentos \
        scripts scripts/experimentos

.PHONY: all run plot experimentos clean dirs

# Compila el simulador principal (interactivo) y el de experimentos.
all: dirs $(MORSE_BIN) $(EXP_BIN)

dirs:
	@mkdir -p $(DIRS)

$(MORSE_BIN): src/main.cpp src/osc_morse.cpp include/osc_morse.h | dirs
	$(CXX) $(CXXFLAGS) src/main.cpp src/osc_morse.cpp -o $(MORSE_BIN)

$(EXP_BIN): src/experimentos.cpp src/osc_morse.cpp include/osc_morse.h | dirs
	$(CXX) $(CXXFLAGS) src/experimentos.cpp src/osc_morse.cpp -o $(EXP_BIN)

# Compila y ejecuta la simulacion principal (pide datos por teclado
# y genera las graficas de x(t), v(t), P(x), espacio de fase y energia).
run: $(MORSE_BIN)
	./$(MORSE_BIN)

# Alias de run, pensado para cuando ya existe results/datos/trayectoria.dat
# y solo se quieren regenerar las graficas manualmente con Gnuplot.
plot:
	gnuplot scripts/plot_x_t.plt
	gnuplot scripts/plot_v_t.plt
	gnuplot scripts/plot_P_x.plt
	gnuplot scripts/plot_fase.plt
	gnuplot scripts/plot_energia.plt

# Compila y ejecuta los experimentos de la Seccion 8 del enunciado
# (no pide datos por teclado, usa parametros fijos y reproducibles).
experimentos: $(EXP_BIN)
	./$(EXP_BIN)

clean:
	rm -rf $(BIN_DIR)
	rm -f results/datos/*.dat results/datos/experimentos/*.dat
	rm -f results/figuras/*.png results/figuras/experimentos/*.png
	rm -f scripts/*.plt scripts/experimentos/*.plt
