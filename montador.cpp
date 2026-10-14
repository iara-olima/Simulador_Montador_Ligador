#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

enum Opcodes {
    IN,
    MOV,
    ADD,
    STORE,
    OUT,
    JUMP,
    STOP,
    SUB,
    MUL,
    DIV,
    BR,
    BEQ,
    BGT,
    BLT
};

struct Simbolos {
    string nome;
    int end;
};

#define NUM_SIMBOLOS 4

int defineArg(string arg) {
    if (arg == "r0")
        return 0;
    else if (arg == "r1")
        return 1;
    else if (arg == "r2")
        return 2;
    else
        return -1;
}

int defineOperacao(string op) {
    if (op == "in")
        return IN;
    else if (op == "mov")
        return MOV;
    else if (op == "add")
        return ADD;
    else if (op == "store")
        return STORE;
    else if (op == "out")
        return OUT;
    else if (op == "jmp")
        return JUMP;
    else if (op == "stop")
        return STOP;
    else if (op == "sub")
        return SUB;
    else if (op == "mul")
        return MUL;
    else if (op == "div")
        return DIV;
    else if (op == "br")
        return BR;
    else if (op == "beq")
        return BEQ;
    else if (op == "bgt")
        return BGT;
    else if (op == "blt")
        return BLT;
    else
        return -1;
}

int buscaSimbolo(Simbolos tabela[], string nome) {
    for (int i = 0; i < NUM_SIMBOLOS; i++) {

        if (tabela[i].nome == nome) {
            return tabela[i].end;
        }
    }
    return defineArg(nome);
}

int main(int argc, char *argv[]) {
    string linha, op, arg1, arg2, arg3;
    ofstream arquivoSaida("saida.txt");
    int o, a1, a2, a3, i=0;
    Simbolos tabela[NUM_SIMBOLOS];

    if (argc != 2) {
        cout << "Numero de argumentos insuficientes" << endl;
    }

    ifstream arquivo(argv[1]);

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo" << endl;
    }

    int c = 0;

    while (getline(arquivo, linha)) {
        istringstream ss(linha);
        if (!(ss >> op)) continue;

        if (op == "byte" || op == ":") {
            ss >> arg1;
            tabela[i].nome = arg1;
            tabela[i].end  = i;
            i++;
        }
        
        c++;
    }   
    arquivo.close();
    
    for (int i = 0; i < NUM_SIMBOLOS; i++) {
        cout << tabela[i].nome << " " << tabela[i].end << endl;
    }

    ifstream arquivo2(argv[1]);
    while (getline(arquivo2, linha)) {
        istringstream ss(linha);
        if (!(ss >> op)) continue;

        arg1 = arg2 = arg3 = "";
        
        if (op == "byte" || op == ":") {
            arquivoSaida << "-1 -1 -1 -1" << endl; 
        } else {
            ss >> arg1 >> arg2 >> arg3;
            
            o  = defineOperacao(op);
            a1 = buscaSimbolo(tabela, arg1);
            a2 = buscaSimbolo(tabela, arg2);
            a3 = buscaSimbolo(tabela, arg3);

            arquivoSaida << o << " " << a1 << " " << a2 << " " << a3 << endl;
        }
    }
    arquivoSaida.close();
}