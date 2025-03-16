#include <stdio.h>
#include <stdbool.h>
#include <conio.h>    // Para kbhit() e getch()
#include <windows.h>  // Para Sleep() e GetTickCount()

// Definição do dispositivo
typedef struct {
    bool necessita_atencao;
} Dispositivo;

// Função para verificar o estado do dispositivo (Poolling)
bool verificar_dispositivo(Dispositivo *dispositivo) {
    return dispositivo->necessita_atencao;
}

// Função para simular a interrupção, modificando a flag
void sinalizar_interrupcao(Dispositivo *dispositivo) {
    dispositivo->necessita_atencao = true;
}

// Função que implementa o poolling (verificação contínua do dispositivo)
void loop_pooling(Dispositivo *dispositivo) {
    while (1) {
        // Verifica se o dispositivo precisa de atenção
        if (verificar_dispositivo(dispositivo)) {
            printf("Dispositivo precisa de atenção! (Poolling)\n");
        } else {
            printf("Nenhuma necessidade de atenção. (Poolling)\n");
        }

        // Sempre informa ao usuário que ele pode pressionar 'Q' para sair
        printf("Pressione 'Q' para sair.\n");

        // Verificar se o usuário pressionou uma tecla para sair
        if (kbhit()) {
            char ch = getch(); 
            if (ch == 'q' || ch == 'Q') {
                printf("Saindo do loop...\n");
                break;  
            }
        }

        
        Sleep(1000);  
    }
}

int main() {
    // Criando o dispositivo, inicialmente sem necessidade de atenção
    Dispositivo dispositivo = {false};

    // Simulando a situação onde o dispositivo não precisa de atenção
    printf("Iniciando com dispositivo sem necessidade de atenção...\n");

    // Usar GetTickCount() para controlar o tempo (em milissegundos)
    unsigned long start_time = GetTickCount();  // Tempo de início em milissegundos

    // Rodando o loop de poolling
    while (1) {
        // Verifica o tempo passado
        unsigned long current_time = GetTickCount();
        
        // Se passaram 5 segundos (5000 ms), o dispositivo começa a precisar de atenção
        if (current_time - start_time >= 5000) {
            if (!dispositivo.necessita_atencao) {
                printf("Agora o dispositivo precisa de atenção...\n");
                sinalizar_interrupcao(&dispositivo);
            }
        }

        // Verifica se o dispositivo precisa de atenção
        if (verificar_dispositivo(&dispositivo)) {
            printf("Dispositivo precisa de atenção! (Polling)\n");
        } else {
            printf("Nenhuma necessidade de atenção. (Polling)\n");
        }

        // Sempre informa ao usuário que ele pode pressionar 'Q' para sair
        printf("Pressione 'Q' para sair.\n");

        // Verificar se o usuário pressionou uma tecla para sair
        if (kbhit()) {
            char ch = getch();  
            if (ch == 'q' || ch == 'Q') {
                printf("Saindo do loop...\n");
                break;  
            }
        }

        // Aguarda 1 segundo antes de verificar novamente
        Sleep(1000);  // Sleep em milissegundos no Windows
    }

    return 0;
}
