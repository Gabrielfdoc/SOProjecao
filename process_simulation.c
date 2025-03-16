#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

HANDLE semaforo_cpu;
HANDLE semaforo_impressora;
HANDLE semaforo_memoria;

DWORD WINAPI usar_cpu(LPVOID arg) {
    int id_processo = *((int*)arg);
    printf("O processo %d está usando a CPU.\n", id_processo);
    Sleep(rand() % 3 + 1);  // Simula o uso da CPU por um tempo aleatório entre 1 e 3 segundos
    printf("O processo %d terminou de usar a CPU.\n", id_processo);
    return 0;
}

DWORD WINAPI usar_impressora(LPVOID arg) {
    int id_processo = *((int*)arg);
    printf("O processo %d está usando a impressora.\n", id_processo);
    Sleep(rand() % 3 + 1);  // Simula o uso da impressora por um tempo aleatório entre 1 e 3 segundos
    printf("O processo %d terminou de usar a impressora.\n", id_processo);
    return 0;
}

DWORD WINAPI usar_memoria(LPVOID arg) {
    int id_processo = *((int*)arg);
    printf("O processo %d está usando a memória.\n", id_processo);
    Sleep(rand() % 3 + 1);  // Simula o uso de memória por um tempo aleatório entre 1 e 3 segundos
    printf("O processo %d terminou de usar a memória.\n", id_processo);
    return 0;
}

// Função que simula um processo usando a CPU, memória e impressora sequencialmente
DWORD WINAPI processo(LPVOID arg) {
    int id_processo = *((int*)arg);

    // Simula o uso da CPU
    WaitForSingleObject(semaforo_cpu, INFINITE);  // Aguarda para acessar a CPU
    usar_cpu(arg);
    ReleaseSemaphore(semaforo_cpu, 1, NULL);  // Libera a CPU

    // Simula o uso da memória
    WaitForSingleObject(semaforo_memoria, INFINITE);  // Aguarda para acessar a memória
    usar_memoria(arg);
    ReleaseSemaphore(semaforo_memoria, 1, NULL);  // Libera a memória

    // Simula o uso da impressora
    WaitForSingleObject(semaforo_impressora, INFINITE);  // Aguarda para acessar a impressora
    usar_impressora(arg);
    ReleaseSemaphore(semaforo_impressora, 1, NULL);  // Libera a impressora

    return 0;
}

int main() {
    srand(time(NULL));  // Inicializa o gerador de números aleatórios para os tempos de espera

    // Inicializa os semáforos (semáforos binários com contador inicial 1)
    semaforo_cpu = CreateSemaphore(NULL, 1, 1, NULL);
    semaforo_impressora = CreateSemaphore(NULL, 1, 1, NULL);
    semaforo_memoria = CreateSemaphore(NULL, 1, 1, NULL);

    int num_processos = 5;
    HANDLE threads[num_processos];

    // Cria e inicia as threads (processos)
    for (int i = 0; i < num_processos; i++) {
        int* id_processo = malloc(sizeof(int));  // Aloca memória para o ID do processo
        *id_processo = i;
        threads[i] = CreateThread(NULL, 0, processo, id_processo, 0, NULL);
        if (threads[i] == NULL) {
            perror("Erro ao criar thread");
            exit(1);
        }
    }

    // Aguarda todas as threads (processos) terminarem
    WaitForMultipleObjects(num_processos, threads, TRUE, INFINITE);

    // Limpeza: Fecha os handles das threads
    for (int i = 0; i < num_processos; i++) {
        CloseHandle(threads[i]);
    }

    CloseHandle(semaforo_cpu);
    CloseHandle(semaforo_impressora);
    CloseHandle(semaforo_memoria);

    getchar(); 

    return 0;
}
