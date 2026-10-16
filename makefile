CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

all: montador simulador

montador: montador.cpp
	$(CXX) $(CXXFLAGS) montador.cpp -o montador

simulador: simulador.cpp
	$(CXX) $(CXXFLAGS) simulador.cpp -o simulador

run: all
	./montador $(ARQUIVO) && ./simulador saida.txt

clean:
	rm -f montador simulador saida_binario.txt