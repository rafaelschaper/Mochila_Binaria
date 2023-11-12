#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "funcoes.h"

void main(){
    srand((unsigned) time(NULL));

    clock_t tempo; //Variável para armazenar o tempo.
    Mochila dados;

    abrirArquivo("teste2.txt", &dados);

    /* Solução melhor benefício */
    solucao_melhorBeneficio(dados);

    /* Solução menor peso */
    solucao_melhorPeso(dados);

    /* Solução melhor custo benefício */
    solucao_custoBeneficio(dados);

    liberacaoMemoria(&dados);
}