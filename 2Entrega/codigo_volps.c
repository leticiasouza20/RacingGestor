#include <stdio.h>

#define MAX_PILOTOS 50
#define MAX_CORRIDAS 100
#define MAX_RESULTADOS 500
#define MAX_POSICOES 20

/* ---------- STRUCTS ---------- */

typedef struct {
    int id_piloto;
    char nome[100];
    int numero;
    char nacionalidade[50];
    int id_equipe;
    int pontos;
} Piloto;

typedef struct {
    int id_corrida;
    char nome[100];
    int id_pista;
    char data[11];
} Corrida;

typedef struct {
    int id_resultado;
    int id_corrida;
    int id_piloto;
    int posicao;
    int pontos;
} Resultado;

/* ---------- VETORES ---------- */

Piloto pilotos[MAX_PILOTOS];
int totalPilotos = 0;

Corrida corridas[MAX_CORRIDAS];
int totalCorridas = 0;

Resultado resultados[MAX_RESULTADOS];
int totalResultados = 0;

/* ---------- BUSCAS ---------- */

/* Procura um piloto pelo codigo */
int buscarPiloto(int id) {
    for (int i = 0; i < totalPilotos; i++) {
        if (pilotos[i].id_piloto == id) {
            return i;
        }
    }
    return -1;
}

/* Procura uma corrida pelo codigo */
int buscarCorrida(int id) {
    for (int i = 0; i < totalCorridas; i++) {
        if (corridas[i].id_corrida == id) {
            return i;
        }
    }
    return -1;
}

/* ---------- PONTUACAO ---------- */

/* Pontuacao da Formula 1: apenas os 10 primeiros pontuam */
int pontosPorPosicao(int posicao) {
    int tabela[] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};

    if (posicao < 1 || posicao > 10) {
        return 0;
    }
    return tabela[posicao - 1];
}

/* ---------- VALIDACOES ---------- */

/* Verifica se a posicao ja foi usada por outro piloto na mesma corrida */
int posicaoJaUsada(int id_corrida, int posicao) {
    for (int i = 0; i < totalResultados; i++) {
        if (resultados[i].id_corrida == id_corrida &&
            resultados[i].posicao == posicao) {
            return 1;
        }
    }
    return 0;
}

/* Verifica se o piloto ja tem resultado nessa corrida */
int resultadoJaCadastrado(int id_corrida, int id_piloto) {
    for (int i = 0; i < totalResultados; i++) {
        if (resultados[i].id_corrida == id_corrida &&
            resultados[i].id_piloto == id_piloto) {
            return 1;
        }
    }
    return 0;
}

/* ---------- REGISTRAR RESULTADO ---------- */

void registrarResultado() {
    int id_corrida;
    int id_piloto;
    int posicao;
    int indice_corrida;
    int indice_piloto;
    Resultado novo;

    printf("\n--- REGISTRAR RESULTADO ---\n");

    if (totalResultados >= MAX_RESULTADOS) {
        printf("Erro: limite de resultados atingido.\n");
        return;
    }

    /* corrida precisa existir */
    printf("Codigo da corrida: ");
    scanf("%d", &id_corrida);

    indice_corrida = buscarCorrida(id_corrida);
    if (indice_corrida == -1) {
        printf("Erro: corrida nao encontrada.\n");
        return;
    }

    /* piloto precisa existir */
    printf("Codigo do piloto: ");
    scanf("%d", &id_piloto);

    indice_piloto = buscarPiloto(id_piloto);
    if (indice_piloto == -1) {
        printf("Erro: piloto nao encontrado.\n");
        return;
    }

    /* piloto nao pode repetir na mesma corrida */
    if (resultadoJaCadastrado(id_corrida, id_piloto)) {
        printf("Erro: esse piloto ja tem resultado nesta corrida.\n");
        return;
    }

    /* posicao valida */
    printf("Posicao de chegada (1 a %d): ", MAX_POSICOES);
    scanf("%d", &posicao);

    if (posicao < 1 || posicao > MAX_POSICOES) {
        printf("Erro: a posicao deve estar entre 1 e %d.\n", MAX_POSICOES);
        return;
    }

    /* posicao nao pode repetir na mesma corrida */
    if (posicaoJaUsada(id_corrida, posicao)) {
        printf("Erro: essa posicao ja foi registrada nesta corrida.\n");
        return;
    }

    /* monta e salva o resultado */
    novo.id_resultado = totalResultados + 1;
    novo.id_corrida = id_corrida;
    novo.id_piloto = id_piloto;
    novo.posicao = posicao;
    novo.pontos = pontosPorPosicao(posicao);

    resultados[totalResultados] = novo;
    totalResultados++;

    /* soma os pontos no piloto, para os outros modulos mostrarem */
    pilotos[indice_piloto].pontos = pilotos[indice_piloto].pontos + novo.pontos;

    printf("\nResultado cadastrado com sucesso!\n");
    printf("Corrida: %s | Piloto: %s | Posicao: %d | Pontos: %d\n",
           corridas[indice_corrida].nome,
           pilotos[indice_piloto].nome,
           novo.posicao,
           novo.pontos);
}

/* ---------- LISTAR RESULTADOS ---------- */

void listarResultados() {
    int indice_corrida;
    int indice_piloto;

    printf("\n--- LISTA DE RESULTADOS ---\n");

    if (totalResultados == 0) {
        printf("Nenhum resultado cadastrado.\n");
        return;
    }

    for (int i = 0; i < totalResultados; i++) {
        indice_corrida = buscarCorrida(resultados[i].id_corrida);
        indice_piloto = buscarPiloto(resultados[i].id_piloto);

        printf("\nCodigo do resultado: %d\n", resultados[i].id_resultado);

        if (indice_corrida != -1) {
            printf("Corrida: %s\n", corridas[indice_corrida].nome);
        } else {
            printf("Corrida: %d\n", resultados[i].id_corrida);
        }

        if (indice_piloto != -1) {
            printf("Piloto: %s\n", pilotos[indice_piloto].nome);
        } else {
            printf("Piloto: %d\n", resultados[i].id_piloto);
        }

        printf("Posicao: %d\n", resultados[i].posicao);
        printf("Pontos: %d\n", resultados[i].pontos);
    }
}

/* ---------- CONSULTAR RESULTADO DE UMA CORRIDA ---------- */

void consultarResultadoCorrida() {
    int id_corrida;
    int indice_corrida;
    int achou;
    Resultado lista[MAX_RESULTADOS];
    int totalLista = 0;
    Resultado aux;
    int indice_piloto;

    printf("\n--- CONSULTAR RESULTADO DE CORRIDA ---\n");

    printf("Codigo da corrida: ");
    scanf("%d", &id_corrida);

    indice_corrida = buscarCorrida(id_corrida);
    if (indice_corrida == -1) {
        printf("Erro: corrida nao encontrada.\n");
        return;
    }

    /* junta os resultados da corrida escolhida */
    for (int i = 0; i < totalResultados; i++) {
        if (resultados[i].id_corrida == id_corrida) {
            lista[totalLista] = resultados[i];
            totalLista++;
        }
    }

    printf("\nCorrida: %s\n", corridas[indice_corrida].nome);
    printf("Pista (codigo): %d\n", corridas[indice_corrida].id_pista);
    printf("Data: %s\n", corridas[indice_corrida].data);

    if (totalLista == 0) {
        printf("Nenhum resultado cadastrado para esta corrida.\n");
        return;
    }

    /* ordena por posicao (menor primeiro) */
    for (int i = 0; i < totalLista - 1; i++) {
        for (int j = 0; j < totalLista - 1 - i; j++) {
            if (lista[j].posicao > lista[j + 1].posicao) {
                aux = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = aux;
            }
        }
    }

    printf("\n--- CLASSIFICACAO DA CORRIDA ---\n");

    achou = 0;
    for (int i = 0; i < totalLista; i++) {
        indice_piloto = buscarPiloto(lista[i].id_piloto);

        if (indice_piloto != -1) {
            printf("%2d. %-20s | Pontos: %d\n",
                   lista[i].posicao,
                   pilotos[indice_piloto].nome,
                   lista[i].pontos);
            achou = 1;
        }
    }

    if (achou == 0) {
        printf("Pilotos dos resultados nao foram encontrados.\n");
    }
}

/* ---------- CLASSIFICACAO DOS PILOTOS ---------- */

void exibirClassificacao() {
    Piloto copia[MAX_PILOTOS];
    int vitorias[MAX_PILOTOS];
    int total = totalPilotos;
    Piloto aux;
    int auxVitorias;
    int achou;

    printf("\n--- CLASSIFICACAO DOS PILOTOS ---\n");

    if (totalPilotos == 0) {
        printf("Nenhum piloto cadastrado.\n");
        return;
    }

    /* a pontuacao e recalculada a partir dos resultados */
    for (int i = 0; i < total; i++) {
        copia[i] = pilotos[i];
        copia[i].pontos = 0;
        vitorias[i] = 0;
    }

    for (int i = 0; i < totalResultados; i++) {
        for (int j = 0; j < total; j++) {
            if (resultados[i].id_piloto == copia[j].id_piloto) {
                copia[j].pontos = copia[j].pontos + resultados[i].pontos;

                if (resultados[i].posicao == 1) {
                    vitorias[j] = vitorias[j] + 1;
                }
            }
        }
    }

    /* ordena por pontos (maior primeiro) e, no empate, por vitorias */
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - 1 - i; j++) {
            if (copia[j].pontos < copia[j + 1].pontos ||
                (copia[j].pontos == copia[j + 1].pontos &&
                 vitorias[j] < vitorias[j + 1])) {

                aux = copia[j];
                copia[j] = copia[j + 1];
                copia[j + 1] = aux;

                auxVitorias = vitorias[j];
                vitorias[j] = vitorias[j + 1];
                vitorias[j + 1] = auxVitorias;
            }
        }
    }

    printf("\nPos | Piloto               | Equipe | Pontos | Vitorias\n");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < total; i++) {
        printf("%3d | %-20s | %6d | %6d | %8d\n",
               i + 1,
               copia[i].nome,
               copia[i].id_equipe,
               copia[i].pontos,
               vitorias[i]);
    }

    /* avisa se ainda nao existe nenhum resultado */
    achou = 0;
    for (int i = 0; i < total; i++) {
        if (copia[i].pontos > 0) {
            achou = 1;
        }
    }

    if (achou == 0) {
        printf("\nNenhum ponto distribuido: ainda nao ha resultados registrados.\n");
    }
}

/* ============================================================
   APOIO PARA TESTE - remover na integracao com o main.c
   (o cadastro de pilotos e de corridas e dos outros modulos)
   ============================================================ */

void cadastrarPilotoTeste() {
    Piloto novo;

    if (totalPilotos >= MAX_PILOTOS) {
        printf("Erro: limite de pilotos atingido.\n");
        return;
    }

    printf("\n--- CADASTRAR PILOTO (teste) ---\n");

    novo.id_piloto = totalPilotos + 1;

    printf("Nome: ");
    scanf(" %99[^\n]", novo.nome);

    printf("Numero: ");
    scanf("%d", &novo.numero);

    printf("Nacionalidade: ");
    scanf(" %49[^\n]", novo.nacionalidade);

    printf("Codigo da equipe: ");
    scanf("%d", &novo.id_equipe);

    novo.pontos = 0;

    pilotos[totalPilotos] = novo;
    totalPilotos++;

    printf("Piloto cadastrado com o codigo %d.\n", novo.id_piloto);
}

void cadastrarCorridaTeste() {
    Corrida nova;

    if (totalCorridas >= MAX_CORRIDAS) {
        printf("Erro: limite de corridas atingido.\n");
        return;
    }

    printf("\n--- CADASTRAR CORRIDA (teste) ---\n");

    nova.id_corrida = totalCorridas + 1;

    printf("Nome da corrida: ");
    scanf(" %99[^\n]", nova.nome);

    printf("Codigo da pista: ");
    scanf("%d", &nova.id_pista);

    printf("Data (DD/MM/AAAA): ");
    scanf(" %10s", nova.data);

    corridas[totalCorridas] = nova;
    totalCorridas++;

    printf("Corrida cadastrada com o codigo %d.\n", nova.id_corrida);
}

/* ---------- MENU (somente para testar o modulo isolado) ---------- */

int main() {
    int opcao;

    do {
        printf("\n===== RESULTADOS E CLASSIFICACAO =====\n");
        printf("1 - Registrar resultado\n");
        printf("2 - Listar resultados\n");
        printf("3 - Consultar resultado de uma corrida\n");
        printf("4 - Classificacao dos pilotos\n");
        printf("5 - Cadastrar piloto (teste)\n");
        printf("6 - Cadastrar corrida (teste)\n");
        printf("0 - Sair\n");
        printf("======================================\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                registrarResultado();
                break;

            case 2:
                listarResultados();
                break;

            case 3:
                consultarResultadoCorrida();
                break;

            case 4:
                exibirClassificacao();
                break;

            case 5:
                cadastrarPilotoTeste();
                break;

            case 6:
                cadastrarCorridaTeste();
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
