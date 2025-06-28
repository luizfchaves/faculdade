// https://github.com/luizfchaves/faculdade/tree/master/2-semestre/algoritmos
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Por algum motivo, o pc do Gabriel só reconheu os acentos com isso aqui
// (Aqui e na main)
#ifdef _WIN32
#include <windows.h>
#endif

/**
  Alunos:
  - Luiz Fernando de Souza Chaves - 1240209236
  - Gabriel Lopes Rocha - 1240206639
  - Matheus Rodrigues Medeiros - 1240210236
  - João Vitor da Cruz Miraglia - 1240206056
*/

char FILENAME[11] = "imoveis.txt";

int isLinux = 0;
typedef struct {
  char codigo[50];

  float valor;
  int tamanhoM2;

  // Apesar do UF ser 2 caracteres, precisei deixar 3 para o
  // "final" da string, estava printando infinito com [2]
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
  int funcionou = system("cls");
  // Tentando ser espertinho:
  // system retorna 0 se foi com sucesso e diferente de 0 se foi com erro, então
  // se cls não funcionou, provavelmente vai funcionar com o clear
  if (funcionou != 0) {
    isLinux = 1;
  }

  if (isLinux) {
    system("clear");
  } else {
    system("cls");
  }
}
void pause_terminal() {
  // o scanf deixa um \n no buffer, então preciso utilizar um getchar a mais
  // para remover o \n
  getchar();

  printf("Pressione Enter para continuar...");
  getchar();
}

int pegaNumeroOpcao(int opcaoMaxima) {
  // Pega opção do usuário de maneira segura para não causar loop infinito

  int input = -1;
  int isOpcaoValida = 0;
  do {
    isOpcaoValida = scanf(" %d", &input);
    if (!isOpcaoValida) {
      // https://stackoverflow.com/questions/1716013/why-is-scanf-causing-infinite-loop-in-this-code/12425529
      // mais especificamente: https://stackoverflow.com/a/41152924
      // Basicamente, caso o usuário digite algo que não seja um número,
      // o scanf deixa o que foi digitado no buffer

      scanf("%*s"); // isso limpa o buffer
      printf("Entrada inválida. Por favor, insira um número: ");
      continue;
    }
    if (input > opcaoMaxima || input <= 0) {
      isOpcaoValida = 0;
    }

    if (!isOpcaoValida) {
      printf("Opção inválida. Por favor, insira uma opção válida: ");
    }
  } while (!isOpcaoValida);

  return input;
}

void salva_no_arquivo(imovelStruct imovel) {
  FILE *file = fopen(FILENAME, "a");
  if (file == NULL) {
    printf("Erro ao abrir arquivo");
    exit(1);
  }

  fwrite(&imovel, sizeof(imovelStruct), 1, file);
  fclose(file);
}

void lista_imoveis() {
  limpa_terminal();
  FILE *file = fopen(FILENAME, "r");
  if (file == NULL) {
    printf("Nenhum imóvel cadastrado.\n");
    pause_terminal();
    return;
  }
  // Como não sei quantos imóveis serão cadastrados, vou utilizar uma lista
  // encadeada
  typedef struct listaEncadeadStruct {
    imovelStruct imovel;
    struct listaEncadeadStruct *proximo;
  } listaEncadeadaStruct;

  listaEncadeadaStruct *cabeca = NULL;
  listaEncadeadaStruct *atual = cabeca;

  // Colocando todo o arquivo em memória
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
  fclose(file);

  if (cabeca == NULL) {
    printf("Nenhum imóvel cadastrado.\n");
    pause_terminal();
    return;
  }

  // Ordenando a lista encadeada por valor
  // Atual é o item atual que está sendo verificado
  // A função percorre a lista e encontra o menor valor nos próximos
  // Depois troca o valor do item atual com o menor valor encontrado
  // Repete isso até que todos os itens estejam ordenados
  atual = cabeca;
  while (atual != NULL) {
    listaEncadeadaStruct *valorMenor = atual;

    listaEncadeadaStruct *listaTemp = atual->proximo;
    while (listaTemp != NULL) {
      if (listaTemp->imovel.valor < valorMenor->imovel.valor) {
        valorMenor = listaTemp;
      }
      listaTemp = listaTemp->proximo;
    }

    imovelStruct tempImovel = atual->imovel;
    atual->imovel = valorMenor->imovel;
    valorMenor->imovel = tempImovel;

    atual = atual->proximo;
  }

  printf("Lista de Imóveis ordenada por valor:\n");
  atual = cabeca;
  while (atual != NULL) {
    print_imovel(atual->imovel);
    printf("\n");

    listaEncadeadaStruct *temp = atual;
    atual = atual->proximo;
    free(temp);
  }

  pause_terminal();
}

void altera_no_arquivo(imovelStruct imovelAlterado) {
  FILE *file = fopen(FILENAME, "r+");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    exit(1);
  }
  FILE *tempFile = fopen("temp.txt", "w");
  if (tempFile == NULL) {
    printf("Erro ao criar arquivo temporário.\n");
    fclose(file);
    exit(1);
  }

  // Todos os imoveis que tem código diferente do código vão direto pro temp
  // o igual vai o alterado pro tempo ao inves do inicial
  imovelStruct imovelOriginal;
  while (fread(&imovelOriginal, sizeof(imovelStruct), 1, file)) {

    if (strcmp(imovelOriginal.codigo, imovelAlterado.codigo) == 0) {
      fwrite(&imovelAlterado, sizeof(imovelStruct), 1, tempFile);
    } else {
      fwrite(&imovelOriginal, sizeof(imovelStruct), 1, tempFile);
    }
  }

  fclose(file);
  fclose(tempFile);

  if (remove(FILENAME) != 0) {
    printf("Erro ao remover o arquivo original.\n");
    exit(1);
  }

  if (rename("temp.txt", FILENAME) != 0) {
    printf("Erro ao renomear o arquivo temporário.\n");
    exit(1);
  }
}

void remove_no_arquivo(char *codigo) {
  FILE *file = fopen(FILENAME, "r+");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    exit(1);
  }
  FILE *tempFile = fopen("temp.txt", "w");
  if (tempFile == NULL) {
    printf("Erro ao criar arquivo temporário.\n");
    fclose(file);
    exit(1);
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
    exit(1);
  }

  if (rename("temp.txt", FILENAME) != 0) {
    printf("Erro ao renomear o arquivo temporário.\n");
    exit(1);
  }
}

imovelStruct procura_imovel_por_codigo(char *codigo) {

  // Inicializando para comparar depois
  imovelStruct imovelFound;
  strcpy(imovelFound.codigo, "");
  imovelFound.valor = 0.0;
  imovelFound.tamanhoM2 = 0;
  strcpy(imovelFound.estadoUF, "");
  strcpy(imovelFound.cidade, "");

  FILE *file = fopen(FILENAME, "r");
  if (file == NULL) {
    return imovelFound; // Retorna vazio se o arquivo não existir
  }

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
      printf("O que deseja fazer?\n");
      printf("1. Cancelar\n");
      printf("2. Cadastrar outro código\n");

      int opcao = pegaNumeroOpcao(2);
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
      printf("Imóvel com código %s não encontrado.\n", imovel.codigo);
      printf("O que deseja fazer?\n");
      printf("1. Cancelar\n");
      printf("2. Buscar outro código\n");

      int opcao = pegaNumeroOpcao(2);
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

    altera_no_arquivo(imovel);
    break;
  } while (1);

  printf("Imóvel alterado com sucesso!\n");
  pause_terminal();
}

void remove_imovel() {
  limpa_terminal();
  char codigo[50];
  printf("Exclusão de Imóvel\n");

  do {
    printf("Digite o código do imóvel a ser excluído: ");
    scanf(" %[^\n]", codigo);

    imovelStruct imovel;
    imovel = procura_imovel_por_codigo(codigo);

    if (strcmp(imovel.codigo, "") == 0) {
      printf("Imóvel com código %s não encontrado.\n", imovel.codigo);
      printf("O que deseja fazer?\n");
      printf("1. Cancelar\n");
      printf("2. Buscar outro código\n");

      int opcao = pegaNumeroOpcao(2);
      if (opcao == 1) {
        printf("Remoção cancelada.\n");
        pause_terminal();
        return;
      }
      continue;
    }

    printf("Imóvel encontrado:\n");
    print_imovel(imovel);

    printf("Tem certeza que deseja excluir este imóvel?\n");
    printf("1. Sim\n");
    printf("2. Não\n");

    int confirmacao = pegaNumeroOpcao(2);
    if (confirmacao == 2) {
      printf("Remoção cancelada.\n");
      pause_terminal();
      return;
    }

    remove_no_arquivo(codigo);
    break;
  } while (1);

  printf("Imóvel excluído com sucesso!\n");
  pause_terminal();
}

void consulta_imoveis() {
  limpa_terminal();
  char codigo[50];
  printf("Consulta de Imóvel\n");
  printf("Digite o código do imóvel que deseja consultar: ");
  scanf(" %[^\n]", codigo);

  FILE *file = fopen(FILENAME, "r");
  if (file == NULL) {
    printf("Nenhum imóvel encontrado com o código: %s\n", codigo);
    pause_terminal();
    return;
  }

  imovelStruct imovel;
  int encontrado = 0;
  while (fread(&imovel, sizeof(imovelStruct), 1, file)) {
    if (strcmp(imovel.codigo, codigo) == 0) {
      print_imovel(imovel);
      encontrado = 1;
    }
  }
  fclose(file);

  if (!encontrado) {
    printf("Nenhum imóvel encontrado com o código: %s\n", codigo);
  }
  pause_terminal();
}

int main() {
  setlocale(LC_ALL, "pt_BR.UTF-8");

#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif

  while (1) {
    limpa_terminal();
    printf("\nMenu:\n");
    printf("1. Cadastrar Imóvel\n");
    printf("2. Lista Imóveis\n");
    printf("3. Consultar Imóvel\n");
    printf("4. Alterar Imóvel\n");
    printf("5. Excluir Imóvel\n");
    printf("6. Sair\n");
    printf("Escolha uma opção: ");

    int opcao = pegaNumeroOpcao(6);

    switch (opcao) {
    case 1:
      cadastra_imovel();
      break;
    case 2:
      lista_imoveis();
      break;
    case 3:
      consulta_imoveis();
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
    }
  }
  return 0;
}
