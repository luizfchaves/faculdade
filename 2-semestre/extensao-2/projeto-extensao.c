
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define PRECO_QUILO 50.0
#define PRECO_QUENTINHA 20.0
#define PRECO_EMBALAGEM 0.5

/**
 * Atividade de extensão 2
 *
 * Alunos:
 - Luiz Fernando de Souza Chaves - 1240209236
  - Gabriel Lopes Rocha - 1240206639
  - Matheus Rodrigues Medeiros - 1240210236
  - João Vitor da Cruz Miraglia - 1240206056
 */

typedef enum {
  MENU_SAIR = -1,
  MENU_INICIO = 1,
  MENU_CADASTRO_VENDA = 2,
  MENU_LISTA_DIARIA = 3,
  MENU_LISTA_MENSAL = 4
} menuEnum;
typedef enum { REFEICAO_QUENTINHA, REFEICAO_A_QUILO } refeicaoEnum;

typedef struct {
  int dia;
  int mes;
  float valor_comida;
  int peso;
  float valor_bebidas;
  int quantidade_bebidas;
  refeicaoEnum tipoRefeicao;

  struct vendaStruct *proxima_venda;
} vendaStruct;

int dia, mes;

void pause_terminal() {
  // o scanf deixa um \n no buffer, então preciso utilizar um getchar a mais
  // para remover o \n
  getchar();

  printf("Pressione Enter para continuar...");
  getchar();
}
void clear_screen() {
  int funcionou = system("cls");
  // Tentando ser espertinho:
  // system retorna 0 se foi com sucesso e diferente de 0 se foi com erro, então
  // se cls não funcionou, provavelmente vai funcionar com o clear
  if (funcionou != 0) {
    system("clear");
  }
}

void salva_venda(vendaStruct venda) {
  FILE *file = fopen("arquivo.txt", "a");
  if (file == NULL) {
    printf("Erro ao abrir arquivo");
    exit(1);
  }

  fwrite(&venda, sizeof(vendaStruct), 1, file);

  fclose(file);
}

void inicia_programa() {
  printf("Olá!\n");
  int sucesso = 0;
  while (sucesso != 1) {
    printf("Que dia é hoje? ");
    scanf(" %d", &dia);

    if (dia >= 1 && dia <= 31) {
      sucesso = 1;
    } else {
      printf("Dia inválido. Tente novamente.\n");
    }
  }

  sucesso = 0;
  while (sucesso != 1) {
    printf("Que mês é hoje? ");
    scanf(" %d", &mes);

    if (mes >= 1 && mes <= 12) {
      sucesso = 1;
    } else {
      printf("Mês inválido. Tente novamente.\n");
    }
  }
}

menuEnum menu_inicio() {
  printf("Menu Inicial\n");
  printf("1. Cadastro de Venda\n");
  printf("2. Consultar vendas do dia\n");
  printf("3. Sair\n");

  int opcao;
  scanf("%d", &opcao);

  switch (opcao) {
  case 1:
    return MENU_CADASTRO_VENDA;
  case 2:
    return MENU_LISTA_DIARIA;

  case 3:
    return MENU_SAIR;
  default:
    printf("Opção inválida. Tente novamente.\n");
    return MENU_INICIO;
  }
}

menuEnum menu_cadastro_venda() {

  printf("Cadastro de Venda\n\n");

  int sucesso = 0;
  refeicaoEnum refeicao;

  while (sucesso != 1) {
    printf("1. Quentinha\n");
    printf("2. A quilo\n");
    printf("Digite o item da refeição: ");

    int opcao;
    scanf("%d", &opcao);

    sucesso = 1;
    if (opcao == 1) {
      refeicao = REFEICAO_QUENTINHA;
      printf("Você escolheu Quentinha.\n");
    } else if (opcao == 2) {
      refeicao = REFEICAO_A_QUILO;
      printf("Você escolheu A quilo.\n");
    } else {
      printf("Opção inválida. Tente novamente.\n");
      sucesso = 0;
    }
  }

  int peso = 0;
  float valor_comida;
  if (refeicao == REFEICAO_QUENTINHA) {
    valor_comida = PRECO_QUENTINHA;
    valor_comida += PRECO_EMBALAGEM;
  } else if (refeicao == REFEICAO_A_QUILO) {
    printf("Qual o peso da refeição (Em gramas)? ");
    scanf("%d", &peso);
    valor_comida = peso * PRECO_QUILO / 1000.0f;
  }

  printf("O preço da refeição é: R$%.2f\n", valor_comida);

  bool adicionar_bebidas = 0;
  sucesso = 0;
  while (sucesso != 1) {
    printf("Deseja adicionar bebidas? \n");
    printf("1. Sim\n");
    printf("2. Não\n");

    int opcao;
    scanf("%d", &opcao);

    if (opcao == 1 || opcao == 2) {
      sucesso = 1;
      adicionar_bebidas = (opcao == 1);
    } else {
      printf("Opção inválida. Tente novamente.\n");
    }
  }

  int quantidade_bebidas = 0;
  float valor_bebidas = 0.0f;
  if (adicionar_bebidas) {
    printf("Qual o valor?");
    scanf("%f", &valor_bebidas);

    printf("Quantas bebidas? ");
    scanf("%d", &quantidade_bebidas);

    printf("Você escolheu adicionar bebidas no valor de R$%.2f\n",
           valor_bebidas);
  }

  clear_screen();
  printf("O preço total da refeição é: R$%.2f\n", valor_comida + valor_bebidas);

  printf("Resumo da venda:\n");
  printf("Tipo de refeição: %s\n",
         (refeicao == REFEICAO_QUENTINHA) ? "Quentinha" : "A quilo");
  printf("Peso da refeição: %d gramas\n", peso);
  printf("Valor da refeição: R$%.2f\n", valor_comida);
  printf("Bebidas foram adicionadas? %s\n", adicionar_bebidas ? "Sim" : "Não");
  if (adicionar_bebidas) {
    printf("Quantidade de bebidas: %d\n", quantidade_bebidas);
    printf("Valor total das bebidas: R$%.2f\n", valor_bebidas);
  }

  sucesso = 0;

  while (sucesso != 1) {
    printf("Confirmar venda?\n");
    printf("1. Sim\n");
    printf("2. Não\n");
    int opcao;
    scanf("%d", &opcao);
    if (opcao == 1) {
      printf("Venda confirmada!\n");
      sucesso = 1;

      vendaStruct venda;
      venda.dia = dia;
      venda.mes = mes;
      venda.valor_comida = valor_comida;
      venda.peso = peso;
      venda.valor_bebidas = valor_bebidas;
      venda.quantidade_bebidas = quantidade_bebidas;
      venda.tipoRefeicao = refeicao;

      salva_venda(venda);
      return MENU_INICIO;
    } else if (opcao == 2) {
      printf("Venda cancelada!\n");
      sucesso = 1;
      return MENU_CADASTRO_VENDA;
    } else {
      printf("Opção inválida. Tente novamente.\n");
    }
  }

  return MENU_INICIO;
}

menuEnum menu_lista_diaria() {
  FILE *file = fopen("arquivo.txt", "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    exit(1);
  }

  vendaStruct venda;
  typedef struct listaEncadeadStruct {
    int dia;
    int mes;
    float valorTotal;
    int pesoTotal;
    int quantidadeQuentinhas;

    struct listaEncadeadStruct *proximo;
  } listaEncadeadaStruct;

  listaEncadeadaStruct *cabeca = NULL;
  listaEncadeadaStruct *atual = cabeca;

  while (fread(&venda, sizeof(vendaStruct), 1, file) == 1) {
    printf("Lendo venda: %d/%d\n", venda.dia, venda.mes);
    listaEncadeadaStruct *listaTemp = cabeca;
    while (listaTemp != NULL) {
      if (listaTemp->dia == venda.dia && listaTemp->mes == venda.mes) {
        break;
      }
      listaTemp = listaTemp->proximo;
    }
    if (listaTemp == NULL) {
      listaEncadeadaStruct *novo = malloc(sizeof(listaEncadeadaStruct));
      novo->dia = venda.dia;
      novo->mes = venda.mes;
      novo->valorTotal = venda.valor_comida + venda.valor_bebidas;
      novo->pesoTotal = venda.peso;
      novo->quantidadeQuentinhas = 0;
      if (venda.tipoRefeicao == REFEICAO_QUENTINHA) {
        novo->quantidadeQuentinhas = 1;
      }

      novo->proximo = NULL;

      if (cabeca == NULL) {
        cabeca = novo;
        atual = novo;
      } else {
        atual->proximo = novo;
        atual = novo;
      }
    } else {
      listaTemp->valorTotal += venda.valor_comida + venda.valor_bebidas;
      listaTemp->pesoTotal += venda.peso;
      if (venda.tipoRefeicao == REFEICAO_QUENTINHA) {
        listaTemp->quantidadeQuentinhas += 1;
      }
    }
  }

  fclose(file);
  if (cabeca == NULL) {
    printf("Nenhuma venda registrada.\n");
    pause_terminal();
    return MENU_INICIO;
  }

  printf("Relatório de Vendas do Diário\n");
  atual = cabeca;

  while (atual != NULL) {
    printf("%d/%d\n", atual->dia, atual->mes);
    printf("  Número de quentinhas: %d\n", atual->quantidadeQuentinhas);
    printf("  Peso Total em vendas à quilo: %d\n", atual->pesoTotal);
    printf("  Valor Total: R$%.2f\n", atual->valorTotal);

    atual = atual->proximo;
    free(atual);
  }
  printf("Vendas lidas com sucesso!\n");

  pause_terminal();
  return MENU_INICIO;
}

menuEnum menu_lista_mensal() {
  FILE *file = fopen("arquivo.txt", "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    exit(1);
  }

  vendaStruct venda;
  typedef struct listaEncadeadStruct {
    int mes;
    float valorTotal;
    int pesoTotal;
    int quantidadeQuentinhas;

    struct listaEncadeadStruct *proximo;
  } listaEncadeadaStruct;

  listaEncadeadaStruct *cabeca = NULL;
  listaEncadeadaStruct *atual = cabeca;

  while (fread(&venda, sizeof(vendaStruct), 1, file) == 1) {
    listaEncadeadaStruct *listaTemp = cabeca;
    while (listaTemp != NULL) {
      if (listaTemp->mes == venda.mes) {
        break;
      }
      listaTemp = listaTemp->proximo;
    }
    if (listaTemp == NULL) {
      listaEncadeadaStruct *novo = malloc(sizeof(listaEncadeadaStruct));
      novo->mes = venda.mes;
      novo->valorTotal = venda.valor_comida + venda.valor_bebidas;
      novo->pesoTotal = venda.peso;
      novo->quantidadeQuentinhas = 0;
      if (venda.tipoRefeicao == REFEICAO_QUENTINHA) {
        novo->quantidadeQuentinhas = 1;
      }

      novo->proximo = NULL;

      if (cabeca == NULL) {
        cabeca = novo;
        atual = novo;
      } else {
        atual->proximo = novo;
        atual = novo;
      }
    } else {
      listaTemp->valorTotal += venda.valor_comida + venda.valor_bebidas;
      listaTemp->pesoTotal += venda.peso;
      if (venda.tipoRefeicao == REFEICAO_QUENTINHA) {
        listaTemp->quantidadeQuentinhas += 1;
      }
    }
  }

  fclose(file);
  if (cabeca == NULL) {
    printf("Nenhuma venda registrada.\n");
    system("pause");

    return MENU_INICIO;
  }

  printf("Vendas.\n");
  atual = cabeca;

  while (atual != NULL) {
    printf("Mês: %d\n", atual->mes);
    printf("Número de quentinhas: %d\n", atual->quantidadeQuentinhas);
    printf("Peso Total: %d\n", atual->pesoTotal);
    printf("Valor Total: R$%.2f\n", atual->valorTotal);

    atual = atual->proximo;
    free(atual);
  }
  printf("Vendas lidas com sucesso!\n");

  system("pause");

  return MENU_INICIO;
}

int main() {
  setlocale(LC_ALL, "pt_BR.UTF-8");

#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif
  clear_screen();
  inicia_programa();

  menuEnum menuAtual = MENU_INICIO;

  while (menuAtual != MENU_SAIR) {
    clear_screen();
    printf("Hoje é dia %d do mês %d\n", dia, mes);

    switch (menuAtual) {
    case MENU_INICIO:
      menuAtual = menu_inicio();
      break;

    case MENU_CADASTRO_VENDA:
      menuAtual = menu_cadastro_venda();
      break;

    case MENU_LISTA_DIARIA:
      menuAtual = menu_lista_diaria();
      break;

    case MENU_LISTA_MENSAL:
      menuAtual = menu_lista_mensal();
      break;

    default:
      break;
    }
  }
  return 1;
}
