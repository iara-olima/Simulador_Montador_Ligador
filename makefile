CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

MONTADOR = montador
LIGADOR = ligador
SIMULADOR = simulador

ARQ_A = programaA.txt
ARQ_B = programaB.txt

INTERMEDIARIO_A = programaA_intermediario.txt
INTERMEDIARIO_B = programaB_intermediario.txt

SAIDA = saida_binario.txt


# ==========================================================
# PADRAO
# ==========================================================

all: simular


# ==========================================================
# COMPILACAO
# ==========================================================

$(MONTADOR): montador-ligador.cpp
	$(CXX) $(CXXFLAGS) montador-ligador.cpp -o $(MONTADOR)


$(LIGADOR): ligador.cpp
	$(CXX) $(CXXFLAGS) ligador.cpp -o $(LIGADOR)


$(SIMULADOR): simulador.cpp
	$(CXX) $(CXXFLAGS) simulador.cpp -o $(SIMULADOR)


# ==========================================================
# MONTAGEM
# ==========================================================

$(INTERMEDIARIO_A): $(ARQ_A) $(MONTADOR)
	./$(MONTADOR) $(ARQ_A)


$(INTERMEDIARIO_B): $(ARQ_B) $(MONTADOR)
	./$(MONTADOR) $(ARQ_B)


montar: $(INTERMEDIARIO_A) $(INTERMEDIARIO_B)


# ==========================================================
# LIGACAO
# ==========================================================

$(SAIDA): $(INTERMEDIARIO_A) $(INTERMEDIARIO_B) $(LIGADOR)
	./$(LIGADOR) $(INTERMEDIARIO_A) $(INTERMEDIARIO_B)


ligar: $(SAIDA)


# ==========================================================
# SIMULACAO
# ==========================================================

simular: $(SAIDA) $(SIMULADOR)
	./$(SIMULADOR) $(SAIDA)


# ==========================================================
# TESTE AUTOMATICO
# ==========================================================

test: $(SAIDA) $(SIMULADOR)
	@echo "=========================================="
	@echo "         TESTE COMPLETO DO SISTEMA"
	@echo "=========================================="
	@echo ""
	./$(SIMULADOR) $(SAIDA)


# ==========================================================
# LIMPEZA
# ==========================================================

clean:
	rm -f $(MONTADOR)
	rm -f $(LIGADOR)
	rm -f $(SIMULADOR)
	rm -f $(INTERMEDIARIO_A)
	rm -f $(INTERMEDIARIO_B)
	rm -f $(SAIDA)