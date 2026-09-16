# Título do Projeto: RacingGestor — Sistema de Gerenciamento de Campeonato de Fórmula 1

## 1. Descrição do Sistema

O **RacingGestor** é um sistema desenvolvido em linguagem C para auxiliar no gerenciamento de campeonatos de Fórmula 1. Seu objetivo é organizar as principais informações relacionadas às competições, permitindo o cadastro e a consulta de pilotos, equipes, pistas e corridas.

O sistema possibilita registrar os resultados de cada corrida, relacionando a corrida à pista utilizada e armazenando a posição obtida por cada piloto. Dessa forma, o usuário poderá consultar posteriormente os resultados e as informações das corridas cadastradas.

O projeto busca proporcionar uma forma simples e organizada de armazenar e consultar os dados do campeonato, facilitando o acompanhamento das informações dos participantes, das pistas e dos resultados das corridas.

## 2. Fluxo de Utilização Esperado para o Sistema

Ao iniciar o **RacingGestor**, o usuário terá acesso ao menu principal, que apresentará as seguintes opções:

1. **Cadastrar Piloto**
2. **Cadastrar Equipe**
3. **Cadastrar Pista**
4. **Listar Pilotos**
5. **Listar Equipes**
6. **Listar Pistas**
7. **Cadastrar Corrida**
8. **Registrar Resultado**
9. **Listar Corridas**
10. **Consultar Resultado de Corrida**
11. **Pesquisar Piloto**
0. **Sair**

### Funcionamento das opções

- Caso o usuário escolha **1**, o sistema solicitará os dados necessários para cadastrar um novo piloto, como nome, número, nacionalidade e equipe. Após o cadastro, as informações serão armazenadas para utilização no campeonato.
- Caso o usuário escolha **2**, o sistema solicitará os dados da equipe, como código, nome e país, realizando o cadastro da equipe participante.
- Caso o usuário escolha **3**, o sistema solicitará informações da pista, como nome, país e quantidade de voltas, para cadastrá-la no sistema.
- Caso o usuário escolha **4**, o sistema apresentará a lista de todos os pilotos cadastrados, exibindo suas principais informações.
- Caso o usuário escolha **5**, o sistema apresentará a lista de todas as equipes cadastradas.
- Caso o usuário escolha **6**, o sistema apresentará a lista de todas as pistas cadastradas, exibindo suas principais informações.
- Caso o usuário escolha **7**, o sistema permitirá cadastrar uma corrida, informando o nome da corrida, a pista e a data.
- Caso o usuário escolha **8**, o sistema solicitará a corrida e a posição de cada piloto participante. Com base nas posições registradas, serão atribuídos os pontos correspondentes aos pilotos e suas respectivas equipes.
- Caso o usuário escolha **9**, o sistema apresentará a lista de todas as corridas cadastradas, informando seus dados e a pista em que serão realizadas.
- Caso o usuário escolha **10**, o sistema permitirá consultar uma corrida cadastrada, apresentando a pista utilizada e o resultado dos pilotos.
- Caso o usuário escolha **11**, o sistema permitirá pesquisar um piloto utilizando seu código ou nome, apresentando suas informações e seus resultados registrados nas corridas.
- Caso o usuário escolha **0**, o sistema encerrará a execução do programa.

### Tratamento de Erros
  As operações que apresentarem erros, como piloto não encontrado, equipe não cadastrada, corrida inexistente ou cadastro duplicado, deverão exibir mensagens claras ao usuário.
  Caso seja informado um código ou cadastro que não exista, o sistema informará que o registro não foi encontrado. Após cada operação, o usuário poderá retornar ao menu principal e escolher outra opção.

## 3. Fluxograma da Lógica do Sistema

## 4. Estrutura de Dados

O **RacingGestor** utilizará as seguintes estruturas heterogêneas (structs) para organizar e armazenar os dados de pilotos, equipes, pistas, corridas e resultados do campeonato.

```c
// Estrutura para armazenamento dos Pilotos
typedef struct {
    int id_piloto;
    char nome[100];
    int numero;
    char nacionalidade[50];
    int id_equipe;
    int pontos;
} Piloto;

// Estrutura para armazenamento das Equipes
typedef struct {
    int id_equipe;
    char nome[100];
    char pais[50];
    int pontos;
} Equipe;

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
```
