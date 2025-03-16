#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_ARQUIVOS 10
#define NOME_MAX 100
#define MAX_MENSAGENS 10

// Estrutura da mensagem
typedef struct {
    char comando[10];  // Comando (criar, ler, excluir, listar)
    char nome[NOME_MAX];
    char conteudo[NOME_MAX];
} Mensagem;

// Estrutura do Microkernel
typedef struct {
    Mensagem fila_mensagens[MAX_MENSAGENS];
    int inicio, fim;
} Microkernel;

// Protótipos das funções
void criar_arquivo(const char* nome, const char* conteudo);
void ler_arquivo(const char* nome);
void excluir_arquivo(const char* nome);
void listar_arquivos();
void init_microkernel(Microkernel* kernel);
int adicionar_mensagem(Microkernel* kernel, Mensagem mensagem);
int processar_mensagem(Microkernel* kernel);
void enviar_mensagem(Microkernel* kernel, const char* comando, const char* nome, const char* conteudo);

void init_microkernel(Microkernel* kernel) {
    kernel->inicio = 0;
    kernel->fim = 0;
}

// Função para adicionar uma mensagem na fila
int adicionar_mensagem(Microkernel* kernel, Mensagem mensagem) {
    if ((kernel->fim + 1) % MAX_MENSAGENS == kernel->inicio) {
        printf("Erro: Fila de mensagens cheia.\n");
        return -1;
    }
    kernel->fila_mensagens[kernel->fim] = mensagem;
    kernel->fim = (kernel->fim + 1) % MAX_MENSAGENS;
    return 0;
}

// Função para processar uma mensagem da fila
int processar_mensagem(Microkernel* kernel) {
    if (kernel->inicio == kernel->fim) {
        printf("Erro: Fila de mensagens vazia.\n");
        return -1;
    }

    Mensagem msg = kernel->fila_mensagens[kernel->inicio];
    kernel->inicio = (kernel->inicio + 1) % MAX_MENSAGENS;

    // Processando a mensagem
    if (strcmp(msg.comando, "criar") == 0) {
        criar_arquivo(msg.nome, msg.conteudo);
    } else if (strcmp(msg.comando, "ler") == 0) {
        ler_arquivo(msg.nome);
    } else if (strcmp(msg.comando, "excluir") == 0) {
        excluir_arquivo(msg.nome);
    } else if (strcmp(msg.comando, "listar") == 0) {
        listar_arquivos();
    } else {
        printf("Erro: Comando inválido %s.\n", msg.comando);
    }
    return 0;
}

// Função para criar um arquivo fisicamente
void criar_arquivo(const char* nome, const char* conteudo) {
    FILE *arquivo = fopen(nome, "w");  // Abre o arquivo para escrita (se não existir, cria)
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo %s.\n", nome);
        return;
    }
    fprintf(arquivo, "%s", conteudo);  // Escreve o conteúdo no arquivo
    fclose(arquivo);  
    printf("Arquivo %s criado com sucesso.\n", nome);
}

// Função para ler um arquivo
void ler_arquivo(const char* nome) {
    FILE *arquivo = fopen(nome, "r");  // Abre o arquivo para leitura
    if (arquivo == NULL) {
        printf("Erro: O arquivo %s não foi encontrado.\n", nome);
        return;
    }
    char conteudo[1024];
    printf("Conteúdo do arquivo %s:\n", nome);
    while (fgets(conteudo, sizeof(conteudo), arquivo) != NULL) {
        printf("%s", conteudo);  // Exibe o conteúdo do arquivo
    }
    fclose(arquivo);  // Fecha o arquivo
}

// Função para excluir um arquivo
void excluir_arquivo(const char* nome) {
    if (remove(nome) == 0) {
        printf("Arquivo %s excluído com sucesso.\n", nome);
    } else {
        printf("Erro ao excluir o arquivo %s.\n", nome);
    }
}

// Função para listar arquivos no diretório atual
void listar_arquivos() {
    DIR *dir = opendir(".");  // Abre o diretório atual
    if (dir == NULL) {
        printf("Erro ao abrir o diretório.\n");
        return;
    }

    struct dirent *entry;
    printf("Arquivos no diretório atual:\n");
    while ((entry = readdir(dir)) != NULL) {
        printf("- %s\n", entry->d_name);  // Exibe o nome do arquivo
    }
    closedir(dir);  // Fecha o diretório
}

// Função para enviar uma mensagem para o microkernel
void enviar_mensagem(Microkernel* kernel, const char* comando, const char* nome, const char* conteudo) {
    Mensagem msg;
    strcpy(msg.comando, comando);
    if (nome != NULL) strcpy(msg.nome, nome);
    if (conteudo != NULL) strcpy(msg.conteudo, conteudo);

    if (adicionar_mensagem(kernel, msg) == 0) {
        printf("Mensagem '%s' enviada para o kernel.\n", comando);
    }
}

int main() {
    Microkernel kernel;
    init_microkernel(&kernel);

    // Enviar comandos para o serviço
    enviar_mensagem(&kernel, "criar", "arquivo1.txt", "Conteúdo do arquivo 1.");
    enviar_mensagem(&kernel, "criar", "arquivo2.txt", "Conteúdo do arquivo 2.");
    enviar_mensagem(&kernel, "ler", "arquivo1.txt", NULL);
    enviar_mensagem(&kernel, "excluir", "arquivo2.txt", NULL);
    enviar_mensagem(&kernel, "listar", NULL, NULL);

    // Processar as mensagens na fila
    processar_mensagem(&kernel);
    processar_mensagem(&kernel);
    processar_mensagem(&kernel);
    processar_mensagem(&kernel);
    processar_mensagem(&kernel);


    printf("Pressione ENTER para sair...\n");
    getchar(); 
    
    return 0;
}
