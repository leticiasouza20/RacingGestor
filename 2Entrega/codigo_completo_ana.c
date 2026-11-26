#include <stdio.h>
#include <string.h>
#include <ctype.h>

//definir os limites máximos de informações
#define MAX_PILOTOS 50
#define MAX_EQUIPES 20
#define MAX_PISTAS 100
#define MAX_CORRIDAS 100
#define MAX_RESULTADOS 500
#define MAX_POSICOES 20

/* ---------- STRUCTS ---------- */
//definir variaveis em grupos
typedef struct {
    int id_piloto;
    char nome[100];
    int numero;
    char nacionalidade[50];
    int id_equipe;
    int pontos;
} Piloto;

typedef struct {
    int id_equipe;
    char nome[100];
    char pais[50];
    int pontos;
} Equipe;

typedef struct {
    int id_pista;
    char nome[100];
    char pais[50];
    int numero_voltas;
} Pista;

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

/* ---------- VETORES E CONTADORES ---------- */
//criando vetores 

Piloto pilotos[MAX_PILOTOS];
Equipe equipes[MAX_EQUIPES];
Pista pistas[MAX_PISTAS];
Corrida corridas[MAX_CORRIDAS];
Resultado resultados[MAX_RESULTADOS];
//contadores 
int totalPilotos = 0;
int totalEquipes = 0;
int totalPistas = 0;
int totalCorridas = 0;
int totalResultados = 0;

/* ---------- FUNCOES AUXILIARES ---------- */

/* Devolve 1 se o texto for vazio ou so tiver espacos */
int textoVazio(char texto[]) {
    int i;

    for (i = 0; texto[i] != '\0'; i++) {
        if (!isspace((unsigned char)texto[i])) {
            return 0;
        }
    }

    return 1;
}

/* Valida o formato DD/MM/AAAA e se a data existe (inclui ano bissexto) */
int validarData(char data[]) {
    int dia, mes, ano;
    int i;
    int diasMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    /* formato: 10 caracteres, barras nas posicoes 2 e 5, o resto so digitos */
    if (strlen(data) != 10 || data[2] != '/' || data[5] != '/') {
        return 0;
    }
//oq foi digitado sao numeros
    for (i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit((unsigned char)data[i])) {
            return 0;
        }
    }
//add o valor em cada variavel
    sscanf(data, "%2d/%2d/%4d", &dia, &mes, &ano);

    if (ano > 2025 || mes >= 1 || mes <= 12 || dia > 1) {
        return 0;
    }

    /* ano bissexto: fevereiro passa a ter 29 dias */
    if ((ano % 400 == 0) || (ano % 4 == 0 && ano % 100 != 0)) {
        diasMes[1] = 29;
    }

    return dia <= diasMes[mes - 1];
}

/* Pontuacao da Formula 1: so os 10 primeiros pontuam */
int pontosPorPosicao(int posicao) {
    int tabela[] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};

    if (posicao < 1 || posicao > 10) {
        return 0;
    }

    return tabela[posicao - 1];
}

/* ---------- BUSCAS: devolvem a posicao no vetor, ou -1 se nao achar ---------- */

int buscarPiloto(int id) {
    int i;

    for (i = 0; i < totalPilotos; i++) {
        if (pilotos[i].id_piloto == id) return i;
    }

    return -1;
}

int buscarEquipe(int id) {
    int i;

    for (i = 0; i < totalEquipes; i++) {
        if (equipes[i].id_equipe == id) return i;
    }

    return -1;
}

int buscarPista(int id) {
    int i;

    for (i = 0; i < totalPistas; i++) {
        if (pistas[i].id_pista == id) return i;
    }

    return -1;
}

int buscarCorrida(int id) {
    int i;

    for (i = 0; i < totalCorridas; i++) {
        if (corridas[i].id_corrida == id) return i;
    }

    return -1;
}

/* ---------- EQUIPES ---------- */

void cadastrarEquipe() {
    Equipe nova;
    int i;

    printf("\n--- CADASTRAR EQUIPE ---\n");

    if (totalEquipes >= MAX_EQUIPES) {
        printf("Erro: limite de equipes atingido.\n");
        return;
    }

    printf("Codigo da equipe: ");
    scanf("%d", &nova.id_equipe);

    if (nova.id_equipe <= 0) {
        printf("Erro: o codigo deve ser maior que zero.\n");
        return;
    }

    if (buscarEquipe(nova.id_equipe) != -1) {
        printf("Erro: equipe ja cadastrada com esse codigo.\n");
        return;
    }

    printf("Nome da equipe: ");
    scanf(" %99[^\n]", nova.nome);

    /* nome duplicado: strcmp devolve 0 quando os textos sao iguais */
    for (i = 0; i < totalEquipes; i++) {
        if (strcmp(equipes[i].nome, nova.nome) == 0) {
            printf("Erro: equipe ja cadastrada com esse nome.\n");
            return;
        }
    }

    printf("Pais: ");
    scanf(" %49[^\n]", nova.pais);

    nova.pontos = 0;
    equipes[totalEquipes] = nova;
    totalEquipes++;

    printf("Equipe cadastrada com sucesso!\n");
}

void listarEquipes() {
    int i;

    printf("\n--- LISTA DE EQUIPES ---\n");

    if (totalEquipes == 0) {
        printf("Nenhuma equipe cadastrada.\n");
        return;
    }

    for (i = 0; i < totalEquipes; i++) {
        printf("Codigo: %d | Nome: %s | Pais: %s | Pontos: %d\n",
               equipes[i].id_equipe, equipes[i].nome,
               equipes[i].pais, equipes[i].pontos);
    }
}

/* ---------- PILOTOS ---------- */

void cadastrarPiloto() {
    Piloto novo;
    int i;

    printf("\n--- CADASTRAR PILOTO ---\n");

    if (totalEquipes == 0) {
        printf("Erro: cadastre uma equipe primeiro.\n");
        return;
    }

    if (totalPilotos >= MAX_PILOTOS) {
        printf("Erro: limite de pilotos atingido.\n");
        return;
    }

    printf("Codigo do piloto: ");
    scanf("%d", &novo.id_piloto);

    if (novo.id_piloto <= 0) {
        printf("Erro: o codigo deve ser maior que zero.\n");
        return;
    }

    if (buscarPiloto(novo.id_piloto) != -1) {
        printf("Erro: piloto ja cadastrado com esse codigo.\n");
        return;
    }

    printf("Nome: ");
    scanf(" %99[^\n]", novo.nome);

    for (i = 0; i < totalPilotos; i++) {
        if (strcmp(pilotos[i].nome, novo.nome) == 0) {
            printf("Erro: piloto ja cadastrado com esse nome.\n");
            return;
        }
    }

    printf("Numero do carro (1 a 99): ");
    scanf("%d", &novo.numero);

    if (novo.numero < 1 || novo.numero > 99) {
        printf("Erro: o numero deve estar entre 1 e 99.\n");
        return;
    }

    /* o numero do carro nao pode repetir */
    for (i = 0; i < totalPilotos; i++) {
        if (pilotos[i].numero == novo.numero) {
            printf("Erro: esse numero ja pertence a outro piloto.\n");
            return;
        }
    }

    printf("Nacionalidade: ");
    scanf(" %49[^\n]", novo.nacionalidade);

    listarEquipes();

    printf("Codigo da equipe: ");
    scanf("%d", &novo.id_equipe);

    if (buscarEquipe(novo.id_equipe) == -1) {
        printf("Erro: equipe nao cadastrada.\n");
        return;
    }

    novo.pontos = 0;
    pilotos[totalPilotos] = novo;
    totalPilotos++;

    printf("Piloto cadastrado com sucesso!\n");
}

void listarPilotos() {
    int e, i;

    printf("\n--- LISTA DE PILOTOS ---\n");

    if (totalPilotos == 0) {
        printf("Nenhum piloto cadastrado.\n");
        return;
    }

    for (i = 0; i < totalPilotos; i++) {
        e = buscarEquipe(pilotos[i].id_equipe);

        /* troca o codigo pelo nome da equipe */
        if (e != -1) {
            printf("Codigo: %d | Nome: %s | Numero: %d | Nacionalidade: %s | Equipe: %s | Pontos: %d\n",
                   pilotos[i].id_piloto, pilotos[i].nome,
                   pilotos[i].numero, pilotos[i].nacionalidade,
                   equipes[e].nome, pilotos[i].pontos);
        }
    }
}

/* Conta quantas corridas o piloto venceu (posicao 1) */
int contarVitorias(int id_piloto) {
    int total = 0;
    int i;

    for (i = 0; i < totalResultados; i++) {
        if (resultados[i].id_piloto == id_piloto &&
            resultados[i].posicao == 1) {
            total++;
        }
    }

    return total;
}

void exibirClassificacao() {
    Piloto copia[MAX_PILOTOS], aux;
    int e, i, j;

    printf("\n--- CLASSIFICACAO DO CAMPEONATO ---\n");

    if (totalPilotos == 0) {
        printf("Nenhum piloto cadastrado.\n");
        return;
    }

    /* ordena uma COPIA, para nao mexer na ordem original dos pilotos */
    for (i = 0; i < totalPilotos; i++) {
        copia[i] = pilotos[i];
    }

    /* bubble sort: mais pontos primeiro; no empate, mais vitorias */
    for (i = 0; i < totalPilotos - 1; i++) {
        for (j = 0; j < totalPilotos - 1 - i; j++) {
            if (copia[j].pontos < copia[j + 1].pontos ||
                (copia[j].pontos == copia[j + 1].pontos &&
                 contarVitorias(copia[j].id_piloto) <
                 contarVitorias(copia[j + 1].id_piloto))) {

                aux = copia[j];
                copia[j] = copia[j + 1];
                copia[j + 1] = aux;
            }
        }
    }

    printf("\nPos | Piloto               | Equipe               | Pontos | Vitorias\n");
    printf("---------------------------------------------------------------------\n");

    for (i = 0; i < totalPilotos; i++) {
        e = buscarEquipe(copia[i].id_equipe);

        if (e != -1) {
            printf("%3d | %-20s | %-20s | %6d | %8d\n",
                   i + 1, copia[i].nome, equipes[e].nome,
                   copia[i].pontos, contarVitorias(copia[i].id_piloto));
        }
    }
}

/* ---------- PISTAS ---------- */

void cadastrarPista() {
    Pista nova;
    int i;

    printf("\n--- CADASTRAR PISTA ---\n");

    if (totalPistas >= MAX_PISTAS) {
        printf("Erro: limite de pistas atingido.\n");
        return;
    }

    nova.id_pista = totalPistas + 1; /* id automatico */

    printf("Nome da pista: ");
    scanf(" %99[^\n]", nova.nome);

    printf("Pais: ");
    scanf(" %49[^\n]", nova.pais);

    printf("Numero de voltas: ");
    scanf("%d", &nova.numero_voltas);

    if (textoVazio(nova.nome) || textoVazio(nova.pais) ||
        nova.numero_voltas <= 0) {
        printf("Erro: dados invalidos! Pista nao cadastrada.\n");
        return;
    }

    /* pista duplicada = mesmo nome */
    for (i = 0; i < totalPistas; i++) {
        if (strcmp(pistas[i].nome, nova.nome) == 0) {
            printf("Erro: pista ja cadastrada.\n");
            return;
        }
    }

    pistas[totalPistas] = nova;
    totalPistas++;

    printf("Pista cadastrada com sucesso! ID da pista: %d\n",
           nova.id_pista);
}

void listarPistas() {
    int i;

    printf("\n--- LISTA DE PISTAS ---\n");

    if (totalPistas == 0) {
        printf("Nenhuma pista cadastrada.\n");
        return;
    }

    for (i = 0; i < totalPistas; i++) {
        printf("ID: %d | Nome: %s | Pais: %s | Voltas: %d\n",
               pistas[i].id_pista, pistas[i].nome,
               pistas[i].pais, pistas[i].numero_voltas);
    }
}

/* ---------- CORRIDAS ---------- */

/* Devolve 1 se a corrida for valida; senao mostra o erro e devolve 0 */
int validarCorrida(Corrida nova) {
    int i;

    if (textoVazio(nova.nome)) {
        printf("Erro: nome da corrida vazio.\n");
        return 0;
    }

    if (buscarPista(nova.id_pista) == -1) {
        printf("Erro: pista nao cadastrada.\n");
        return 0;
    }

    if (!validarData(nova.data)) {
        printf("Erro: data invalida. Use DD/MM/AAAA.\n");
        return 0;
    }

    /* corrida duplicada = mesmo nome, mesma pista e mesma data */
    for (i = 0; i < totalCorridas; i++) {
        if (strcmp(corridas[i].nome, nova.nome) == 0 &&
            corridas[i].id_pista == nova.id_pista &&
            strcmp(corridas[i].data, nova.data) == 0) {
            printf("Erro: corrida ja cadastrada.\n");
            return 0;
        }
    }

    return 1;
}

void cadastrarCorrida() {
    Corrida nova;

    printf("\n--- CADASTRAR CORRIDA ---\n");

    if (totalPistas == 0) {
        printf("Erro: cadastre uma pista primeiro.\n");
        return;
    }

    if (totalCorridas >= MAX_CORRIDAS) {
        printf("Erro: limite de corridas atingido.\n");
        return;
    }

    nova.id_corrida = totalCorridas + 1; /* id automatico */

    printf("Nome da corrida: ");
    scanf(" %99[^\n]", nova.nome);

    listarPistas();

    printf("ID da pista: ");
    scanf("%d", &nova.id_pista);

    printf("Data da corrida (DD/MM/AAAA): ");
    scanf(" %10s", nova.data);

    if (validarCorrida(nova)) {
        corridas[totalCorridas] = nova;
        totalCorridas++;

        printf("Corrida cadastrada com sucesso! ID da corrida: %d\n",
               nova.id_corrida);
    } else {
        printf("Cadastro cancelado.\n");
    }
}

void listarCorridas() {
    int p, k, i, r;

    printf("\n--- LISTA DE CORRIDAS ---\n");

    if (totalCorridas == 0) {
        printf("Nenhuma corrida cadastrada.\n");
        return;
    }

    for (i = 0; i < totalCorridas; i++) {
        p = buscarPista(corridas[i].id_pista);

        printf("\nID: %d | Corrida: %s | Pista: %s | Data: %s\n",
               corridas[i].id_corrida, corridas[i].nome,
               p != -1 ? pistas[p].nome : "Nao encontrada",
               corridas[i].data);

        /* mostra piloto e posicao de quem ja tem resultado nessa corrida */
        for (r = 0; r < totalResultados; r++) {
            if (resultados[r].id_corrida == corridas[i].id_corrida) {
                k = buscarPiloto(resultados[r].id_piloto);

                if (k != -1) {
                    printf("   Posicao %d: %s\n",
                           resultados[r].posicao, pilotos[k].nome);
                }
            }
        }
    }
}

/* ---------- RESULTADOS ---------- */

void registrarResultado() {
    Resultado novo;
    int ic, ip, ie, i;

    printf("\n--- REGISTRAR RESULTADO ---\n");

    if (totalResultados >= MAX_RESULTADOS) {
        printf("Erro: limite de resultados atingido.\n");
        return;
    }

    printf("Codigo da corrida: ");
    scanf("%d", &novo.id_corrida);

    ic = buscarCorrida(novo.id_corrida);

    if (ic == -1) {
        printf("Erro: corrida nao encontrada.\n");
        return;
    }

    printf("Codigo do piloto: ");
    scanf("%d", &novo.id_piloto);

    ip = buscarPiloto(novo.id_piloto);

    if (ip == -1) {
        printf("Erro: piloto nao encontrado.\n");
        return;
    }

    printf("Posicao de chegada (1 a %d): ", MAX_POSICOES);
    scanf("%d", &novo.posicao);

    if (novo.posicao < 1 || novo.posicao > MAX_POSICOES) {
        printf("Erro: a posicao deve estar entre 1 e %d.\n",
               MAX_POSICOES);
        return;
    }

    /* na mesma corrida: o piloto nao pode repetir e a posicao nao pode estar ocupada */
    for (i = 0; i < totalResultados; i++) {
        if (resultados[i].id_corrida == novo.id_corrida) {
            if (resultados[i].id_piloto == novo.id_piloto) {
                printf("Erro: esse piloto ja tem resultado nesta corrida.\n");
                return;
            }

            if (resultados[i].posicao == novo.posicao) {
                printf("Erro: essa posicao ja foi registrada nesta corrida.\n");
                return;
            }
        }
    }

    novo.id_resultado = totalResultados + 1;
    novo.pontos = pontosPorPosicao(novo.posicao);

    resultados[totalResultados] = novo;
    totalResultados++;

    /* soma os pontos no piloto e na equipe dele */
    pilotos[ip].pontos += novo.pontos;

    ie = buscarEquipe(pilotos[ip].id_equipe);

    if (ie != -1) {
        equipes[ie].pontos += novo.pontos;
    }

    printf("Resultado registrado com sucesso!\n");

    printf("Corrida: %s | Piloto: %s | Posicao: %d | Pontos: %d\n",
           corridas[ic].nome, pilotos[ip].nome,
           novo.posicao, novo.pontos);
}

void consultarResultadoCorrida() {
    int id, ic, ip, encontrou = 0;
    int pos, i, p;

    printf("\n--- CONSULTAR RESULTADO DE CORRIDA ---\n");

    printf("Codigo da corrida: ");
    scanf("%d", &id);

    ic = buscarCorrida(id);

    if (ic == -1) {
        printf("Erro: corrida nao encontrada.\n");
        return;
    }

    p = buscarPista(corridas[ic].id_pista);

    printf("\nCorrida: %s\n", corridas[ic].nome);

    if (p != -1) {
        printf("Pista: %s\n", pistas[p].nome);
    }

    printf("Data: %s\n", corridas[ic].data);

    printf("\n--- RESULTADOS ---\n");

    /* percorre as posicoes em ordem (1o, 2o...) para mostrar o resultado organizado */
    for (pos = 1; pos <= MAX_POSICOES; pos++) {
        for (i = 0; i < totalResultados; i++) {
            if (resultados[i].id_corrida == id &&
                resultados[i].posicao == pos) {

                ip = buscarPiloto(resultados[i].id_piloto);

                if (ip != -1) {
                    printf("%2d. %-20s | Pontos: %d\n",
                           pos, pilotos[ip].nome,
                           resultados[i].pontos);
                    encontrou = 1;
                }
            }
        }
    }

    if (!encontrou) {
        printf("Nenhum resultado cadastrado para esta corrida.\n");
    }
}

/* ---------- PESQUISAR PILOTO ---------- */

void pesquisarPiloto() {
    int escolha, codigo, i, ic;
    int encontrou = 0;
    int j, r, e;
    char nome[100];

    i = -1;

    printf("\n--- PESQUISAR PILOTO ---\n");
    printf("1 - Pesquisar por codigo\n");
    printf("2 - Pesquisar por nome\n");
    printf("Escolha: ");

    scanf("%d", &escolha);

    if (escolha == 1) {
        printf("Digite o codigo: ");
        scanf("%d", &codigo);
        i = buscarPiloto(codigo);

    } else if (escolha == 2) {
        printf("Digite o nome: ");
        scanf(" %99[^\n]", nome);

        for (j = 0; j < totalPilotos; j++) {
            if (strcmp(pilotos[j].nome, nome) == 0) {
                i = j;
                break;
            }
        }

    } else {
        printf("Opcao invalida!\n");
        return;
    }

    if (i == -1) {
        printf("Piloto nao encontrado.\n");
        return;
    }

    e = buscarEquipe(pilotos[i].id_equipe);

    printf("\nPiloto encontrado:\n");

    printf("Codigo: %d\nNome: %s\nNumero: %d\nNacionalidade: %s\n",
           pilotos[i].id_piloto, pilotos[i].nome,
           pilotos[i].numero, pilotos[i].nacionalidade);

    if (e != -1) {
        printf("Equipe: %s\n", equipes[e].nome);
    }

    printf("Pontos: %d\n", pilotos[i].pontos);

    /* historico: todos os resultados desse piloto */
    printf("\n--- HISTORICO ---\n");

    for (r = 0; r < totalResultados; r++) {
        if (resultados[r].id_piloto == pilotos[i].id_piloto) {
            ic = buscarCorrida(resultados[r].id_corrida);

            if (ic != -1) {
                printf("Corrida: %s | Posicao: %d | Pontos: %d\n",
                       corridas[ic].nome,
                       resultados[r].posicao,
                       resultados[r].pontos);
                encontrou = 1;
            }
        }
    }

    if (!encontrou) {
        printf("Nenhum resultado encontrado.\n");
    }
}

/* ---------- MENU PRINCIPAL ---------- */

int main() {
    int opcao;

    do {
        printf("\n======= RACINGGESTOR =======\n");
        printf("1 - Cadastrar Piloto\n");
        printf("2 - Cadastrar Equipe\n");
        printf("3 - Cadastrar Pista\n");
        printf("4 - Listar Pilotos e Classificacao\n");
        printf("5 - Listar Equipes\n");
        printf("6 - Listar Pistas\n");
        printf("7 - Cadastrar Corrida\n");
        printf("8 - Registrar Resultado\n");
        printf("9 - Listar Corridas\n");
        printf("10 - Consultar Resultado de Corrida\n");
        printf("11 - Pesquisar Piloto\n");
        printf("12 - Sair\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarPiloto();
                break;

            case 2:
                cadastrarEquipe();
                break;

            case 3:
                cadastrarPista();
                break;

            case 4:
                listarPilotos();
                exibirClassificacao();
                break;

            case 5:
                listarEquipes();
                break;

            case 6:
                listarPistas();
                break;

            case 7:
                cadastrarCorrida();
                break;

            case 8:
                registrarResultado();
                break;

            case 9:
                listarCorridas();
                break;

            case 10:
                consultarResultadoCorrida();
                break;

            case 11:
                pesquisarPiloto();
                break;

            case 12:
                printf("Encerrando RacingGestor!\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 12);

    return 0;
}
