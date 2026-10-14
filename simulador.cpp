#include <iostream>
#include <fstream>

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
    BLT,
    END = -1
};

struct Instrucao {
    int op;
    int arg1;
    int arg2;
    int arg3;
};

Instrucao mem[256];
int dadosMem[64];
int pc = 0;
bool continua = true;

int reg[2];

void memoria(char *argv[]) {

    ifstream arquivo(argv[1]);

    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo." << endl;
        return;
    }

    int op, arg1, arg2, arg3;
    int i = 0;

    while (arquivo >> op >> arg1 >> arg2 >> arg3) {

        mem[i].op = op;
        mem[i].arg1 = arg1;
        mem[i].arg2 = arg2;
        mem[i].arg3 = arg3;

        i++;
    }
    arquivo.close();
}

void cpu() {
    while (continua) {
        if (mem[pc].op == -1) {
            pc++;
            continue;
        }
        switch (mem[pc].op) {
            case IN:
                cout << "Digite um valor: ";
                cin >> dadosMem[mem[pc].arg1];
                pc++;
                break;
            case MOV:
                reg[mem[pc].arg1] = dadosMem[mem[pc].arg2];
                pc++;
                break;
            case ADD:
                reg[mem[pc].arg1] = reg[mem[pc].arg1] + reg[mem[pc].arg2];
                pc++;
                break;
            case STORE:
                dadosMem[mem[pc].arg1] = reg[mem[pc].arg2];
                pc++;
                break;
            case OUT:
                cout << "resultado: "
                     << dadosMem[mem[pc].arg1]
                     << endl;
                pc++;
                break;
            case JUMP:
                cout << "\n\tVOLTANDO\t\n";
                cout << " _____  \n";
                cout << "/     \\ \n";
                cout << "| '   ' |\n";
                cout << "|   )   |\n";
                cout << "| \\___/ |\n";
                cout << " \\_____/\n";
                cout << "\n";
                pc = mem[pc].arg1;
                break;
            case STOP:
                continua = false;
                break;
            case SUB:
                reg[mem[pc].arg1] =
                    reg[mem[pc].arg1] - reg[mem[pc].arg2];

                pc++;
                break;
            case MUL:
                reg[mem[pc].arg1] =
                    reg[mem[pc].arg1] * reg[mem[pc].arg2];

                pc++;
                break;
            case DIV:
                reg[mem[pc].arg1] = reg[mem[pc].arg1] / reg[mem[pc].arg2];
                pc++;
                break;
            case BR:
                pc = mem[pc].arg1;
                break;
            case BEQ:
                if (reg[mem[pc].arg1] == reg[mem[pc].arg2]) {
                    pc = mem[pc].arg3;
                }
                else {
                    pc++;
                }
                break;
            case BGT:
                if (reg[mem[pc].arg1] > reg[mem[pc].arg2]) {
                    pc = mem[pc].arg3;
                }
                else {
                    pc++;
                }
                break;
            case BLT:
                if (reg[mem[pc].arg1] < reg[mem[pc].arg2]) {
                    pc = mem[pc].arg3;
                }
                else {
                    pc++;
                }
                break;
                case END:
                continua = false;
                break;
            default:
                cout << "Opcode invalido: "
                     << mem[pc].op
                     << endl;
                continua = false;
                break;
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        cout << "Quantidade de argumentos invalida" << endl;
        return 1;
    }

    memoria(argv);
    cpu();
}