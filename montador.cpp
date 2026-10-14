
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

enum Opcodes {
    IN, MOV, ADD, STORE, OUT, JUMP, STOP,
    SUB, MUL, DIV, BR, BEQ, BGT, BLT
};

struct Simbolos {
    string nome;
    int end;
};

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
    if (op == "in")    return IN;
    if (op == "mov")   return MOV;
    if (op == "add")   return ADD;
    if (op == "store") return STORE;
    if (op == "out")   return OUT;
    if (op == "jmp")   return JUMP;
    if (op == "stop")  return STOP;
    if (op == "sub")   return SUB;
    if (op == "mul")   return MUL;
    if (op == "div")   return DIV;
    if (op == "br")    return BR;
    if (op == "beq")   return BEQ;
    if (op == "bgt")   return BGT;
    if (op == "blt")   return BLT;

    return -1;
}

int buscaSimbolo(const vector<Simbolos>& tabela,
                 const string& nome) {
    for (const auto& simbolo : tabela) {
        if (simbolo.nome == nome)
            return simbolo.end;
    }

    return defineArg(nome);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Uso: " << argv[0] << " arquivo.asm" << endl;
        return 1;
    }

    ifstream arquivo(argv[1]);

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo de entrada." << endl;
        return 1;
    }

    ofstream arquivoSaida("saida.txt");

    if (!arquivoSaida.is_open()) {
        cout << "Erro ao criar arquivo de saida." << endl;
        return 1;
    }

    vector<Simbolos> tabela;
    string linha, op, arg1, arg2, arg3;

    // PRIMEIRA PASSAGEM:
    // Registra variaveis e labels com seus enderecos corretos.
    int endereco = 0;

    while (getline(arquivo, linha)) {
        istringstream ss(linha);

        if (!(ss >> op))
            continue;

        if (op == "end")
            continue;

        if (op == "byte") {
            ss >> arg1;

            if (arg1.empty()) {
                cout << "Erro: variavel sem nome." << endl;
                return 1;
            }

            tabela.push_back({arg1, endereco});

        } else if (op == ":") {
            ss >> arg1;

            if (arg1.empty()) {
                cout << "Erro: label sem nome." << endl;
                return 1;
            }

            // O label ocupa uma linha na saida,
            // mas o desvio deve apontar para a proxima linha.
            tabela.push_back({arg1, endereco + 1});
        }

        // Cada linha nao vazia, exceto "end",
        // corresponde a uma linha na saida.
        endereco++;
    }

    arquivo.close();

    cout << "Tabela de simbolos:" << endl;

    for (const auto& simbolo : tabela) {
        cout << simbolo.nome << " "
             << simbolo.end << endl;
    }

    // SEGUNDA PASSAGEM:
    ifstream arquivo2(argv[1]);

    if (!arquivo2.is_open()) {
        cout << "Erro ao reabrir arquivo de entrada." << endl;
        return 1;
    }

    while (getline(arquivo2, linha)) {
        istringstream ss(linha);

        if (!(ss >> op))
            continue;

        // Diretivas que nao geram instrucao executavel.
        if (op == "end")
            continue;

        if (op == "byte" || op == ":") {
            arquivoSaida << "-1 -1 -1 -1" << endl;
            continue;
        }

        arg1 = arg2 = arg3 = "";

        ss >> arg1 >> arg2 >> arg3;

        int o = defineOperacao(op);
        int a1 = buscaSimbolo(tabela, arg1);
        int a2 = buscaSimbolo(tabela, arg2);
        int a3 = buscaSimbolo(tabela, arg3);

        if (o == -1) {
            cout << "Erro: operacao desconhecida: "
                 << op << endl;
            return 1;
        }

        arquivoSaida << o << " "
                     << a1 << " "
                     << a2 << " "
                     << a3 << endl;
    }

    arquivo2.close();
    arquivoSaida.close();

    cout << "Montagem concluida."
         << endl;

}