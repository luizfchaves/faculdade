#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define PRECO_QUILO 10.0
#define PRECO_QUENTINHA 20.0
#define PRECO_EMBALAGEM 0.5

/**
 * Atividade de extensão 2
 * 
 * Alunos:
 * 
 * 
 * 
 * 
 * 
 */

typedef enum {
  MENU_SAIR = -1,
  MENU_INICIO = 1,
  MENU_CADASTRO_VENDA = 2,
} menuEnum;

typedef struct {
  int dia;
  int mes;
  float valor_comida;
  int peso;
  float valor_bebidas;
  int quantidade_bebidas;

  struct vendaStruct *proxima_venda;
} vendaStruct;

int dia, mes;

void clear_screen() {
  system("clear | cls");
}

void salva_venda(vendaStruct venda){
  // data, mes, valor_comida, peso, valor_bebidas, quantidade_bebidas

  // Verifica se o arquivo existe antes
  FILE *file = fopen("arquivo.txt", "r");
  bool file_exists = file != NULL;
  if (file_exists) fclose(file);

  file = fopen("arquivo.txt", "a");
  if (!file_exists) {
    fprintf(file, "dia,mes,valor_comida,peso,valor_bebidas,quantidade_bebidas\n");
    // fwrite(file, &vendaStruct, sizeof(vendaStruct), 1, file);
  }

//  fprintf(file, "%d,%d,%.2f,%.2f,%.2f,%d\n", dia, mes, venda.valor_comida, venda.peso, venda.valor_bebidas, venda.quantidade_bebidas);
  fclose(file);
}

void le_vendas(){
  FILE *file = fopen("arquivo.txt", "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  char linha[256];
  while (fgets(linha, sizeof(linha), file)) {
    printf("%s", linha);
  }
  fclose(file);
  printf("Vendas lidas com sucesso!\n");
}


void inicia_programa(){
  printf("Olá!\n");
  int sucesso = 0;
  while(sucesso != 1){
    printf("Que dia é hoje? ");
    scanf("%d", &dia);

    if(dia >= 1 && dia <= 31){
      sucesso = 1;
    } else {
      printf("Dia inválido. Tente novamente.\n");
    }
  }

  sucesso = 0;
  while(sucesso != 1){
    printf("Que mês é hoje? ");
    scanf("%d", &mes);

    if(mes >= 1 && mes <= 12){
      sucesso = 1;
    } else {
      printf("Mês inválido. Tente novamente.\n");
    }
  }
}

menuEnum menu_inicio(){
  printf("Menu Inicial\n");
  printf("1. Cadastro de Venda\n");
  printf("2. Sair\n");
  
  int opcao;
  scanf("%d", &opcao);

  switch(opcao) {
    case 1:
      return MENU_CADASTRO_VENDA;
    case 2:
      return MENU_SAIR;
    default:
      printf("Opção inválida. Tente novamente.\n");
      return MENU_INICIO;
  }
}

menuEnum menu_cadastro_venda(){
  typedef enum{
    REFEICAO_QUENTINHA,
    REFEICAO_A_QUILO
  } refeicaoEnum;

  printf("Cadastro de Venda\n\n");
  
  int sucesso = 0;
  refeicaoEnum refeicao;

  while(sucesso != 1){
    printf("1. Quentinha\n");
    printf("2. A quilo\n");
    printf("Digite o item da refeição: ");
    
    int opcao;
    scanf("%d", &opcao);

    sucesso = 1;
    if(opcao == 1){
      refeicao = REFEICAO_QUENTINHA;
      printf("Você escolheu Quentinha.\n");
    } else if(opcao == 2){
      refeicao = REFEICAO_A_QUILO;
      printf("Você escolheu A quilo.\n");
    } else {
      printf("Opção inválida. Tente novamente.\n");
      sucesso = 0;
    }
  }

  int peso;
  printf("Qual o peso da refeição (Em gramas)? ");
  scanf("%d", &peso);

  float valor_comida;
  if(refeicao == REFEICAO_QUENTINHA){
    valor_comida = PRECO_QUENTINHA;
    valor_comida += PRECO_EMBALAGEM;
  }
  else if(refeicao == REFEICAO_A_QUILO){
    valor_comida = peso * PRECO_QUILO;
  }

  printf("O preço da refeição é: R$%.2f\n", valor_comida);

  bool adicionar_bebidas = 0;
  sucesso = 0;
  while(sucesso != 1){
    printf("Deseja adicionar bebidas? \n");
    printf("1. Sim\n");
    printf("2. Não\n");

    int opcao;
    scanf("%d", &opcao);

    if(opcao == 1 || opcao == 2){
      sucesso = 1;
      adicionar_bebidas = (opcao == 1);
    } else {
      printf("Opção inválida. Tente novamente.\n");
    }
  }

  int quantidade_bebidas = 0;
  float valor_bebidas = 0.0f;
  if(adicionar_bebidas){
    printf("Qual o valor?");
    scanf("%f", &valor_bebidas);

    printf("Quantas bebidas? ");
    scanf("%d", &quantidade_bebidas);

    printf("Você escolheu adicionar bebidas no valor de R$%.2f\n", valor_bebidas);
  }

  clear_screen();
  printf("O preço total da refeição é: R$%.2f\n", valor_comida + valor_bebidas);

  printf("Resumo da venda:\n");
  printf("Tipo de refeição: %s\n", (refeicao == REFEICAO_QUENTINHA) ? "Quentinha" : "A quilo");
  printf("Peso da refeição: %d gramas\n", peso);
  printf("Valor da refeição: R$%.2f\n", valor_comida);
  printf("Bebidas foram adicionadas? %s\n", adicionar_bebidas ? "Sim" : "Não");
  if(adicionar_bebidas){
    printf("Quantidade de bebidas: %d\n", quantidade_bebidas);
    printf("Valor total das bebidas: R$%.2f\n", valor_bebidas);
  }

  sucesso = 0;

  while(sucesso != 1){
    printf("Confirmar venda?\n");
    printf("1. Sim\n");
    printf("2. Não\n");
    int opcao;
    scanf("%d", &opcao);
    if(opcao == 1){
      printf("Venda confirmada!\n");
      sucesso = 1;

      vendaStruct venda;
      venda.dia = dia;
      venda.mes = mes;
      venda.valor_comida = valor_comida;
      venda.peso = peso;
      venda.valor_bebidas = valor_bebidas;
      venda.quantidade_bebidas = quantidade_bebidas;

      salva_venda(venda);
      return MENU_INICIO;
    } else if(opcao == 2){
      printf("Venda cancelada!\n");
      sucesso = 1;
      return MENU_CADASTRO_VENDA;
    } else {
      printf("Opção inválida. Tente novamente.\n");
    }

  }

  // Entender se está certo
  return MENU_INICIO; 
}

int main() {
  clear_screen();
  inicia_programa();
  printf("Hoje é dia %d do mês %d\n", dia, mes);

  menuEnum menuAtual = MENU_INICIO;
  
  while(menuAtual != MENU_SAIR){
    clear_screen();

    switch (menuAtual) {
      case MENU_INICIO:
        menuAtual = menu_inicio();
        break;
    
      case MENU_CADASTRO_VENDA:
        menuAtual = menu_cadastro_venda();
        break;
      default:
        break;
    }

  }
  return 1;
}
