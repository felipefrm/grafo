#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <sys/resource.h>
#include "entradaSaida.h"

Arquivos* argumentosEntrada(int argc, char* argv[]){

  Arquivos* arq = malloc(sizeof(Arquivos));   // armazena os ponteiros dos arquivos
  int opcao;
  arq->flag = 1;                                  // passados por linha de comando em uma struct
  if (argc < 5){
    fprintf(stderr, "Use: ./tp3 -i [ARQUIVO DE ENTRADA DE DADOS] -o [ARQUIVO DE SAIDA]\n");
    arq->flag = 0;
    return arq;
  }
  while((opcao = getopt(argc, argv, "i:o:"))!= -1) { // recebe-se os argumentos por meio
                                                     // da funcao getopt()
    switch(opcao) {
      case 'i':
        if(!(arq->entrada = fopen(optarg, "r"))) {
          fprintf(stderr, "Erro na abertura do arquivo.\n");
          arq->flag = 0;
          return arq;
        }
        break;
      case 'o':
        if(!(arq->saida = fopen(optarg, "w"))) {
          fprintf(stderr, "Erro na abertura do arquivo.\n");
          arq->flag = 0;
          return arq;
        }
    }
  }
  return arq;
}

int** alocaMatriz(int linha, int coluna){

  int **matriz = (int**)calloc(sizeof(int*), linha);
  for (int i=0; i<linha; i++)
    matriz[i] = (int*)calloc(sizeof(int), coluna);
  return matriz;
}

int verificaArqVazio(FILE* arq){
  int tamanho_arq;
  fseek (arq, 0, SEEK_END);               // aponta para o fim do arquivo com fseek()
  if((tamanho_arq = ftell (arq)) == 0){   // retorna o valor da posição do ponteiro com ftell()
    fprintf(stderr, "O arquivo de entrada está vazio!\n");
    return 0;
  }
  rewind(arq);   // retorna o ponteiro para o inicio do arquivo, para os proximos
  return 1;      // procedimentos
}

int calculaQtdArestas(FILE* arq){
  // int qtdVertices;
  // fscanf(arq, "%d", qtdVertices);
  char c;
  int qtdNumeros=-1;
  while (!feof(arq)){
    c = fgetc(arq);
    if (c >= 48 && c <=57)
      qtdNumeros++;
  }
  rewind(arq);
  printf("%d\n", qtdNumeros);
  printf("Numero arestas: %d\n", qtdNumeros/2);
  return (int)(qtdNumeros/2);
}

int leQtdVertices(FILE *arq){
  int qtdVertices;
  fscanf(arq, "%d", &qtdVertices);
  fscanf(arq, "\n");

  return qtdVertices;
}


int** leArestas(FILE *arq, int qtdArestas){
  int **mat = alocaMatriz(qtdArestas, 2);
  char c;
  for (int i=0; i<qtdArestas; i++){
    fscanf(arq, "%d %d\n", &mat[i][0], &mat[i][1]);
  }
  return mat;
}

void contaTempoProcessador(double *utime, double *stime){
  struct rusage resources;
  getrusage(RUSAGE_SELF, &resources);
  *utime = (double) resources.ru_utime.tv_sec + 1.e-6 * (double) resources.ru_utime.tv_usec;
  *stime = (double) resources.ru_stime.tv_sec + 1.e-6 * (double) resources.ru_stime.tv_usec;
}

void imprimeTempo(double user_time, double system_time, FILE* arq){
  fprintf(arq, "Tempo de execução:\n");
  fprintf(arq, "%fs (tempo de usuário) + %fs (tempo de sistema) = %fs (tempo total)\n\n", user_time, system_time, user_time+system_time);
}

void liberaArquivos(Arquivos *arq){
  fclose(arq->entrada);
  fclose(arq->saida);
  free(arq);
}
