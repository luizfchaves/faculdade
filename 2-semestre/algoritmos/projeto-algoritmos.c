#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
  Alunos:
  - Luiz Fernando de Souza Chaves - 1240209236
  - Gabriel Lopes Rocha - 1240206639
  - Matheus Rodrigues Medeiros - 1240210236
  - João Vitor da Cruz Miraglia - 1240206056
*/

char FILENAME[11] = "imoveis.txt";
int isLinux = 1;

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
void limpa_terminal() {
  int funcionou = system("clear");
  // system retorna 0 se foi com sucesso e -1 se foi com erro, então se clear
  // não funcionou, provavelmente vai funcionar com o cls
  if (funcionou == -1) {
    isLinux = 0;
  }

  if (isLinux) {
    system("clear");
  } else {
    system("cls");
  }
}
void pause_terminal() {
  // Pelo que entendi, o scanf deixa um \n no buffer, então preciso utilizar
  // esse getchar para remover o \n
  getchar();

  printf("Pressione Enter para continuar...");
  getchar();
}

void salva_no_arquivo(imovelStruct imovel) {
  FILE *file = fopen(FILENAME, "a");
  if (file == NULL) {
    printf("Erro com a abertura do arquivo");
    exit(1);
  }

  fwrite(&imovel, sizeof(imovelStruct), 1, file);
  fclose(file);
}

void lista_arquivo() {
  FILE *file = fopen(FILENAME, "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    exit(1);
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

  // Ordenando a lista encadeada por valor
  // Arrumar é o item atual que está sendo verificado
  // A função percorre a lista e encontra o menor valor
  // Depois troca o valor do item atual com o menor valor encontrado
  // Repete isso até que todos os itens estejam ordenados

  listaEncadeadaStruct *arrumar = cabeca;
  while (arrumar != NULL) {
    listaEncadeadaStruct *valorMenor = arrumar;

    listaEncadeadaStruct *listaTemp = arrumar->proximo;

    while (listaTemp != NULL) {
      if (listaTemp->imovel.valor < valorMenor->imovel.valor) {
        valorMenor = listaTemp;
      }
      listaTemp = listaTemp->proximo;
    }

    imovelStruct tempImovel = arrumar->imovel;
    arrumar->imovel = valorMenor->imovel;
    valorMenor->imovel = tempImovel;

    arrumar = arrumar->proximo;
  }

  limpa_terminal();
  printf("==========================================================\n");
  printf("Lista de Imóveis ordenada por valor:\n\n");
  atual = cabeca;
  while (atual != NULL) {
    print_imovel(atual->imovel);
    printf("\n");

    listaEncadeadaStruct *temp = atual;
    atual = atual->proximo;
    free(temp);
  }
  printf("==========================================================\n");

  fclose(file);
  pause_terminal();
}

void consulta_arquivo(char codigo[50]) {

  FILE *file = fopen(FILENAME, "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }
  limpa_terminal();
  printf("==========================================================\n");
  printf("Consulta de Imóveis:\n");
  imovelStruct imovel;
  int encontrado = 0;
  while (fread(&imovel, sizeof(imovelStruct), 1, file)) {
    if (strcmp(imovel.codigo, codigo) == 0) {
      printf("\nImóvel encontrado:\n");
      print_imovel(imovel);
      encontrado = 1;
    }
  }

  if (!encontrado) {
    printf("Nenhum imóvel encontrado com o código: %s\n", codigo);
  }
  printf("==========================================================\n");
  fclose(file);
  pause_terminal();
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

int remove_no_arquivo(char *codigo) {
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

imovelStruct procura_imovel_por_codigo(char *codigo) {
  FILE *file = fopen(FILENAME, "r");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    system("PAUSE");
    exit(1);
  }
  imovelStruct imovelFound;

  // Incializando para comparar depois
  strcpy(imovelFound.codigo, "");
  imovelFound.valor = 0.0;
  imovelFound.tamanhoM2 = 0;
  strcpy(imovelFound.estadoUF, "");
  strcpy(imovelFound.cidade, "");

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

void cadastra_imovel() {
  printf("Cadastro de Imóvel\n");
  imovelStruct imovel;

  do {
    printf("Digite o código do imóvel: ");
    scanf(" %[^\n]", imovel.codigo);

    imovelStruct imovel_existente = procura_imovel_por_codigo(imovel.codigo);
    if (strcmp(imovel_existente.codigo, "") != 0) {
      printf("Imóvel com código %s já cadastrado.\n", imovel.codigo);
      printf("Deseja sair? (1. Sim, [Qualquer outro]. Não): ");

      int opcao;
      scanf(" %d", &opcao);
      if (opcao == 1) {
        printf("Cadastro cancelado.\n");
        return;
      }
      continue;
    }

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
    break;
  } while (1);

  pause_terminal();
}

void altera_imovel() {
  limpa_terminal();

  char codigo[50];
  printf("Alteração de Imóvel\n");
  do {
    printf("Digite o código do imóvel a ser alterado: ");
    scanf(" %[^\n]", codigo);

    imovelStruct imovel;
    imovel = procura_imovel_por_codigo(codigo);

    if (strcmp(imovel.codigo, "") == 0) {
      printf("Imóvel com código %s não exite.\n", imovel.codigo);
      printf("Deseja sair? (1. Sim, [Qualquer outro]. Não): ");

      int opcao;
      scanf(" %d", &opcao);

      if (opcao == 1) {
        printf("Alteração cancelada.\n");
        pause_terminal();
        return;
      }
      continue;
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
      pause_terminal();
      break;
    } else {
      printf("Erro ao alterar o imóvel.\n");
    }
  } while (1);
}

void remove_imovel() {
  limpa_terminal();

  char codigo[50];
  printf("Exclusão de Imóvel\n");
  printf("Digite o código do imóvel a ser excluído: ");
  scanf(" %[^\n]", codigo);

  imovelStruct imovel;
  imovel = procura_imovel_por_codigo(codigo);
  if (strcmp(imovel.codigo, "") == 0) {
    printf("Imóvel com código %s não encontrado.\n", codigo);
    pause_terminal();
    return;
  }

  printf("Imóvel encontrado:\n");
  print_imovel(imovel);

  int confirmacao;
  printf("Tem certeza que deseja excluir este imóvel? (1.Sim, [Qualquer "
         "outro]. Não): ");
  scanf(" %d", &confirmacao);

  if (confirmacao != 1) {
    printf("Exclusão cancelada.\n");
  } else if (remove_no_arquivo(codigo)) {
    printf("Imóvel excluído com sucesso!\n");
  } else {
    printf("Erro ao excluir o imóvel.\n");
  }

  pause_terminal();
}

void consulta_imovel() {
  char codigo[50];
  printf("Consulta de Imóvel\n");
  printf("Digite o código do imóvel que deseja consultar: ");
  scanf(" %[^\n]", codigo);

  consulta_arquivo(codigo);
}
int main() {
  while (1) {
    limpa_terminal();
    printf("\nMenu:\n");
    printf("1. Cadastrar Imóvel\n");
    printf("2. Lista Imóveis\n");
    printf("3. Consultar Imóvel\n");
    printf("4. Alterar Imóvel\n");
    printf("5. Excluir Imóvel\n");
    if (isLinux) {
      printf("6. Alterar para limpeza de terminal do Windows\n");
    } else {
      printf("6. Alterar para limpeza de terminal do Linux\n");
    }
    printf("7. Sair\n");
    printf("Escolha uma opção: ");

    int opcao, opcaoValida = 0;
    opcaoValida = scanf(" %d", &opcao);
    if (!opcaoValida) {
      // https://stackoverflow.com/questions/1716013/why-is-scanf-causing-infinite-loop-in-this-code/12425529
      scanf("%*s"); // Pelo que parece, isso limpa o buffer
      printf("Entrada inválida. Por favor, insira um número.\n");
      continue;
    }

    switch (opcao) {
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
      // Ternário
      isLinux = isLinux ? 0 : 1;
      break;
    case 7:
      printf("Saindo do programa...\n");
      return 0;
    default:
      printf("Opção inválida. Tente novamente.\n");
    }
  }
  return 0;
}
