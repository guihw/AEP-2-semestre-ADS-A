#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define MAX_USUARIOS 100
#define TAM_SENHA 100
#define ARQUIVO "usuarios.txt"

typedef struct {
    char nome[50];
    char senha[TAM_SENHA];
} Usuario;

Usuario usuarios[MAX_USUARIOS];
int totalUsuarios = 0;

void rot13_extendido(char str[]) {
    size_t i;
    for (i = 0; i < strlen(str); i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = ((str[i] - 'a' + 13) % 26) + 'a';
        } else if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = ((str[i] - 'A' + 13) % 26) + 'A';
        } else if (str[i] >= '0' && str[i] <= '9') {
            str[i] = ((str[i] - '0' + 5) % 10) + '0';
        }
    }
}

int validarSenha(char senha[]) {
    int temMaiuscula = 0, temNumero = 0;
    size_t i;

    if (strlen(senha) < 6) {
        printf("Erro: A senha deve ter pelo menos 6 caracteres.\n");
        return 0;
    }

    for (i = 0; i < strlen(senha); i++) {
        if (isupper(senha[i])) temMaiuscula = 1;
        if (isdigit(senha[i])) temNumero = 1;
    }

    if (!temMaiuscula) {
        printf("Erro: A senha deve conter pelo menos uma letra maiúscula.\n");
    }
    if (!temNumero) {
        printf("Erro: A senha deve conter pelo menos um número.\n");
    }

    return temMaiuscula && temNumero;
}

void salvarUsuarios() {
    FILE *arquivo = fopen(ARQUIVO, "w");
    int i;
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    for (i = 0; i < totalUsuarios; i++) {
        fprintf(arquivo, "%s %s\n", usuarios[i].nome, usuarios[i].senha);
    }
    fclose(arquivo);
}

void carregarUsuarios() {
    FILE *arquivo = fopen(ARQUIVO, "r");
    if (arquivo != NULL) {
        while (fscanf(arquivo, "%s %s", usuarios[totalUsuarios].nome, usuarios[totalUsuarios].senha) != EOF) {
            totalUsuarios++;
        }
        fclose(arquivo);
    }
}

void adicionarUsuario() {
    if (totalUsuarios >= MAX_USUARIOS) {
        printf("Limite de usuários atingido!\n");
        return;
    }

    Usuario novo;
    printf("Nome do usuário: ");
    scanf("%s", novo.nome);

    do {
        printf("Senha (mínimo 6 caracteres, uma letra maiúscula e um número): ");
        scanf("%s", novo.senha);
    } while (!validarSenha(novo.senha));

    rot13_extendido(novo.senha);
    usuarios[totalUsuarios++] = novo;
    salvarUsuarios();
    printf("Usuário adicionado com sucesso!\n");
}

void listarUsuarios() {
    int i;
    printf("Lista de usuários:\n");
    for (i = 0; i < totalUsuarios; i++) {
        printf("Nome: %s, Senha criptografada: %s\n", usuarios[i].nome, usuarios[i].senha);
    }
}

void excluirUsuario() {
    char nome[50];
    int i, j;
    printf("Nome do usuário a ser excluído: ");
    scanf("%s", nome);

    int encontrado = 0;
    for (i = 0; i < totalUsuarios; i++) {
        if (strcmp(usuarios[i].nome, nome) == 0) {
            for (j = i; j < totalUsuarios - 1; j++) {
                usuarios[j] = usuarios[j + 1];
            }
            totalUsuarios--;
            salvarUsuarios();
            printf("Usuário excluído com sucesso!\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Usuário não encontrado!\n");
    }
}

void atualizarSenha() {
    char nome[50], novaSenha[TAM_SENHA];
    int i;
    printf("Nome do usuário: ");
    scanf("%s", nome);

    int encontrado = 0;
    for (i = 0; i < totalUsuarios; i++) {
        if (strcmp(usuarios[i].nome, nome) == 0) {
            do {
                printf("Nova senha (mínimo 6 caracteres, uma letra maiúscula e um número): ");
                scanf("%s", novaSenha);
            } while (!validarSenha(novaSenha));

            rot13_extendido(novaSenha);
            strcpy(usuarios[i].senha, novaSenha);
            salvarUsuarios();
            printf("Senha atualizada com sucesso!\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Usuário não encontrado!\n");
    }
}

void menu() {
    int opcao;
    do {
        printf("\n--- MENU ---\n");
        printf("1. Adicionar usuário\n");
        printf("2. Listar usuários\n");
        printf("3. Atualizar senha\n");
        printf("4. Excluir usuário\n");
        printf("5. Voltar\n");
        printf("\n-------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionarUsuario();
                break;
            case 2:
                listarUsuarios();
                break;
            case 3:
                atualizarSenha();
                break;
            case 4:
                excluirUsuario();
                break;
            case 5:
                printf("Voltando...\n");
                return;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (1);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    carregarUsuarios();
    int opcao;

    do {
        printf("\n--- SISTEMA DE GERENCIAMENTO ---\n");
        printf("1. Abrir menu\n");
        printf("2. Sair\n");
        printf("\n--------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            menu();
        } else if (opcao == 2) {
            printf("Encerrando o programa...\n");
            break;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    } while (1);

    return 0;
}
