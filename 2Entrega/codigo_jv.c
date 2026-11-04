
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estrutura para armazenamento das Pistas
typedef struct {
    int id_pista;
    char nome[100];
    char pais[50];
    int numero_voltas;
} Pista;

// Estrutura para armazenamento das Corridas
typedef struct {
    int id_corrida;
    char nome[100];
    int id_pista;
    char data[11];
} Corrida;

// Estrutura para armazenamento dos Resultados
typedef struct {
    int id_resultado;
    int id_corrida;
    int id_piloto;
    int posicao;
    int pontos;
} Resultado;

// Vetores para armazenar os dados
Pista pistas[100];
Corrida corridas[100];
Resultado resultados[500];

int totalPistas = 0;
int totalCorridas = 0;
int totalResultados = 0;

// Verifica se uma string está vazia
int textoVazio(char texto[]) {
    for (int i = 0; texto[i] != '\0'; i++) {
        if (!isspace((unsigned char)texto[i])) {
            return 0;
        }
    }
    return 1;
}

// Procura uma pista pelo ID
int buscarPista(int id) {
    for (int i = 0; i < totalPistas; i++) {
        if (pistas[i].id_pista == id) {
            return i;
        }
    }
    return -1;
}

// Valida o formato e a data
int validarData(char data[]) {
    int dia, mes, ano;

    if (strlen(data) != 10 ||
        data[2] != '/' || data[5] != '/') {
        return 0;
    }

    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit((unsigned char)data[i])) {
            return 0;
        }
    }

    sscanf(data, "%2d/%2d/%4d", &dia, &mes, &ano);

    if (ano < 1900 || mes < 1 || mes > 12 || dia < 1) {
        return 0;
    }

    int diasMes[] = {31, 28, 31, 30, 31, 30,
                     31, 31, 30, 31, 30, 31};

    // Verifica ano bissexto
    if ((ano % 400 == 0) ||
        (ano % 4 == 0 && ano % 100 != 0)) {
        diasMes[1] = 29;
    }

    if (dia > diasMes[mes - 1]) {
        return 0;
    }

    return 1;
}

// Cadastrar pista
void cadastrarPista() {
    if (totalPistas >= 100) {
        printf("\nLimite de pistas atingido!\n");
        return;
    }

    Pista nova;

    printf("\n--- CADASTRO DE PISTA ---\n");

    nova.id_pista = totalPistas + 1;

    printf("Nome da pista: ");
    scanf(" %99[^\n]", nova.nome);

    printf("Pais: ");
    scanf(" %49[^\n]", nova.pais);

    printf("Numero de voltas: ");
    scanf("%d", &nova.numero_voltas);

    if (textoVazio(nova.nome) ||
        textoVazio(nova.pais) ||
        nova.numero_voltas <= 0) {
        printf("\nDados invalidos! Pista nao cadastrada.\n");
        return;
    }

    pistas[totalPistas] = nova;
    totalPistas++;

    printf("\nPista cadastrada com sucesso!\n");
    printf("ID da pista: %d\n", nova.id_pista);
}

// Listar pistas
void listarPistas() {
    printf("\n--- LISTA DE PISTAS ---\n");

    if (totalPistas == 0) {
        printf("Nenhuma pista cadastrada.\n");
        return;
    }

    for (int i = 0; i < totalPistas; i++) {
        printf("\nID: %d\n", pistas[i].id_pista);
        printf("Nome: %s\n", pistas[i].nome);
        printf("Pais: %s\n", pistas[i].pais);
        printf("Voltas: %d\n", pistas[i].numero_voltas);
    }
}

// Validação dos dados de uma corrida
int validarCorrida(Corrida nova) {
    if (textoVazio(nova.nome)) {
        printf("\nErro: nome da corrida vazio.\n");
        return 0;
    }

    if (buscarPista(nova.id_pista) == -1) {
        printf("\nErro: pista nao cadastrada.\n");
        return 0;
    }

    if (!validarData(nova.data)) {
        printf("\nErro: data invalida. Use DD/MM/AAAA.\n");
        return 0;
    }

    for (int i = 0; i < totalCorridas; i++) {
        if (strcmp(corridas[i].nome, nova.nome) == 0 &&
            corridas[i].id_pista == nova.id_pista &&
            strcmp(corridas[i].data, nova.data) == 0) {
            printf("\nErro: corrida ja cadastrada.\n");
            return 0;
        }
    }

    return 1;
}

// Cadastrar corrida
void cadastrarCorrida() {
    if (totalCorridas >= 100) {
        printf("\nLimite de corridas atingido!\n");
        return;
    }

    Corrida nova;

    printf("\n--- CADASTRO DE CORRIDA ---\n");

    nova.id_corrida = totalCorridas + 1;

    printf("Nome da corrida: ");
    scanf(" %99[^\n]", nova.nome);

    printf("\nPistas disponiveis:\n");
    listarPistas();

    printf("\nDigite o ID da pista: ");
    scanf("%d", &nova.id_pista);

    printf("Data da corrida (DD/MM/AAAA): ");
    scanf(" %10s", nova.data);

    if (validarCorrida(nova)) {
        corridas[totalCorridas] = nova;
        totalCorridas++;

        printf("\nCorrida cadastrada com sucesso!\n");
        printf("ID da corrida: %d\n", nova.id_corrida);
    } else {
        printf("\nCadastro cancelado.\n");
    }
}

// Listar corridas
void listarCorridas() {
    printf("\n--- LISTA DE CORRIDAS ---\n");

    if (totalCorridas == 0) {
        printf("Nenhuma corrida cadastrada.\n");
        return;
    }

    for (int i = 0; i < totalCorridas; i++) {
        int indice = buscarPista(corridas[i].id_pista);

        printf("\nID da corrida: %d\n", corridas[i].id_corrida);
        printf("Nome: %s\n", corridas[i].nome);

        if (indice != -1) {
            printf("Pista: %s\n", pistas[indice].nome);
        }

        printf("Data: %s\n", corridas[i].data);
    }
}

// Menu principal
int main() {
    int opcao;

    do {
        printf("\n========== MENU ==========\n");
        printf("1 - Cadastrar pista\n");
        printf("2 - Listar pistas\n");
        printf("3 - Cadastrar corrida\n");
        printf("4 - Listar corridas\n");
        printf("0 - Sair\n");
        printf("==========================\n");

        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarPista();
                break;

            case 2:
                listarPistas();
                break;

            case 3:
                cadastrarCorrida();
                break;

            case 4:
                listarCorridas();
                break;

            case 0:
                printf("\nEncerrando programa...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
