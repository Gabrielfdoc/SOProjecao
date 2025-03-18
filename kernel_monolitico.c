#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define NOME_MAX 100

// Função para criar um arquivo fisicamente
void criar_arquivo(const char* nome, const char* conteudo) {
    FILE *arquivo = fopen(nome, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo %s.\n", nome);
        return;
    }
    fprintf(arquivo, "%s", conteudo);
    fclose(arquivo);  
    printf("Arquivo %s criado com sucesso.\n", nome);
}

// Função para ler um arquivo
void ler_arquivo(const char* nome) {
    FILE *arquivo = fopen(nome, "r");
    if (arquivo == NULL) {
        printf("Erro: O arquivo %s não foi encontrado.\n", nome);
        return;
    }
    char conteudo[1024];
    printf("Conteúdo do arquivo %s:\n", nome);
    while (fgets(conteudo, sizeof(conteudo), arquivo) != NULL) {
        printf("%s", conteudo);
    }
    fclose(arquivo);
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
    DIR *dir = opendir(".");
    if (dir == NULL) {
        printf("Erro ao abrir o diretório.\n");
        return;
    }
    struct dirent *entry;
    printf("Arquivos no diretório atual:\n");
    while ((entry = readdir(dir)) != NULL) {
        printf("- %s\n", entry->d_name);
    }
    closedir(dir);
}

int main() {
    // Operações monolíticas, todas em um único bloco
    criar_arquivo("arquivo1.txt", "Conteúdo do arquivo 1.");
    ler_arquivo("arquivo1.txt");
    excluir_arquivo("arquivo1.txt");
    listar_arquivos();

    printf("Pressione ENTER para sair...\n");
    getchar(); 

    return 0;
}
