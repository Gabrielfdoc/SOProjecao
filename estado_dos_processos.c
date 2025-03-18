#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Definindo os estados
#define PRONTO "Pronto"
#define EXECUCAO "Execução"
#define BLOQUEADO "Bloqueado"

// Estrutura para representar um processo
typedef struct {
    int id;
    const char* estado; 
} Processo;

// Função para mudar o estado do processo aleatoriamente
void mudar_estado(Processo* p) {
    const char* novo_estado;
    do {
        switch (rand() % 3) {
            case 0: novo_estado = PRONTO; break;
            case 1: novo_estado = EXECUCAO; break;
            case 2: novo_estado = BLOQUEADO; break;
        }
    } while (novo_estado == p->estado);
    
    p->estado = novo_estado;  // Atribuindo o novo estado
}

// Função para simular a execução dos processos
void simular_processos(int num_processos, int ciclos) {
    // Array de processos
    Processo processos[num_processos];
    
    // Inicializando os processos com id e estado inicial como "Pronto"
    for (int i = 0; i < num_processos; i++) {
        processos[i].id = i + 1;
        processos[i].estado = PRONTO;
    }

    // Simulando pelos ciclos especificados
    for (int ciclo = 0; ciclo < ciclos; ciclo++) {
        printf("\nCiclo %d:\n", ciclo + 1);
        
        // Exibindo o estado atual de cada processo
        for (int i = 0; i < num_processos; i++) {
            printf("Processo %d - Estado: %s\n", processos[i].id, processos[i].estado);
        }

        // Mudando o estado de cada processo
        for (int i = 0; i < num_processos; i++) {
            mudar_estado(&processos[i]);
        }

        //Simula o tempo passando
        sleep(2);
    }
}

int main() {
    // Inicializar a semente aleatória
    srand(time(NULL));

    // Número de processos e ciclos da simulação
    int num_processos = 5;
    int ciclos = 10;

    // Iniciar a simulação
    simular_processos(num_processos, ciclos);

    return 0;
}
