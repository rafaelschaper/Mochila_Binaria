#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
    int n;
    int limiteMochila;
    int *peso;
    int *beneficio;
}Mochila;

void abrirArquivo(char *path, Mochila *dados);

void imprimirDados(Mochila dados);

void alocacaoMemoria(Mochila *dados);

void liberacaoMemoria(Mochila *dados);

int pesoTotal(Mochila dados);

void CalculaSolucao_melhorPeso(Mochila dados, int *bin);

void relacao_pesoBen(Mochila dados, float *vetor);

void calcula_custoBeneficio(Mochila dados, int *solucao);

void calcula_melhorBeneficio(Mochila dados, int *bin);

int pesoSolucao(int *solucao, Mochila dados);

int beneficioSolucao(int *solucao, Mochila dados);

void copiaVetor(int n, int *destino, int *origem);

int imprimeVetorSolucao(int n, int *solucao);

void solucao_melhorBeneficio(Mochila dados);

void solucao_melhorPeso(Mochila dados);

void solucao_custoBeneficio(Mochila dados);
