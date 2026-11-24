#include <stdio.h>
#include <string.h>

#define MAX_PILOTOS 50
#define MAX_EQUIPES 20
#define MAX_PISTAS 100
#define MAX_CORRIDAS 100
#define MAX_RESULTADOS 500

/* ESTRUTURAS */

typedef struct {
    int id, numero, equipe, pontos, vitorias; /* vitorias evita recontar na classificacao */
    char nome[50], nacionalidade[30];
} Piloto;

typedef struct {
    int id, pontos;
    char nome[50], pais[30];
} Equipe;

typedef struct {
    int id, voltas;
    char nome[50], pais[30];
} Pista;

typedef struct {
    int id, pista;
    char nome[50], data[11];
} Corrida;

typedef struct {
    int corrida, piloto, posicao; /* os pontos sao calculados pela posicao */
} Resultado;

/* VETORES E CONTADORES */

Piloto pilotos[MAX_PILOTOS];
Equipe equipes[MAX_EQUIPES];
Pista pistas[MAX_PISTAS];
Corrida corridas[MAX_CORRIDAS];
Resultado resultados[MAX_RESULTADOS];

int totalPilotos = 0, totalEquipes = 0, totalPistas = 0;
int totalCorridas = 0, totalResultados = 0;

/* BUSCAS: devolvem a posicao no vetor, ou -1 se nao achar.
   Pistas e corridas tem id automatico (1, 2, 3...), entao
   o id N fica sempre na posicao N-1 e nao precisa de busca. */

int buscarPiloto(int id) {
    int i;
    for (i = 0; i < totalPilotos; i++) {
        if (pilotos[i].id == id) return i;
    }
    return -1;
}

int buscarEquipe(int id) {
    int i;
    for (i = 0; i < totalEquipes; i++) {
        if (equipes[i].id == id) return i;
    }
    return -1;
}

/* EQUIPES */

void cadastrarEquipe() {
    Equipe e; /* variavel temporaria: so entra no vetor se tudo estiver valido */

    if (totalEquipes >= MAX_EQUIPES) {
        printf("Limite de equipes atingido!\n");
        return;
    }

    printf("Codigo da equipe: ");
    scanf("%d", &e.id);

    if (e.id <= 0 || buscarEquipe(e.id) != -1) {
        printf("Codigo invalido ou ja cadastrado!\n");
        return;
    }

    printf("Nome da equipe: ");
    scanf("%49s", e.nome);
    printf("Pais: ");
    scanf("%29s", e.pais);
    e.pontos = 0;

    equipes[totalEquipes++] = e; /* copia a equipe e aumenta o contador */
    printf("Equipe cadastrada!\n");
}

void listarEquipes() {
    int i;

    if (totalEquipes == 0) {
        printf("Nenhuma equipe cadastrada!\n");
        return;
    }

    for (i = 0; i < totalEquipes; i++) {
        printf("\nCodigo: %d\nNome: %s\nPais: %s\nPontos: %d\n",
               equipes[i].id, equipes[i].nome, equipes[i].pais, equipes[i].pontos);
    }
}

/* PILOTOS */

void cadastrarPiloto() {
    Piloto p;
    int i;

    if (totalEquipes == 0 || totalPilotos >= MAX_PILOTOS) {
        printf("Cadastre uma equipe ou verifique o limite!\n");
        return;
    }

    printf("Codigo do piloto: ");
    scanf("%d", &p.id);

    if (p.id <= 0 || buscarPiloto(p.id) != -1) {
        printf("Codigo invalido ou ja cadastrado!\n");
        return;
    }

    printf("Nome: ");
    scanf("%49s", p.nome);

    printf("Numero do carro (1 a 99): ");
    scanf("%d", &p.numero);

    if (p.numero < 1 || p.numero > 99) {
        printf("Numero invalido!\n");
        return;
    }

    /* o numero do carro nao pode repetir */
    for (i = 0; i < totalPilotos; i++) {
        if (pilotos[i].numero == p.numero) {
            printf("Numero ja utilizado!\n");
            return;
        }
    }

    printf("Nacionalidade: ");
    scanf("%29s", p.nacionalidade);

    listarEquipes();
    printf("Codigo da equipe: ");
    scanf("%d", &p.equipe);

    if (buscarEquipe(p.equipe) == -1) {
        printf("Equipe nao encontrada!\n");
        return;
    }

    p.pontos = 0;
    p.vitorias = 0;

    pilotos[totalPilotos++] = p;
    printf("Piloto cadastrado!\n");
}

void listarPilotos() {
    int i;

    if (totalPilotos == 0) {
        printf("Nenhum piloto cadastrado!\n");
        return;
    }

    for (i = 0; i < totalPilotos; i++) {
        printf("\nCodigo: %d\nNome: %s\nNumero: %d\nNacionalidade: %s\nEquipe: %s\nPontos: %d\n",
               pilotos[i].id, pilotos[i].nome, pilotos[i].numero,
               pilotos[i].nacionalidade,
               equipes[buscarEquipe(pilotos[i].equipe)].nome,
               pilotos[i].pontos);
    }
}

void exibirClassificacao() {
    Piloto copia[MAX_PILOTOS], aux;
    int i, j;

    /* ordena uma COPIA, para nao bagunçar a ordem original */
    for (i = 0; i < totalPilotos; i++) {
        copia[i] = pilotos[i];
    }

    /* bubble sort: mais pontos primeiro; empate decide por vitorias */
    for (i = 0; i < totalPilotos - 1; i++) {
        for (j = 0; j < totalPilotos - 1 - i; j++) {
            if (copia[j].pontos < copia[j + 1].pontos ||
               (copia[j].pontos == copia[j + 1].pontos &&
                copia[j].vitorias < copia[j + 1].vitorias)) {
                aux = copia[j];
                copia[j] = copia[j + 1];
                copia[j + 1] = aux;
            }
        }
    }

    printf("\n--- CLASSIFICACAO DO CAMPEONATO ---\n");
    for (i = 0; i < totalPilotos; i++) {
        printf("%d - %s | Pontos: %d | Vitorias: %d\n",
               i + 1, copia[i].nome, copia[i].pontos, copia[i].vitorias);
    }
}

/* PISTAS */

void cadastrarPista() {
    Pista p;

    if (totalPistas >= MAX_PISTAS) {
        printf("Limite de pistas atingido!\n");
        return;
    }

    p.id = totalPistas + 1; /* id automatico */

    printf("Nome da pista: ");
    scanf("%49s", p.nome);
    printf("Pais: ");
    scanf("%29s", p.pais);
    printf("Numero de voltas: ");
    scanf("%d", &p.voltas);

    if (p.voltas <= 0) {
        printf("Numero de voltas invalido!\n");
        return;
    }

    pistas[totalPistas++] = p;
    printf("Pista cadastrada!\n");
}

void listarPistas() {
    int i;
    for (i = 0; i < totalPistas; i++) {
        printf("\nID: %d\nPista: %s\nPais: %s\nVoltas: %d\n",
               pistas[i].id, pistas[i].nome, pistas[i].pais, pistas[i].voltas);
    }
}

/* CORRIDAS */

void cadastrarCorrida() {
    Corrida c;

    if (totalPistas == 0 || totalCorridas >= MAX_CORRIDAS) {
        printf("Cadastre uma pista ou verifique o limite!\n");
        return;
    }

    c.id = totalCorridas + 1; /* id automatico */

    printf("Nome da corrida: ");
    scanf("%49s", c.nome);

    listarPistas();
    printf("ID da pista: ");
    scanf("%d", &c.pista);

    if (c.pista < 1 || c.pista > totalPistas) {
        printf("Pista nao encontrada!\n");
        return;
    }

    printf("Data (DD/MM/AAAA): ");
    scanf("%10s", c.data);

    corridas[totalCorridas++] = c;
    printf("Corrida cadastrada!\n");
}

void listarCorridas() {
    int i;
    for (i = 0; i < totalCorridas; i++) {
        printf("\nID: %d\nCorrida: %s\nData: %s\nPista: %s\n",
               corridas[i].id, corridas[i].nome, corridas[i].data,
               pistas[corridas[i].pista - 1].nome); /* id - 1 = posicao no vetor */
    }
}

/* RESULTADOS */

int pontosPorPosicao(int posicao) {
    int tabela[10] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};

    if (posicao >= 1 && posicao <= 10) return tabela[posicao - 1];
    return 0; /* da 11o lugar em diante nao pontua */
}

void registrarResultado() {
    Resultado r;
    int i, p, pts;

    if (totalResultados >= MAX_RESULTADOS) {
        printf("Limite de resultados atingido!\n");
        return;
    }

    printf("Codigo da corrida: ");
    scanf("%d", &r.corrida);

    if (r.corrida < 1 || r.corrida > totalCorridas) {
        printf("Corrida nao encontrada!\n");
        return;
    }

    printf("Codigo do piloto: ");
    scanf("%d", &r.piloto);

    p = buscarPiloto(r.piloto);
    if (p == -1) {
        printf("Piloto nao encontrado!\n");
        return;
    }

    printf("Posicao de chegada (1 a 20): ");
    scanf("%d", &r.posicao);

    if (r.posicao < 1 || r.posicao > 20) {
        printf("Posicao invalida!\n");
        return;
    }

    /* na mesma corrida: o piloto nao pode repetir e a posicao nao pode estar ocupada */
    for (i = 0; i < totalResultados; i++) {
        if (resultados[i].corrida == r.corrida &&
           (resultados[i].piloto == r.piloto || resultados[i].posicao == r.posicao)) {
            printf("Piloto ou posicao ja cadastrados nessa corrida!\n");
            return;
        }
    }

    /* soma os pontos no piloto e na equipe dele */
    pts = pontosPorPosicao(r.posicao);
    pilotos[p].pontos += pts;
    equipes[buscarEquipe(pilotos[p].equipe)].pontos += pts;

    if (r.posicao == 1) pilotos[p].vitorias++;

    resultados[totalResultados++] = r;
    printf("Resultado registrado!\n");
}

void consultarResultadoCorrida() {
    int id, pos, i, encontrou = 0;

    printf("Codigo da corrida: ");
    scanf("%d", &id);

    if (id < 1 || id > totalCorridas) {
        printf("Corrida nao encontrada!\n");
        return;
    }

    printf("\nCorrida: %s\nData: %s\n", corridas[id - 1].nome, corridas[id - 1].data);
    printf("\n--- RESULTADOS ---\n");

    /* percorre as posicoes em ordem (1o, 2o...) para exibir o podio organizado */
    for (pos = 1; pos <= 20; pos++) {
        for (i = 0; i < totalResultados; i++) {
            if (resultados[i].corrida == id && resultados[i].posicao == pos) {
                printf("%d - %s | Pontos: %d\n", pos,
                       pilotos[buscarPiloto(resultados[i].piloto)].nome,
                       pontosPorPosicao(pos));
                encontrou = 1;
            }
        }
    }

    if (!encontrou) printf("Nenhum resultado cadastrado!\n");
}

void pesquisarPiloto() {
    int opcao, id, i = -1, r, encontrou = 0;
    char nome[50];

    printf("1 - Pesquisar por codigo\n2 - Pesquisar por nome\nEscolha: ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        printf("Codigo: ");
        scanf("%d", &id);
        i = buscarPiloto(id);
    } else if (opcao == 2) {
        printf("Nome: ");
        scanf("%49s", nome);

        for (r = 0; r < totalPilotos; r++) {
            if (strcmp(pilotos[r].nome, nome) == 0) { /* strcmp devolve 0 se os textos forem iguais */
                i = r;
                break;
            }
        }
    } else {
        printf("Opcao invalida!\n");
        return;
    }

    if (i == -1) {
        printf("Piloto nao encontrado!\n");
        return;
    }

    printf("\nNome: %s\nNumero: %d\nNacionalidade: %s\nEquipe: %s\nPontos: %d\n",
           pilotos[i].nome, pilotos[i].numero, pilotos[i].nacionalidade,
           equipes[buscarEquipe(pilotos[i].equipe)].nome, pilotos[i].pontos);

    printf("\n--- HISTORICO ---\n");

    for (r = 0; r < totalResultados; r++) {
        if (resultados[r].piloto == pilotos[i].id) {
            printf("Corrida: %s | Posicao: %d | Pontos: %d\n",
                   corridas[resultados[r].corrida - 1].nome,
                   resultados[r].posicao,
                   pontosPorPosicao(resultados[r].posicao));
            encontrou = 1;
        }
    }

    if (!encontrou) printf("Nenhum resultado encontrado!\n");
}

/* MENU PRINCIPAL */

int main() {
    int opcao;

    do {
        printf("\n======= RACINGGESTOR =======\n"
               "1 - Cadastrar Piloto\n"
               "2 - Cadastrar Equipe\n"
               "3 - Cadastrar Pista\n"
               "4 - Listar Pilotos e Classificacao\n"
               "5 - Listar Equipes\n"
               "6 - Listar Pistas\n"
               "7 - Cadastrar Corrida\n"
               "8 - Registrar Resultado\n"
               "9 - Listar Corridas\n"
               "10 - Consultar Resultado\n"
               "11 - Pesquisar Piloto\n"
               "12 - Sair\n"
               "Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:  cadastrarPiloto(); break;
            case 2:  cadastrarEquipe(); break;
            case 3:  cadastrarPista(); break;
            case 4:  listarPilotos(); exibirClassificacao(); break;
            case 5:  listarEquipes(); break;
            case 6:  listarPistas(); break;
            case 7:  cadastrarCorrida(); break;
            case 8:  registrarResultado(); break;
            case 9:  listarCorridas(); break;
            case 10: consultarResultadoCorrida(); break;
            case 11: pesquisarPiloto(); break;
            case 12: printf("Encerrando RacingGestor!\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 12);

    return 0;
}
