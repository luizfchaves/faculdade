#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
  Alunos:
*/

char FILENAME[11] = "imoveis.txt";

typedef struct {
  char codigo[50];

  float valor;
  int tamanhoM2;
  
  char estadoUF[3];
  char cidade[100];
} imovelStruct;

void print_imovel(imovelStruct imovel) {
  printf("Código: %s\n", imovel.codigo);
  printf("Valor: R$ %.2f\n", imovel.valor);
  printf("Tamanho: %d m²\n", imovel.tamanhoM2);
  printf("Estado: %s\n", imovel.estadoUF);
  printf("Cidade: %s\n", imovel.cidade);
}

void salva_no_arquivo(imovelStruct imovel){
  FILE *file = fopen(FILENAME, "a");
  if (file == NULL) {
		printf("Erro com a abertura do arquivo");
		exit(1); //força saída do programa
	}

  fwrite(&imovel, sizeof(imovelStruct), 1, file);
  fclose(file);
}

void lista_arquivo() {
  FILE *file = fopen(FILENAME, "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  typedef struct listaEncadeadStruct {
    imovelStruct imovel;
    struct listaEncadeadStruct *proximo; 
  } listaEncadeadaStruct;

  listaEncadeadaStruct *cabeca = NULL;
  listaEncadeadaStruct *atual = cabeca;

  
  imovelStruct imovel;
  while (fread(&imovel, sizeof(imovelStruct), 1, file)) {
    listaEncadeadaStruct *novo = malloc(sizeof(listaEncadeadaStruct));
    novo->imovel = imovel;
    novo->proximo = NULL;

    if (cabeca == NULL) {
      cabeca = novo;
      atual = novo;
    } else {
      atual->proximo = novo;
      atual = novo;
    }
  }

  listaEncadeadaStruct *arrumar = cabeca;
  while(arrumar != NULL){
    // float valorAtual = arrumar->imovel.valor;
    listaEncadeadaStruct *valorMenor = arrumar;

    listaEncadeadaStruct *listaTemp = arrumar->proximo;

    while(listaTemp != NULL){
      if(listaTemp->imovel.valor < arrumar->imovel.valor){
        valorMenor = listaTemp;
      }
      listaTemp = listaTemp->proximo;
    }

    if(valorMenor != NULL){
      imovelStruct tempImovel = arrumar->imovel;
      arrumar->imovel = valorMenor->imovel;
      valorMenor->imovel = tempImovel;
    }
    arrumar = arrumar->proximo;
  }

  printf("==========================================================\n");
  printf("Lista de Imóveis ordenada por valor:\n\n");
  atual = cabeca;
  while (atual != NULL) {
    print_imovel(atual->imovel);
    printf("\n");
    atual = atual->proximo;
  }
  printf("==========================================================\n");


  fclose(file);
}

void consulta_arquivo(int (*callback)(imovelStruct)) {
  FILE *file = fopen(FILENAME, "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }
  printf("==========================================================\n");
  printf("Consulta de Imóveis:\n");
  imovelStruct imovel;
  while (fread(&imovel, sizeof(imovelStruct), 1, file)) {
    if (callback(imovel)) {
      printf("\nImóvel encontrado:\n");
      print_imovel(imovel);
    }
  }
  printf("==========================================================\n");
  fclose(file);
}

int altera_no_arquivo(imovelStruct changedImovel) {
  FILE *file = fopen(FILENAME, "r+");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return 0;
  }
  FILE *tempFile = fopen("temp.txt", "w");
  if (tempFile == NULL) {
    printf("Erro ao criar arquivo temporário.\n");
    fclose(file);
    return 0;
  }

  imovelStruct currentImovel;
    while (fread(&currentImovel, sizeof(imovelStruct), 1, file)) {
    if (strcmp(currentImovel.codigo, changedImovel.codigo) == 0) {
      fwrite(&changedImovel, sizeof(imovelStruct), 1, tempFile);
    } else {
      fwrite(&currentImovel, sizeof(imovelStruct), 1, tempFile);
    }
  }

  fclose(file);
  fclose(tempFile);

  if (remove(FILENAME) != 0) {
    printf("Erro ao remover o arquivo original.\n");
    return 0;
  }

  if (rename("temp.txt", FILENAME) != 0) {
    printf("Erro ao renomear o arquivo temporário.\n");
    return 0;
  }

  return 1;  
}

int remove_no_arquivo(char *codigo){
    FILE *file = fopen(FILENAME, "r+");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return 0;
  }
  FILE *tempFile = fopen("temp.txt", "w");
  if (tempFile == NULL) {
    printf("Erro ao criar arquivo temporário.\n");
    fclose(file);
    return 0;
  }

  imovelStruct currentImovel;
    while (fread(&currentImovel, sizeof(imovelStruct), 1, file)) {
    if (strcmp(currentImovel.codigo, codigo) != 0) {
      fwrite(&currentImovel, sizeof(imovelStruct), 1, tempFile);
    }
  }

  fclose(file);
  fclose(tempFile);

  if (remove(FILENAME) != 0) {
    printf("Erro ao remover o arquivo original.\n");
    return 0;
  }

  if (rename("temp.txt", FILENAME) != 0) {
    printf("Erro ao renomear o arquivo temporário.\n");
    return 0;
  }

  return 1;  
}

imovelStruct procura_imovel_por_codigo(char *codigo){
  FILE *file = fopen(FILENAME, "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    system("PAUSE");
    exit(1);
  }
  imovelStruct imovelFound;
  imovelStruct imovel;

  while (fread(&imovel, sizeof(imovelStruct), 1, file)) {
    if (strcmp(imovel.codigo, codigo) == 0) {

      imovelFound.valor = imovel.valor;
      imovelFound.tamanhoM2 = imovel.tamanhoM2;
      strcpy(imovelFound.codigo, imovel.codigo);
      strcpy(imovelFound.estadoUF, imovel.estadoUF);
      strcpy(imovelFound.cidade, imovel.cidade);

      break;
    }
  }

  fclose(file);
  return imovelFound;
}

void cadastra_imovel(){
  printf("Cadastro de Imóvel\n");
  imovelStruct imovel;

  printf("Digite o código do imóvel: ");
  scanf(" %[^\n]", imovel.codigo);
  
  printf("Digite o valor do imóvel: ");
  scanf(" %f", &imovel.valor);

  printf("Digite o tamanho em m² (inteiro): ");
  scanf(" %d", &imovel.tamanhoM2);

  printf("Digite o estado (UF): ");
  scanf(" %[^\n]", imovel.estadoUF);

  printf("Digite a cidade: ");
  scanf(" %[^\n]", imovel.cidade);

  printf("Cadastrando imóvel...\n");
  salva_no_arquivo(imovel);
  printf("\nImóvel cadastrado com sucesso!\n");
}

void altera_imovel(){
  char codigo[50];
  printf("Alteração de Imóvel\n");
  printf("Digite o código do imóvel a ser alterado: ");
  scanf(" %[^\n]", codigo);

  imovelStruct imovel;
  imovel = procura_imovel_por_codigo(codigo);
  if (strcmp(imovel.codigo, "") == 0) {
    printf("Imóvel com código %s não encontrado.\n", codigo);
    return;
  }

  printf("Imóvel encontrado:\n");
  print_imovel(imovel);
  printf("Digite o novo valor do imóvel: ");
  scanf(" %f", &imovel.valor);
  printf("Digite o novo tamanho em m² (inteiro): ");
  scanf(" %d", &imovel.tamanhoM2);
  printf("Digite o novo estado (UF): ");
  scanf(" %[^\n]", imovel.estadoUF);
  printf("Digite a nova cidade: ");
  scanf(" %[^\n]", imovel.cidade);
  printf("Alterando imóvel...\n");

  if (altera_no_arquivo(imovel)) {
    printf("Imóvel alterado com sucesso!\n");
  } else {
    printf("Erro ao alterar o imóvel.\n");
  }
}

void remove_imovel(){
  char codigo[50];
  printf("Exclusão de Imóvel\n");
  printf("Digite o código do imóvel a ser excluído: ");
  scanf(" %[^\n]", codigo);

  imovelStruct imovel;
  imovel = procura_imovel_por_codigo(codigo);
  if (strcmp(imovel.codigo, "") == 0) {
    printf("Imóvel com código %s não encontrado.\n", codigo);
    return;
  }

  printf("Imóvel encontrado:\n");
  print_imovel(imovel);

  int confirmacao;
  printf("Tem certeza que deseja excluir este imóvel? (1 - Sim, 0 - Não): ");
  scanf(" %d", &confirmacao);

  if (confirmacao != 1) {
    printf("Exclusão cancelada.\n");
    return;
  }
  if(remove_no_arquivo(codigo)) {
    printf("Imóvel excluído com sucesso!\n");
  } else {
    printf("Erro ao excluir o imóvel.\n");
  }
}

void consulta_imovel() {
  char codigo[50];
  printf("Consulta de Imóvel\n");
  printf("Digite o que deseja consultar:\n");
  printf("1. Código do imóvel\n");
  printf("2. Estado (UF)\n");
  printf("3. Cidade\n");
  int opcao;
  printf("Escolha uma opção: ");
  scanf(" %d", &opcao);

  switch(opcao){
    case 1:
      printf("Digite o código do imóvel: ");
      scanf(" %[^\n]", codigo);
      int busca_por_codigo(imovelStruct imovel) {
        return strcmp(imovel.codigo, codigo) == 0;
      }
      consulta_arquivo(busca_por_codigo);
      break;
    case 2:
      printf("Digite o estado (UF): ");
      scanf(" %[^\n]", codigo);
      int busca_por_estado(imovelStruct imovel) {
        return strcmp(imovel.estadoUF, codigo) == 0;
      }
      consulta_arquivo(busca_por_estado);
      break;
    case 3:
      printf("Digite a cidade: ");
      scanf(" %[^\n]", codigo);
      int busca_por_cidade(imovelStruct imovel) {
        return strcmp(imovel.cidade, codigo) == 0;
      }
      consulta_arquivo(busca_por_cidade);
      break;
    default:
      printf("Opção inválida.\n");
      return;
  }
}

int main() {
  printf("Olá, este é um programa de cadastro de imóveis.\n");
  while(1){
    printf("\nMenu:\n");
    printf("1. Cadastrar Imóvel\n");
    printf("2. Lista arquivo\n");
    printf("3. Consultar Imóvel\n");
    printf("4. Alterar Imóvel\n");
    printf("5. Excluir Imóvel\n");
    printf("6. Sair\n");
    printf("Escolha uma opção: ");
    
    int opcao, opcaoValida = 0;
    opcaoValida = scanf(" %d", &opcao);
    if(!opcaoValida){
      // https://stackoverflow.com/questions/1716013/why-is-scanf-causing-infinite-loop-in-this-code/12425529
      scanf("%*s"); // Pelo que parece, isso limpa o buffer
      printf("Entrada inválida. Por favor, insira um número.\n");
      continue;
    }
    switch(opcao) {
      case 1:
        cadastra_imovel();
        break;
      case 2:
        lista_arquivo();
        break;
      case 3:
        consulta_imovel();
        break;
      case 4: 
        altera_imovel();
        break;
      case 5:
        remove_imovel();
        break;
      case 6:
        printf("Saindo do programa...\n");
        return 0;
      default:
        printf("Opção inválida. Tente novamente.\n");
    }
  }
  return 0;
}