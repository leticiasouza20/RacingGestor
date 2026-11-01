#include <stdio.h>
 
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
    int id_equipe;
    char nome[100];
    char pais[50];
    int pontos;
} Equipe;
 
int main() {
 
    /* ---------- VARIAVEIS ---------- */
 
    Piloto pilotos[50];
    int qtd_pilotos = 0;
 
    Equipe equipes[20];
    int qtd_equipes = 0;
 
    Piloto novo_piloto;
    Equipe nova_equipe;
 
    int opcao;
    int i;
    int j;           /* posicao da letra ao comparar nomes */
    int iguais;      /* 1 = nomes iguais, 0 = nomes diferentes */
    int valido;      /* 1 = dados ok, 0 = deu erro */
    int achou;       /* 1 = encontrou, 0 = nao encontrou */
    int escolha;
    int codigo_busca;
    char nome_busca[100];
 
    /* ---------- MENU ---------- */
    /* o menu repete ate o usuario digitar 0 */
 
    for (opcao = -1; opcao != 0; ) {
 
        printf("\n===== RACINGGESTOR =====\n");
        printf("1 - Cadastrar Piloto\n");
        printf("2 - Cadastrar Equipe\n");
        printf("3 - Listar Pilotos\n");
        printf("4 - Listar Equipes\n");
        printf("5 - Pesquisar Piloto\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
 
        switch (opcao) {
 
            /* ========== CADASTRAR PILOTO ========== */
            case 1:
                printf("\n--- CADASTRAR PILOTO ---\n");
 
                if (qtd_equipes == 0) {
                    printf("Erro: cadastre uma equipe primeiro.\n");
                } else if (qtd_pilotos == 50) {
                    printf("Erro: limite de pilotos atingido.\n");
                } else {
                    valido = 1;
 
                    /* codigo */
                    printf("Codigo do piloto: ");
                    scanf("%d", &novo_piloto.id_piloto);
 
                    if (novo_piloto.id_piloto <= 0) {
                        printf("Erro: o codigo deve ser maior que zero.\n");
                        valido = 0;
                    }
 
                    /* duplicidade do codigo */
                    if (valido == 1) {
                        for (i = 0; i < qtd_pilotos; i++) {
                            if (pilotos[i].id_piloto == novo_piloto.id_piloto) {
                                printf("Erro: piloto ja cadastrado com esse codigo.\n");
                                valido = 0;
                            }
                        }
                    }
 
                    /* nome */
                    if (valido == 1) {
                        printf("Nome: ");
                        scanf(" %[^\n]", novo_piloto.nome);
 
                        /* duplicidade do nome: compara letra por letra */
                        for (i = 0; i < qtd_pilotos; i++) {
                            iguais = 1;
                            for (j = 0; iguais == 1 && (pilotos[i].nome[j] != '\0' || novo_piloto.nome[j] != '\0'); j++) {
                                if (pilotos[i].nome[j] != novo_piloto.nome[j]) {
                                    iguais = 0;
                                }
                            }
                            if (iguais == 1) {
                                printf("Erro: piloto ja cadastrado com esse nome.\n");
                                valido = 0;
                            }
                        }
                    }
 
                    /* numero */
                    if (valido == 1) {
                        printf("Numero do carro (1 a 99): ");
                        scanf("%d", &novo_piloto.numero);
 
                        if (novo_piloto.numero < 1 || novo_piloto.numero > 99) {
                            printf("Erro: o numero deve estar entre 1 e 99.\n");
                            valido = 0;
                        }
                    }
 
                    /* duplicidade do numero */
                    if (valido == 1) {
                        for (i = 0; i < qtd_pilotos; i++) {
                            if (pilotos[i].numero == novo_piloto.numero) {
                                printf("Erro: esse numero ja pertence a outro piloto.\n");
                                valido = 0;
                            }
                        }
                    }
 
                    /* nacionalidade */
                    if (valido == 1) {
                        printf("Nacionalidade: ");
                        scanf(" %[^\n]", novo_piloto.nacionalidade);
                    }
 
                    /* equipe (precisa existir) */
                    if (valido == 1) {
                        printf("Codigo da equipe: ");
                        scanf("%d", &novo_piloto.id_equipe);
 
                        achou = 0;
                        for (i = 0; i < qtd_equipes; i++) {
                            if (equipes[i].id_equipe == novo_piloto.id_equipe) {
                                achou = 1;
                            }
                        }
 
                        if (achou == 0) {
                            printf("Erro: equipe nao cadastrada.\n");
                            valido = 0;
                        }
                    }
 
                    /* se passou por tudo, salva */
                    if (valido == 1) {
                        novo_piloto.pontos = 0;
                        pilotos[qtd_pilotos] = novo_piloto;
                        qtd_pilotos++;
                        printf("Piloto cadastrado com sucesso!\n");
                    }
                }
                break;
 
            /* ========== CADASTRAR EQUIPE ========== */
            case 2:
                printf("\n--- CADASTRAR EQUIPE ---\n");
 
                if (qtd_equipes == 20) {
                    printf("Erro: limite de equipes atingido.\n");
                } else {
                    valido = 1;
 
                    /* codigo */
                    printf("Codigo da equipe: ");
                    scanf("%d", &nova_equipe.id_equipe);
 
                    if (nova_equipe.id_equipe <= 0) {
                        printf("Erro: o codigo deve ser maior que zero.\n");
                        valido = 0;
                    }
 
                    /* duplicidade do codigo */
                    if (valido == 1) {
                        for (i = 0; i < qtd_equipes; i++) {
                            if (equipes[i].id_equipe == nova_equipe.id_equipe) {
                                printf("Erro: equipe ja cadastrada com esse codigo.\n");
                                valido = 0;
                            }
                        }
                    }
 
                    /* nome */
                    if (valido == 1) {
                        printf("Nome da equipe: ");
                        scanf(" %[^\n]", nova_equipe.nome);
 
                        /* duplicidade do nome: compara letra por letra */
                        for (i = 0; i < qtd_equipes; i++) {
                            iguais = 1;
                            for (j = 0; iguais == 1 && (equipes[i].nome[j] != '\0' || nova_equipe.nome[j] != '\0'); j++) {
                                if (equipes[i].nome[j] != nova_equipe.nome[j]) {
                                    iguais = 0;
                                }
                            }
                            if (iguais == 1) {
                                printf("Erro: equipe ja cadastrada com esse nome.\n");
                                valido = 0;
                            }
                        }
                    }
 
                    /* pais */
                    if (valido == 1) {
                        printf("Pais: ");
                        scanf(" %[^\n]", nova_equipe.pais);
                    }
 
                    /* se passou por tudo, salva */
                    if (valido == 1) {
                        nova_equipe.pontos = 0;
                        equipes[qtd_equipes] = nova_equipe;
                        qtd_equipes++;
                        printf("Equipe cadastrada com sucesso!\n");
                    }
                }
                break;
 
            /* ========== LISTAR PILOTOS ========== */
            case 3:
                printf("\n--- LISTA DE PILOTOS ---\n");
 
                if (qtd_pilotos == 0) {
                    printf("Nenhum piloto cadastrado.\n");
                } else {
                    for (i = 0; i < qtd_pilotos; i++) {
                        printf("Codigo: %d | Nome: %s | Numero: %d | Nacionalidade: %s | Equipe: %d | Pontos: %d\n",
                               pilotos[i].id_piloto,
                               pilotos[i].nome,
                               pilotos[i].numero,
                               pilotos[i].nacionalidade,
                               pilotos[i].id_equipe,
                               pilotos[i].pontos);
                    }
                }
                break;
 
            /* ========== LISTAR EQUIPES ========== */
            case 4:
                printf("\n--- LISTA DE EQUIPES ---\n");
 
                if (qtd_equipes == 0) {
                    printf("Nenhuma equipe cadastrada.\n");
                } else {
                    for (i = 0; i < qtd_equipes; i++) {
                        printf("Codigo: %d | Nome: %s | Pais: %s | Pontos: %d\n",
                               equipes[i].id_equipe,
                               equipes[i].nome,
                               equipes[i].pais,
                               equipes[i].pontos);
                    }
                }
                break;
 
            /* ========== PESQUISAR PILOTO ========== */
            case 5:
                printf("\n--- PESQUISAR PILOTO ---\n");
                printf("1 - Pesquisar por codigo\n");
                printf("2 - Pesquisar por nome\n");
                printf("Escolha: ");
                scanf("%d", &escolha);
 
                achou = 0;
 
                if (escolha == 1) {
                    printf("Digite o codigo: ");
                    scanf("%d", &codigo_busca);
 
                    for (i = 0; i < qtd_pilotos; i++) {
                        if (pilotos[i].id_piloto == codigo_busca) {
                            printf("\nPiloto encontrado:\n");
                            printf("Codigo: %d\n", pilotos[i].id_piloto);
                            printf("Nome: %s\n", pilotos[i].nome);
                            printf("Numero: %d\n", pilotos[i].numero);
                            printf("Nacionalidade: %s\n", pilotos[i].nacionalidade);
                            printf("Equipe: %d\n", pilotos[i].id_equipe);
                            printf("Pontos: %d\n", pilotos[i].pontos);
                            achou = 1;
                        }
                    }
 
                    if (achou == 0) {
                        printf("Piloto nao encontrado.\n");
                    }
                } else if (escolha == 2) {
                    printf("Digite o nome: ");
                    scanf(" %[^\n]", nome_busca);
 
                    for (i = 0; i < qtd_pilotos; i++) {
                        /* compara letra por letra */
                        iguais = 1;
                        for (j = 0; iguais == 1 && (pilotos[i].nome[j] != '\0' || nome_busca[j] != '\0'); j++) {
                            if (pilotos[i].nome[j] != nome_busca[j]) {
                                iguais = 0;
                            }
                        }
 
                        if (iguais == 1) {
                            printf("\nPiloto encontrado:\n");
                            printf("Codigo: %d\n", pilotos[i].id_piloto);
                            printf("Nome: %s\n", pilotos[i].nome);
                            printf("Numero: %d\n", pilotos[i].numero);
                            printf("Nacionalidade: %s\n", pilotos[i].nacionalidade);
                            printf("Equipe: %d\n", pilotos[i].id_equipe);
                            printf("Pontos: %d\n", pilotos[i].pontos);
                            achou = 1;
                        }
                    }
 
                    if (achou == 0) {
                        printf("Piloto nao encontrado.\n");
                    }
                } else {
                    printf("Opcao invalida!\n");
                }
                break;
 
            case 0:
                printf("Saindo...\n");
                break;
 
            default:
                printf("Opcao invalida!\n");
        }
    }
 
    return 0;
}
