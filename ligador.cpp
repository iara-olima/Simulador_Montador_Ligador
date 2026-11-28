#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

struct Simbolo {

    string nome;
    int endereco;
};


struct Referencia {
    string nome;
    int enderecoInstrucao;
    int argumento;
};


struct Modulo {
    string nome;
    vector<vector<int>> codigo;
    vector<Simbolo> simbolos;
    vector<string> globais;
    vector<Referencia> referencias;
    int base;
};

bool simboloExiste(
    const vector<Simbolo>& tabela,
    const string& nome
) {

    for (const auto& simbolo : tabela) {

        if (simbolo.nome == nome)
            return true;
    }

    return false;
}

int buscaSimbolo(
    const vector<Simbolo>& tabela,
    const string& nome
) {

    for (const auto& simbolo : tabela) {

        if (simbolo.nome == nome)
            return simbolo.endereco;
    }

    return -1;
}

bool ehGlobal(
    const vector<string>& globais,
    const string& nome
) {

    for (const auto& global : globais) {

        if (global == nome)
            return true;
    }

    return false;
}

bool lerModulo(
    const string& nomeArquivo,
    Modulo& modulo
) {

    ifstream arquivo(nomeArquivo);


    if (!arquivo.is_open()) {

        cout << "Erro ao abrir: "
             << nomeArquivo
             << endl;

        return false;
    }


    modulo.nome = nomeArquivo;


    string linha;
    string secao = "";


    while (getline(arquivo, linha)) {

        istringstream ss(linha);
        if (!(ss >> linha))
            continue;


        if (linha == "CODE") {

            secao = "CODE";

            continue;
        }


        if (linha == "SYMBOLS") {

            secao = "SYMBOLS";

            continue;
        }


        if (linha == "GLOBALS") {

            secao = "GLOBALS";

            continue;
        }


        if (linha == "REFERENCES") {

            secao = "REFERENCES";

            continue;
        }

        if (secao == "CODE") {

            int op;
            int arg1;
            int arg2;
            int arg3;


            op = stoi(linha);


            ss >> arg1
               >> arg2
               >> arg3;


            modulo.codigo.push_back({

                op,
                arg1,
                arg2,
                arg3

            });
        }

        else if (secao == "SYMBOLS") {

            string nome;
            int endereco;


            nome = linha;


            ss >> endereco;


            modulo.simbolos.push_back({

                nome,
                endereco

            });
        }

        else if (secao == "GLOBALS") {

            string nome;


            nome = linha;


            modulo.globais.push_back(nome);
        }

        else if (secao == "REFERENCES") {

            string nome;

            int enderecoInstrucao;

            int argumento;


            nome = linha;


            ss >> enderecoInstrucao
               >> argumento;


            modulo.referencias.push_back({

                nome,
                enderecoInstrucao,
                argumento

            });
        }
    }


    arquivo.close();


    return true;
}

void mostrarModulo(
    const Modulo& modulo
) {

    cout << endl;

    cout << "================================"
         << endl;

    cout << "Modulo: "
         << modulo.nome
         << endl;

    cout << "Base: "
         << modulo.base
         << endl;

    cout << "Tamanho: "
         << modulo.codigo.size()
         << endl;


    cout << endl;

    cout << "Simbolos:"
         << endl;


    for (const auto& simbolo :
         modulo.simbolos) {

        cout << "  "
             << simbolo.nome
             << " -> "
             << simbolo.endereco
             << endl;
    }


    cout << endl;

    cout << "Globais:"
         << endl;


    for (const auto& global :
         modulo.globais) {

        cout << "  "
             << global
             << endl;
    }


    cout << endl;

    cout << "Referencias:"
         << endl;


    for (const auto& referencia :
         modulo.referencias) {

        cout << "  "
             << referencia.nome
             << " -> instrucao "
             << referencia.enderecoInstrucao
             << ", argumento "
             << referencia.argumento
             << endl;
    }
}

int main(
    int argc,
    char* argv[]
) {

    if (argc < 2) {
        cout << "Uso: "
             << argv[0]
             << " arquivo1.txt arquivo2.txt ..."
             << endl;

        return 1;
    }


    /*
    ========================================================
                1. LER TODOS OS MODULOS
    ========================================================
    */

    vector<Modulo> modulos;


    for (int i = 1;
         i < argc;
         i++) {

        Modulo modulo;


        if (!lerModulo(
                argv[i],
                modulo)) {

            return 1;
        }


        modulos.push_back(modulo);
    }


    /*
    ========================================================
                2. CALCULAR AS BASES
    ========================================================
    */

    int enderecoAtual = 0;


    for (auto& modulo :
         modulos) {

        modulo.base =
            enderecoAtual;


        enderecoAtual +=
            modulo.codigo.size();
    }


    /*
    ========================================================
                3. CONSTRUIR TABELA GLOBAL
    ========================================================
    */

    vector<Simbolo> tabelaGlobal;


    for (const auto& modulo :
         modulos) {

        for (const auto& global :
             modulo.globais) {

            int enderecoLocal =
                buscaSimbolo(
                    modulo.simbolos,
                    global
                );

            if (enderecoLocal == -1) {

                cout << endl;

                cout << "Erro: simbolo global "
                     << global
                     << " nao encontrado no modulo "
                     << modulo.nome
                     << endl;

                return 1;
            }

            int enderecoGlobal =
                modulo.base +
                enderecoLocal;

            if (simboloExiste(
                    tabelaGlobal,
                    global)) {

                cout << endl;

                cout << "Erro: simbolo global duplicado: "
                     << global
                     << endl;

                return 1;
            }

            tabelaGlobal.push_back({

                global,
                enderecoGlobal

            });
        }
    }


    /*
    ========================================================
                4. MOSTRAR BASES DOS MODULOS
    ========================================================
    */

    cout << endl;

    cout << "BASE DOS MODULOS"
         << endl;


    for (const auto& modulo :
         modulos) {

        cout << modulo.nome
             << " -> base "
             << modulo.base
             << ", tamanho "
             << modulo.codigo.size()
             << endl;
    }


    /*
    ========================================================
                    5. TABELA GLOBAL
    ========================================================
    */

    cout << endl;

    cout << "TABELA GLOBAL"
         << endl;


    for (const auto& simbolo :
         tabelaGlobal) {

        cout << simbolo.nome
             << " -> "
             << simbolo.endereco
             << endl;
    }


    /*
    ========================================================
                6. RESOLVER REFERENCIAS
    ========================================================
    */

    for (auto& modulo :
         modulos) {


        for (const auto& referencia :
             modulo.referencias) {


            int endereco = -1;

            int enderecoLocal =
                buscaSimbolo(
                    modulo.simbolos,
                    referencia.nome
                );


            if (enderecoLocal != -1) {

                endereco =
                    modulo.base +
                    enderecoLocal;
            }


            else {

                endereco =
                    buscaSimbolo(
                        tabelaGlobal,
                        referencia.nome
                    );
            }

            if (endereco == -1) {

                cout << endl;

                cout << "Erro: simbolo nao encontrado: "
                     << referencia.nome
                     << endl;

                return 1;
            }


            /*
            ----------------------------------------------------
            Verifica endereco da instrucao.
            ----------------------------------------------------
            */

            if (
                referencia.enderecoInstrucao < 0
                ||
                referencia.enderecoInstrucao >=
                    (int)modulo.codigo.size()
            ) {

                cout << endl;

                cout << "Erro: endereco de instrucao invalido."
                     << endl;

                return 1;
            }
            
            if (referencia.argumento == 1) {

                modulo.codigo[
                    referencia.enderecoInstrucao
                ][1] = endereco;
            }


            else if (referencia.argumento == 2) {

                modulo.codigo[
                    referencia.enderecoInstrucao
                ][2] = endereco;
            }


            else if (referencia.argumento == 3) {

                modulo.codigo[
                    referencia.enderecoInstrucao
                ][3] = endereco;
            }


            else {

                cout << "Erro: numero de argumento invalido."
                     << endl;

                return 1;
            }
        }
    }


    /*
    ========================================================
                7. GERAR CODIGO FINAL
    ========================================================
    */

    ofstream saida(
        "saida_binario.txt"
    );


    if (!saida.is_open()) {

        cout << "Erro ao criar saida_binario.txt"
             << endl;

        return 1;
    }

    for (const auto& modulo :
         modulos) {

        for (const auto& instrucao :
             modulo.codigo) {

            saida
                << instrucao[0]
                << " "
                << instrucao[1]
                << " "
                << instrucao[2]
                << " "
                << instrucao[3]
                << endl;
        }
    }


    saida.close();


    /*
    ========================================================
                         RESULTADO
    ========================================================
    */

    cout << endl;

    cout << "Ligacao concluida."
         << endl;


    cout << "Arquivo gerado: "
         << "saida_binario.txt"
         << endl;

    cout << endl;

    cout << "Codigo final:"
         << endl;


    int endereco = 0;


    for (const auto& modulo :
         modulos) {

        for (const auto& instrucao :
             modulo.codigo) {

            cout << endereco
                 << ": "
                 << instrucao[0]
                 << " "
                 << instrucao[1]
                 << " "
                 << instrucao[2]
                 << " "
                 << instrucao[3]
                 << endl;

            endereco++;
        }
    }

}