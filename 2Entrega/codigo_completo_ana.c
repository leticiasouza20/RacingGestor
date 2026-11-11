#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* RACINGGESTOR - Sistema de Gerenciamento de Campeonato de F1
   Junção dos tres codigos da equipe em um unico programa.
   Versao compativel com compiladores antigos (ANSI C / C90). */

#define MAX_PILOTOS 50
#define MAX_EQUIPES 20
#define MAX_PISTAS 100
#define MAX_CORRIDAS 100
#define MAX_RESULTADOS 500
#define MAX_POSICOES 20

/*  ESTRUTURAS DE DADOS  */

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

/*  VETORES COMPARTILHADOS  */

Piloto pilotos[MAX_PILOTOS];
Equipe equipes[MAX_EQUIPES];
Pista pistas[MAX_PISTAS];
Corrida corridas[MAX_CORRIDAS];
Resultado resultados[MAX_RESULTADOS];

int totalPilotos = 0;
int totalEquipes = 0;
int totalPistas = 0;
int totalCorridas = 0;
int totalResultados = 0;

/* LEITURA E VALIDACAO BASICAS */

int textoVazio(char texto[]) {
    int i;
    for (i = 0; texto[i] != '\0'; i++) {
        if (!isspace((unsigned char) texto[i])) {
            return 0;
        }
    }
    return 1;
}

/* Evita que letras no lugar de numeros travem o menu. */
int lerInteiro(char mensagem[]) {
    char linha[100];
    char sobra;
    int valor;

    printf("%s", mensagem);
    if (fgets(linha, sizeof(linha), stdin) == NULL) {
        return -1;
    }

    if (sscanf(linha, " %d %c", &valor, &sobra) != 1) {
        printf("Erro: digite um numero inteiro valido.\n");
        return -1;
    }
    return valor;
}

/* Le uma linha, inclusive nomes com espacos, dentro do limite da struct. */
int lerTexto(char mensagem[], char destino[], int tamanho) {
    char linha[256];
    int c;

    printf("%s", mensagem);
    if (fgets(linha, sizeof(linha), stdin) == NULL) {
        printf("Erro: nao foi possivel ler o texto.\n");
        return 0;
    }

    if (strchr(linha, '\n') == NULL && !feof(stdin)) {
        while ((c = getchar()) != '\n' && c != EOF) {
            /* descarta o restante de uma entrada muito longa */
        }
    }
    linha[strcspn(linha, "\n")] = '\0';

    if (textoVazio(linha) || strlen(linha) >= (unsigned int) tamanho) {
        printf("Erro: texto vazio ou muito longo.\n");
        return 0;
    }

    strcpy(destino, linha);
    return 1;
}

/* BUSCAS: RETORNAM O INDICE OU -1  */

int buscarPiloto(int id) {
    int i;
    for (i = 0; i < totalPilotos; i++) {
        if (pilotos[i].id_piloto == id) {
            return i;
        }
    }
    return -1;
}

int buscarEquipe(int id) {
    int i;
    for (i = 0; i < totalEquipes; i++) {
        if (equipes[i].id_equipe == id) {
            return i;
        }
    }
    return -1;
}

int buscarPista(int id) {
    int i;
    for (i = 0; i < totalPistas; i++) {
        if (pistas[i].id_pista == id) {
            return i;
        }
    }
    return -1;
}

int buscarCorrida(int id) {
    int i;
    for (i = 0; i < totalCorridas; i++) {
        if (corridas[i].id_corrida == id) {
            return i;
        }
    }
    return -1;
}

/* EQUIPES: BASE DO CODIGO_LE.C */

void cadastrarEquipe() {
    int i;
    Equipe nova;

    printf("\n--- CADASTRAR EQUIPE ---\n");
    if (totalEquipes >= MAX_EQUIPES) {
        printf("Erro: limite de equipes atingido.\n");
        return;
    }

    nova.id_equipe = lerInteiro("Codigo da equipe: ");
    if (nova.id_equipe <= 0) {
        printf("Erro: o codigo deve ser maior que zero.\n");
        return;
    }
    if (buscarEquipe(nova.id_equipe) != -1) {
        printf("Erro: equipe ja cadastrada com esse codigo.\n");
        return;
    }

    if (!lerTexto("Nome da equipe: ", nova.nome, 100)) {
        return;
    }
    for (i = 0; i < totalEquipes; i++) {
        if (strcmp(equipes[i].nome, nova.nome) == 0) {
            printf("Erro: equipe ja cadastrada com esse nome.\n");
            return;
        }
    }

    if (!lerTexto("Pais: ", nova.pais, 50)) {
        return;
    }

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

/* PILOTOS: BASE DO CODIGO_LE.C */

void cadastrarPiloto() {
    int i;
    Piloto novo;

    printf("\n--- CADASTRAR PILOTO ---\n");
    if (totalEquipes == 0) {
        printf("Erro: cadastre uma equipe primeiro.\n");
        return;
    }
    if (totalPilotos >= MAX_PILOTOS) {
        printf("Erro: limite de pilotos atingido.\n");
        return;
    }

    novo.id_piloto = lerInteiro("Codigo do piloto: ");
    if (novo.id_piloto <= 0) {
        printf("Erro: o codigo deve ser maior que zero.\n");
        return;
    }
    if (buscarPiloto(novo.id_piloto) != -1) {
        printf("Erro: piloto ja cadastrado com esse codigo.\n");
        return;
    }

    if (!lerTexto("Nome: ", novo.nome, 100)) {
        return;
    }
    for (i = 0; i < totalPilotos; i++) {
        if (strcmp(pilotos[i].nome, novo.nome) == 0) {
            printf("Erro: piloto ja cadastrado com esse nome.\n");
            return;
        }
    }

    novo.numero = lerInteiro("Numero do carro (1 a 99): ");
    if (novo.numero < 1 || novo.numero > 99) {
        printf("Erro: o numero deve estar entre 1 e 99.\n");
        return;
    }
    for (i = 0; i < totalPilotos; i++) {
        if (pilotos[i].numero == novo.numero) {
            printf("Erro: esse numero ja pertence a outro piloto.\n");
            return;
        }
    }

    if (!lerTexto("Nacionalidade: ", novo.nacionalidade, 50)) {
        return;
    }

    printf("\nEquipes cadastradas:\n");
    listarEquipes();
    novo.id_equipe = lerInteiro("Codigo da equipe do piloto: ");
    if (buscarEquipe(novo.id_equipe) == -1) {
        printf("Erro: equipe nao cadastrada.\n");
        return;
    }

    novo.pontos = 0;
    pilotos[totalPilotos] = novo;
    totalPilotos++;
    printf("Piloto cadastrado com sucesso!\n");
}

void exibirClassificacao() {
    int i, j;
    Piloto copia[MAX_PILOTOS];
    int vitorias[MAX_PILOTOS];
    Piloto auxiliar;
    int auxVitorias;

    if (totalPilotos == 0) {
        return;
    }

    /* Copia os pilotos para nao alterar a ordem original dos cadastros. */
    for (i = 0; i < totalPilotos; i++) {
        copia[i] = pilotos[i];
        vitorias[i] = 0;
    }

    /* Conta as vitorias para desempatar a classificacao. */
    for (i = 0; i < totalResultados; i++) {
        for (j = 0; j < totalPilotos; j++) {
            if (resultados[i].id_piloto == copia[j].id_piloto &&
                resultados[i].posicao == 1) {
                vitorias[j]++;
            }
        }
    }

    /* Ordena por pontos e, em caso de empate, por numero de vitorias. */
    for (i = 0; i < totalPilotos - 1; i++) {
        for (j = 0; j < totalPilotos - 1 - i; j++) {
            if (copia[j].pontos < copia[j + 1].pontos ||
                (copia[j].pontos == copia[j + 1].pontos &&
                 vitorias[j] < vitorias[j + 1])) {
                auxiliar = copia[j];
                copia[j] = copia[j + 1];
                copia[j + 1] = auxiliar;

                auxVitorias = vitorias[j];
                vitorias[j] = vitorias[j + 1];
                vitorias[j + 1] = auxVitorias;
            }
        }
    }

    printf("\n--- CLASSIFICACAO DO CAMPEONATO ---\n");
    printf("Pos | Piloto                    | Pontos | Vitorias\n");
    for (i = 0; i < totalPilotos; i++) {
        printf("%3d | %-25s | %6d | %8d\n", i + 1,
               copia[i].nome, copia[i].pontos, vitorias[i]);
    }
}

void listarPilotos() {
    int i;
    printf("\n--- LISTA DE PILOTOS ---\n");
    if (totalPilotos == 0) {
        printf("Nenhum piloto cadastrado.\n");
        return;
    }

    for (i = 0; i < totalPilotos; i++) {
        int e = buscarEquipe(pilotos[i].id_equipe);
        printf("Codigo: %d | Nome: %s | Numero: %d | Nacionalidade: %s",
               pilotos[i].id_piloto, pilotos[i].nome,
               pilotos[i].numero, pilotos[i].nacionalidade);
        if (e != -1) {
            printf(" | Equipe: %s", equipes[e].nome);
        }
        printf(" | Pontos: %d\n", pilotos[i].pontos);
    }

 
    exibirClassificacao();
}

/* PISTAS E CORRIDAS: BASE DO CODIGO_JV.C */

int validarData(char data[]) {
    int i;
    int dia, mes, ano;
    int diasMes[] = {31, 28, 31, 30, 31, 30,
                     31, 31, 30, 31, 30, 31};

    if (strlen(data) != 10 || data[2] != '/' || data[5] != '/') {
        return 0;
    }
    for (i = 0; i < 10; i++) {
        if (i != 2 && i != 5 && !isdigit((unsigned char) data[i])) {
            return 0;
        }
    }

    sscanf(data, "%2d/%2d/%4d", &dia, &mes, &ano);
    if (ano < 1900 || mes < 1 || mes > 12 || dia < 1) {
        return 0;
    }
    if (ano % 400 == 0 || (ano % 4 == 0 && ano % 100 != 0)) {
        diasMes[1] = 29;
    }
    if (dia > diasMes[mes - 1]) {
        return 0;
    }
    return 1;
}

void cadastrarPista() {
    int i;
    Pista nova;

    printf("\n--- CADASTRAR PISTA ---\n");
    if (totalPistas >= MAX_PISTAS) {
        printf("Erro: limite de pistas atingido.\n");
        return;
    }

    nova.id_pista = totalPistas + 1;
    if (!lerTexto("Nome da pista: ", nova.nome, 100)) {
        return;
    }
    if (!lerTexto("Pais: ", nova.pais, 50)) {
        return;
    }
    nova.numero_voltas = lerInteiro("Numero de voltas: ");
    if (nova.numero_voltas <= 0) {
        printf("Erro: o numero de voltas deve ser maior que zero.\n");
        return;
    }

    for (i = 0; i < totalPistas; i++) {
        if (strcmp(pistas[i].nome, nova.nome) == 0 &&
            strcmp(pistas[i].pais, nova.pais) == 0) {
            printf("Erro: pista ja cadastrada.\n");
            return;
        }
    }

    pistas[totalPistas] = nova;
    totalPistas++;
    printf("Pista cadastrada com sucesso! ID: %d\n", nova.id_pista);
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

void cadastrarCorrida() {
    int i;
    Corrida nova;

    printf("\n--- CADASTRAR CORRIDA ---\n");
    if (totalCorridas >= MAX_CORRIDAS) {
        printf("Erro: limite de corridas atingido.\n");
        return;
    }
    if (totalPistas == 0) {
        printf("Erro: cadastre uma pista primeiro.\n");
        return;
    }

    nova.id_corrida = totalCorridas + 1;
    if (!lerTexto("Nome da corrida: ", nova.nome, 100)) {
        return;
    }
    printf("\nPistas disponiveis:\n");
    listarPistas();
    nova.id_pista = lerInteiro("Digite o ID da pista: ");
    if (buscarPista(nova.id_pista) == -1) {
        printf("Erro: pista nao cadastrada.\n");
        return;
    }

    if (!lerTexto("Data da corrida (DD/MM/AAAA): ", nova.data, 11)) {
        return;
    }
    if (!validarData(nova.data)) {
        printf("Erro: data invalida. Use DD/MM/AAAA.\n");
        return;
    }

    for (i = 0; i < totalCorridas; i++) {
        if (strcmp(corridas[i].nome, nova.nome) == 0 &&
            corridas[i].id_pista == nova.id_pista &&
            strcmp(corridas[i].data, nova.data) == 0) {
            printf("Erro: corrida ja cadastrada.\n");
            return;
        }
    }

    corridas[totalCorridas] = nova;
    totalCorridas++;
    printf("Corrida cadastrada com sucesso! ID: %d\n", nova.id_corrida);
}

void listarCorridas() {
    int i;
    printf("\n--- LISTA DE CORRIDAS ---\n");
    if (totalCorridas == 0) {
        printf("Nenhuma corrida cadastrada.\n");
        return;
    }

    for (i = 0; i < totalCorridas; i++) {
        int p = buscarPista(corridas[i].id_pista);
        printf("ID: %d | Nome: %s | Data: %s",
               corridas[i].id_corrida, corridas[i].nome,
               corridas[i].data);
        if (p != -1) {
            printf(" | Pista: %s", pistas[p].nome);
        }
        printf("\n");
    }
}

/* RESULTADOS: BASE DO CODIGO_VOLPS.C */

int pontosPorPosicao(int posicao) {
    int tabela[] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};
    if (posicao < 1 || posicao > 10) {
        return 0;
    }
    return tabela[posicao - 1];
}

int posicaoJaUsada(int id_corrida, int posicao) {
    int i;
    for (i = 0; i < totalResultados; i++) {
        if (resultados[i].id_corrida == id_corrida &&
            resultados[i].posicao == posicao) {
            return 1;
        }
    }
    return 0;
}

int resultadoJaCadastrado(int id_corrida, int id_piloto) {
    int i;
    for (i = 0; i < totalResultados; i++) {
        if (resultados[i].id_corrida == id_corrida &&
            resultados[i].id_piloto == id_piloto) {
            return 1;
        }
    }
    return 0;
}

void registrarResultado() {
    Resultado novo;
    int indiceCorrida, indicePiloto, indiceEquipe;

    printf("\n--- REGISTRAR RESULTADO ---\n");
    if (totalResultados >= MAX_RESULTADOS) {
        printf("Erro: limite de resultados atingido.\n");
        return;
    }

    novo.id_corrida = lerInteiro("Codigo da corrida: ");
    indiceCorrida = buscarCorrida(novo.id_corrida);
    if (indiceCorrida == -1) {
        printf("Erro: corrida nao encontrada.\n");
        return;
    }

    novo.id_piloto = lerInteiro("Codigo do piloto: ");
    indicePiloto = buscarPiloto(novo.id_piloto);
    if (indicePiloto == -1) {
        printf("Erro: piloto nao encontrado.\n");
        return;
    }
    if (resultadoJaCadastrado(novo.id_corrida, novo.id_piloto)) {
        printf("Erro: esse piloto ja tem resultado nesta corrida.\n");
        return;
    }

    novo.posicao = lerInteiro("Posicao de chegada (1 a 20): ");
    if (novo.posicao < 1 || novo.posicao > MAX_POSICOES) {
        printf("Erro: posicao deve estar entre 1 e 20.\n");
        return;
    }
    if (posicaoJaUsada(novo.id_corrida, novo.posicao)) {
        printf("Erro: essa posicao ja foi registrada nesta corrida.\n");
        return;
    }

    indiceEquipe = buscarEquipe(pilotos[indicePiloto].id_equipe);
    if (indiceEquipe == -1) {
        printf("Erro: equipe do piloto nao encontrada.\n");
        return;
    }

    novo.id_resultado = totalResultados + 1;
    novo.pontos = pontosPorPosicao(novo.posicao);
    resultados[totalResultados] = novo;
    totalResultados++;

    /* A pontuacao deve ser acumulada no piloto E na equipe. */
    pilotos[indicePiloto].pontos += novo.pontos;
    equipes[indiceEquipe].pontos += novo.pontos;

    printf("Resultado cadastrado com sucesso!\n");
    printf("Corrida: %s | Piloto: %s | Posicao: %d | Pontos: %d\n",
           corridas[indiceCorrida].nome, pilotos[indicePiloto].nome,
           novo.posicao, novo.pontos);
}

void consultarResultadoCorrida() {
    int i, j;
    int id, indiceCorrida, indicePista;
    Resultado lista[MAX_RESULTADOS];
    Resultado auxiliar;
    int totalLista = 0;

    printf("\n--- CONSULTAR RESULTADO DE CORRIDA ---\n");
    id = lerInteiro("Codigo da corrida: ");
    indiceCorrida = buscarCorrida(id);
    if (indiceCorrida == -1) {
        printf("Erro: corrida nao encontrada.\n");
        return;
    }

    indicePista = buscarPista(corridas[indiceCorrida].id_pista);
    printf("Corrida: %s\n", corridas[indiceCorrida].nome);
    if (indicePista != -1) {
        printf("Pista: %s\n", pistas[indicePista].nome);
    }
    printf("Data: %s\n", corridas[indiceCorrida].data);

    for (i = 0; i < totalResultados; i++) {
        if (resultados[i].id_corrida == id) {
            lista[totalLista] = resultados[i];
            totalLista++;
        }
    }
    if (totalLista == 0) {
        printf("Nenhum resultado cadastrado para esta corrida.\n");
        return;
    }

    /* Organiza as colocacoes da primeira para a ultima. */
    for (i = 0; i < totalLista - 1; i++) {
        for (j = 0; j < totalLista - 1 - i; j++) {
            if (lista[j].posicao > lista[j + 1].posicao) {
                auxiliar = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = auxiliar;
            }
        }
    }

    printf("\n--- CLASSIFICACAO DA CORRIDA ---\n");
    for (i = 0; i < totalLista; i++) {
        int p = buscarPiloto(lista[i].id_piloto);
        if (p != -1) {
            printf("%2d. %-25s | Pontos: %d\n",
                   lista[i].posicao, pilotos[p].nome, lista[i].pontos);
        }
    }
}

/* PESQUISA: BASE DO CODIGO_LE.C + HISTORICO */

void pesquisarPiloto() {
    int i, e, encontrouResultado;
    int opcao, indice = -1;
    char nomeBusca[100];

    printf("\n--- PESQUISAR PILOTO ---\n");
    printf("1 - Pesquisar por codigo\n");
    printf("2 - Pesquisar por nome\n");
    opcao = lerInteiro("Escolha: ");

    if (opcao == 1) {
        int id = lerInteiro("Digite o codigo: ");
        indice = buscarPiloto(id);
    } else if (opcao == 2) {
        if (!lerTexto("Digite o nome: ", nomeBusca, 100)) {
            return;
        }
        for (i = 0; i < totalPilotos; i++) {
            if (strcmp(pilotos[i].nome, nomeBusca) == 0) {
                indice = i;
                break;
            }
        }
    } else {
        printf("Opcao invalida!\n");
        return;
    }

    if (indice == -1) {
        printf("Erro: piloto nao encontrado.\n");
        return;
    }

    e = buscarEquipe(pilotos[indice].id_equipe);
    printf("\nPiloto encontrado:\n");
    printf("Codigo: %d\n", pilotos[indice].id_piloto);
    printf("Nome: %s\n", pilotos[indice].nome);
    printf("Numero: %d\n", pilotos[indice].numero);
    printf("Nacionalidade: %s\n", pilotos[indice].nacionalidade);
    if (e != -1) {
        printf("Equipe: %s\n", equipes[e].nome);
    }
    printf("Total de pontos: %d\n", pilotos[indice].pontos);

    /* Exibe os resultados do piloto em todas as corridas registradas. */
    encontrouResultado = 0;
    printf("\n--- HISTORICO DE RESULTADOS ---\n");
    for (i = 0; i < totalResultados; i++) {
        if (resultados[i].id_piloto == pilotos[indice].id_piloto) {
            int c = buscarCorrida(resultados[i].id_corrida);
            if (c != -1) {
                printf("Corrida: %s | Posicao: %d | Pontos: %d\n",
                       corridas[c].nome, resultados[i].posicao,
                       resultados[i].pontos);
                encontrouResultado = 1;
            }
        }
    }
    if (!encontrouResultado) {
        printf("Nenhum resultado registrado para este piloto.\n");
    }
}

/* MENU PRINCIPAL UNICO */

int main() {
    int opcao;

    do {
        printf("\n========== RACINGGESTOR ==========\n");
        printf(" 1 - Cadastrar Piloto\n");
        printf(" 2 - Cadastrar Equipe\n");
        printf(" 3 - Cadastrar Pista\n");
        printf(" 4 - Listar Pilotos\n");
        printf(" 5 - Listar Equipes\n");
        printf(" 6 - Listar Pistas\n");
        printf(" 7 - Cadastrar Corrida\n");
        printf(" 8 - Registrar Resultado\n");
        printf(" 9 - Listar Corridas\n");
        printf("10 - Consultar Resultado de Corrida\n");
        printf("11 - Pesquisar Piloto\n");
        printf("12 - Sair\n");
        printf("==================================\n");

        opcao = lerInteiro("Escolha uma opcao: ");
        switch (opcao) {
            case 1: cadastrarPiloto(); break;
            case 2: cadastrarEquipe(); break;
            case 3: cadastrarPista(); break;
            case 4: listarPilotos(); break;
            case 5: listarEquipes(); break;
            case 6: listarPistas(); break;
            case 7: cadastrarCorrida(); break;
            case 8: registrarResultado(); break;
            case 9: listarCorridas(); break;
            case 10: consultarResultadoCorrida(); break;
            case 11: pesquisarPiloto(); break;
            case 12: printf("\nEncerrando RacingGestor...\n"); break;
            default: printf("Opcao invalida! Escolha entre 1 e 12.\n");
        }
    } while (opcao != 12 && !feof(stdin));

    return 0;
}
