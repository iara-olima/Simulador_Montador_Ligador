#include <stdio.h>

typedef enum {
    IN,
    MOV,
    ADD,
    STORE,
    OUT,
    JUMP,
    STOP
} Opcodes;

typedef struct {
    int op;
    int arg1;
    int arg2;
} Instrucao;

int op, arg1, arg2,i=0;
Instrucao mem[256];
int dadosMem[64];


int pc = 0, continua=1;
int reg[2]; 

void memoria(){
    FILE * f ;
    f = fopen ("simulador.txt", "r");

    while (fscanf (f, "%d %d %d", &op, &arg1, &arg2)==3) {
        mem[i].op = op;
        mem[i].arg1 = arg1;
        mem[i].arg2 = arg2;
        i++;
    }
    fclose(f);
}

void cpu(){
    while(continua){
        switch (mem[pc].op)
        {
            case IN:
                printf("Digite um valor: ");
                scanf("%d", &dadosMem[mem[pc].arg1]);
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
                printf("resultado: %d\n", dadosMem[mem[pc].arg1]);
                pc++;
                break;        
            case JUMP:
                printf("\n\tVOLTANDO\t\n");
                printf("  _____  \n");
                printf(" /     \\ \n");
                printf("| '   ' |\n");
                printf("|   )   |\n");
                printf("| \\___/ |\n");
                printf(" \\_____/\n");
                printf("\n");
                pc = mem[pc].arg1;
                break;
            case STOP:
                continua=0;
                break;
            default:
                break;
        }
    }
}

int main (){
    memoria();
    cpu();
   
}