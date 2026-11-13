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

struct Referencia {
    string nome;
    int enderecoInstrucao;
    int argumento;
};

int defineArg(const string& arg) {

    if (arg == "r0")
        return 0;

    else if (arg == "r1")
        return 1;

    else if (arg == "r2")
        return 2;

    return -1;
}

int defineOperacao(const string& op) {

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

bool simboloExiste(const vector<Simbolos>& tabela, const string& nome) {

    for (const auto& simbolo : tabela) {

        if (simbolo.nome == nome)
            return true;
    }

    return false;
}

/*
    Resolve um argumento.

    Retorna:

    - numero do registrador, se for r0/r1/r2
    - endereco do simbolo, se for um simbolo local
    - -1, se for um simbolo externo

    Quando for externo, tambem registra uma referencia.
*/
int resolveArgumento( const string& argumento, const vector<Simbolos>& tabela, vector<Referencia>& referencias, int enderecoInstrucao, int numeroArgumento) {

    if (argumento.empty())
        return -1;

    int registrador = defineArg(argumento);

    if (registrador != -1)
        return registrador;

    for (const auto& simbolo : tabela) {
        if (simbolo.nome == argumento)
            return simbolo.end;
    }

    referencias.push_back({argumento, enderecoInstrucao, numeroArgumento});

    return -1;
}

int main(int argc, char* argv[]) {

    if (argc != 2) {

        cout << "Uso: " << argv[0]
             << " arquivo.txt" << endl;

        return 1;
    }

    /*
        Abre arquivo de entrada.
    */
    ifstream arquivo(argv[1]);

    if (!arquivo.is_open()) {

        cout << "Erro ao abrir arquivo de entrada."
             << endl;

        return 1;
    }

    /*
        Define o nome do arquivo intermediario.

        Exemplo:

        programaA.txt

        vira:

        programaA_intermediario.txt
    */
    string nomeArquivo = argv[1];

    size_t pos = nomeArquivo.rfind(".txt");

    string nomeSaida;

    if (pos != string::npos) {

        nomeSaida =
            nomeArquivo.substr(0, pos)
            + "_intermediario.txt";

    } else {

        nomeSaida =
            nomeArquivo
            + "_intermediario.txt";
    }

    ofstream arquivoSaida(nomeSaida);

    if (!arquivoSaida.is_open()) {

        cout << "Erro ao criar arquivo de saida."
             << endl;

        return 1;
    }

    vector<Simbolos> tabela;
    vector<Referencia> referencias;

    string linha;
    string op;
    string arg1;
    string arg2;
    string arg3;

    /*
    ============================================================
                         PRIMEIRA PASSAGEM
    ============================================================

    Descobre os simbolos locais e seus enderecos.
    */

    int endereco = 0;

    while (getline(arquivo, linha)) {

        istringstream ss(linha);

        if (!(ss >> op))
            continue;

        /*
            "end" nao ocupa uma posicao.
        */
        if (op == "end")
            continue;

        /*
            Declaracao de variavel.
        */
        if (op == "byte") {

            ss >> arg1;

            if (arg1.empty()) {

                cout << "Erro: variavel sem nome."
                     << endl;

                return 1;
            }

            if (simboloExiste(tabela, arg1)) {

                cout << "Erro: simbolo duplicado: "
                     << arg1 << endl;

                return 1;
            }

            tabela.push_back({
                arg1,
                endereco
            });
        }

        /*
            Label.
        */
        else if (op == ":") {

            ss >> arg1;

            if (arg1.empty()) {

                cout << "Erro: label sem nome."
                     << endl;

                return 1;
            }

            if (simboloExiste(tabela, arg1)) {

                cout << "Erro: simbolo duplicado: "
                     << arg1 << endl;

                return 1;
            }

            /*
                A linha do label gera:

                -1 -1 -1 -1

                Portanto o endereco da label
                e o da proxima instrucao.
            */
            tabela.push_back({arg1, endereco + 1});
        }

        /*
            Toda linha, exceto "end", ocupa
            uma posicao no codigo.
        */
        endereco++;
    }

    arquivo.close();

    /*
    ============================================================
                        SEGUNDA PASSAGEM
    ============================================================
    */

    ifstream arquivo2(argv[1]);

    if (!arquivo2.is_open()) {

        cout << "Erro ao reabrir arquivo de entrada."
             << endl;

        return 1;
    }

    /*
        Escreve o inicio da secao CODE.
    */
    arquivoSaida << "CODE" << endl;

    int enderecoInstrucao = 0;

    while (getline(arquivo2, linha)) {

        istringstream ss(linha);

        if (!(ss >> op))
            continue;

        /*
            "end" nao gera codigo.
        */
        if (op == "end")
            continue;

        /*
            byte e label ocupam uma posicao
            no codigo intermediario.
        */
        if (op == "byte" || op == ":") {

            arquivoSaida
                << "-1 -1 -1 -1"
                << endl;

            enderecoInstrucao++;

            continue;
        }

        arg1 = "";
        arg2 = "";
        arg3 = "";

        ss >> arg1 >> arg2 >> arg3;

        int o = defineOperacao(op);

        if (o == -1) {

            cout << "Erro: operacao desconhecida: "
                 << op << endl;

            return 1;
        }

        int a1 = resolveArgumento(arg1, tabela, referencias, enderecoInstrucao, 1);

        int a2 = resolveArgumento(arg2, tabela, referencias, enderecoInstrucao, 2);

        int a3 = resolveArgumento(arg3, tabela, referencias, enderecoInstrucao, 3);

        arquivoSaida << o << " " << a1 << " " << a2 << " " << a3 << endl;

        enderecoInstrucao++;
    }

    arquivo2.close();

    /*
    ============================================================
                         TABELA DE SIMBOLOS
    ============================================================
    */

    arquivoSaida << endl;
    arquivoSaida << "SYMBOLS" << endl;

    for (const auto& simbolo : tabela) {

        arquivoSaida << simbolo.nome << " " << simbolo.end << endl;
    }

    /*
    ============================================================
                       TABELA DE REFERENCIAS
    ============================================================
    */

    arquivoSaida << endl;
    arquivoSaida << "REFERENCES" << endl;

    for (const auto& referencia : referencias) {

        arquivoSaida << referencia.nome << " " << referencia.enderecoInstrucao << " " << referencia.argumento << endl;
    }

    arquivoSaida.close();

    /*
    ============================================================
                          INFORMACOES
    ============================================================
    */

    cout << endl;
    cout << "Montagem intermediaria concluida." << endl;

    cout << "Arquivo gerado: " << nomeSaida << endl;

    cout << endl;

    cout << "Simbolos locais:" << endl;

    for (const auto& simbolo : tabela) {

        cout << "  "  << simbolo.nome << " -> " << simbolo.end << endl;
    }

    cout << endl;

    cout << "Referencias externas:" << endl;

    for (const auto& referencia : referencias) {

        cout << "  " << referencia.nome << " -> instrucao " << referencia.enderecoInstrucao << ", argumento " << referencia.argumento << endl;
    }

    return 0;
}