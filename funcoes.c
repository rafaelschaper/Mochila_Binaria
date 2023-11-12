#include "funcoes.h"

void alocacaoMemoria(Mochila *dados){
    dados -> peso = (int*) malloc(dados -> n*sizeof(int));
    dados -> beneficio = (int*) malloc(dados -> n*sizeof(int));

    if (dados -> peso == NULL){
        printf("Malloc = NULL");
        exit(EXIT_FAILURE);
    }
    
    if (dados -> beneficio == NULL){
        printf("Malloc = NULL");
        exit(EXIT_FAILURE);
    }
}

void liberacaoMemoria(Mochila *dados){
    free(dados -> peso);
    free(dados -> beneficio);
}

void imprimirDados(Mochila dados){
    printf("Capacidade da mochila: %d\n", dados.limiteMochila);
    printf("Total de itens: %d\n", dados.n);

    printf("Peso dos itens: ");
    for (int i = 0; i < dados.n; i++)
        printf("%d ", dados.peso[i]);

    printf("\nBeneficio dos itens: ");
    for (int i = 0; i < dados.n; i++)
        printf("%d ", dados.beneficio[i]);
        
    printf("\n");
    
}

void abrirArquivo(char *path, Mochila *dados){
    int valor;

    FILE *arquivo;
    arquivo = fopen(path, "r");

    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo!\n");
        exit(EXIT_FAILURE);
    }
   
    fscanf(arquivo, "%d", &valor);
    dados -> limiteMochila = valor;

    fscanf(arquivo, "%d", &valor);
    dados -> n = valor;

    alocacaoMemoria(dados);

    for(int i = 0; i < dados -> n; i++){
        fscanf(arquivo, "%d", &valor);
        dados -> peso[i] = valor;
    }

    for(int i = 0; i < dados -> n; i++){
        fscanf(arquivo, "%d", &valor);
        dados -> beneficio[i] = valor;
    }

    fclose(arquivo);
}

int imprimeVetorSolucao(int n, int *solucao){
    int v;

    printf("Vetor solucao: [");

    for (v = 0; v < n; ++v)
        printf("%d", solucao[v]);
    
    printf("]\t");

    return v;
}

// Função que soma o peso de todos os itens disponíveis para adicionar na mochila.
int pesoTotal(Mochila dados){
    int i;
    int somaVetor = 0;
    for(i = 0; i < dados.n; i++)
        somaVetor += dados.peso[i];
    return somaVetor;
}

// Função que cria o vetor binário que indica quais itens serão colocados na mochila
void CalculaSolucao_melhorPeso(Mochila dados, int *bin){
    int pTotal = pesoTotal(dados);
    int maior = 0;
    int *aux; // Vetor auxiliar para guardar o valor peso dos itens que não entrarão na mochila
    aux = (int*) calloc(dados.n, sizeof(int));
    while(pTotal > dados.limiteMochila){
        pTotal = 0;
        maior = 0;

        // Realiza o calculo até o peso total dos itens disponíveis ser igual ou menor ao peso limite
        // da mochila
        for(int i=0; i < dados.n; i++)
            pTotal = pesoTotal(dados); 

        if (pTotal > dados.limiteMochila){
            for(int i=0; i < dados.n; i++)
                // Verifica qual o maior valor peso e o armazena no vetor auxiliar
                if (dados.peso[i] > maior){ 
                    maior = dados.peso[i];
                    aux[i] = maior;
                }

            for(int i=0; i < dados.n; i++)
                // O maior valor é substituido por 0, o que significa que não ficará na mochila
                if (dados.peso[i] == maior){
                    dados.peso[i] = 0;
                    break;
                }
        }
    }
    for(int i=0; i < dados.n; i++){ 
        // O vetor solução é criado, qualquer elemento != 0 é trocado por 1
        if (dados.peso[i] != 0) bin[i] = 1;
        // Os valores peso retirados são retornados para o vetor peso
        if (dados.peso[i] == 0) dados.peso[i] = aux[i]; 
    }

    free(aux);
}

// Calcula a relação dos itens a partir do peso e benefício de cada item e guarda em um vetor
void relacao_pesoBen(Mochila dados, float *vetor){
    float relacao;
    for (int i=0; i < dados.n; i++){
        relacao = ((float)dados.beneficio[i] / (float)dados.peso[i]);
        vetor[i] = relacao;
    }
}

// Função que cria o vetor binário que indica quais itens podem ser colocados na mochila
void calcula_custoBeneficio(Mochila dados, int *bin){
    int pTotal = 0;
    float maior;
    float *relacao; 
    
    relacao = (float*) calloc(dados.n, sizeof(float));

    relacao_pesoBen(dados, relacao);
    while (pTotal <= dados.limiteMochila){
        maior = 0;
        for (int i=0; i < dados.n; i++)
            // Armazena o maior valor da relação peso benefício na variável
            if (relacao[i] > maior) 
                maior = relacao[i]; 

        for (int i=0; i < dados.n; i++){
            // Caso o maior dado do vetor relação seja igual ao da variável, é incrementado o peso 
            // do item na variável peso total
            if (maior == relacao[i]){ 
                pTotal += dados.peso[i];
                // Caso o peso total seja menor que o peso limite da mochila, o valor 1 é acrescentado 
                // ao local referente ao peso do item, e seu custo benefício é retirado do vetor custo 
                // benefício
                if (pTotal < dados.limiteMochila){ 
                    bin[i] = 1;
                    relacao[i] = 0;
                }
            }
        } 
    }
}

// Função que cria o vetor binário que indica quais itens podem ser colocados na mochila
void calcula_melhorBeneficio(Mochila dados, int *bin){
    int somaPeso = 0, somaBeneficio, maior;
    int *auxBeneficio, *auxPeso; // Vetores criados para fazer mudanças dentro da função sem ser necessário alterar os vetores originais

    auxBeneficio = (int*) calloc(dados.n, sizeof(int)); 
    auxPeso = (int*) calloc(dados.n, sizeof(int)); 

    // Copia o vetor referente ao beneficio dos itens para o vetor auxiliar
    copiaVetor(dados.n, auxBeneficio, dados.beneficio); 
    // Copia o vetor referente ao peso dos itens para o vetor auxiliar
    copiaVetor(dados.n, auxPeso, dados.peso); 

    while (somaPeso <= dados.limiteMochila){
        somaBeneficio = 0;
        maior = 0;

        for (int i=0; i < dados.n; i++)
            if (auxBeneficio[i] > maior)
                maior = auxBeneficio[i]; // O maior valor do vetor benefício é armazenado na variável 

        for (int i=0; i < dados.n; i++) 
            // Se o beneficio do item for igual ao maior valor armazenado e o peso do item 
            //não ultrapasse o limite da mochila, o item é colocado na mochila, o peso total da mochila 
            //é incrementado e o valor benefício é alterado para 0
            if (auxBeneficio[i] == maior && (somaPeso + auxPeso[i]) <= dados.limiteMochila){ 
                bin[i] = 1;
                somaPeso += auxPeso[i];
                auxBeneficio[i] = 0;
            // Se não, o valor do peso e do benefício é alterado para 0
            } else if ((somaPeso + auxPeso[i]) > dados.limiteMochila){
                auxPeso[i] = 0;
                auxBeneficio[i] = 0;
                break;
            }
        
        // Somatório do benefício para controlar a estrutura de loop
        for (int i=0; i < dados.n; i++)
            somaBeneficio += auxBeneficio[i];
        
        // Condição de parada do loop, quando o somatório do vetor for igual a 0, significa que
        // não há mais itens para adicionar na mochila e encerra o loop
        if (somaPeso <= dados.limiteMochila && somaBeneficio == 0)
            break;
    }

    // Libera a memória do vetor auxiliar
    free(auxBeneficio); 
    free(auxPeso);
}

int pesoSolucao(int *solucao, Mochila dados){
    int i;
    int peso = 0;

    for (i = 0; i < dados.n; ++i)
        if (solucao[i] == 1)
            peso += dados.peso[i];
    return peso;
}

int beneficioSolucao(int *solucao, Mochila dados){
    int i;
    int capacidade = 0;

    for (i = 0; i < dados.n; ++i)
        if (solucao[i] == 1)
            capacidade += dados.beneficio[i];
    return capacidade;
}

void copiaVetor(int n, int *destino, int *origem){
    int i;

    for (i = 0; i < n; ++i)
        destino[i] = origem[i];
}

// Função responsável por gerar o resultado da heurística de melhor benefício
void solucao_melhorBeneficio(Mochila dados){
    clock_t tempo; // Variável para armazenar o tempo.

    int peso = 0;
    int beneficio = 0;
    int *solucao;

    solucao = (int*) calloc(dados.n, sizeof(int));

    tempo = clock(); // Armazena o tempo

    calcula_melhorBeneficio(dados, solucao);
    peso = pesoSolucao(solucao, dados);
    beneficio = beneficioSolucao(solucao, dados);

    tempo = clock() - tempo; // Tempo final - tempo inicial

    printf("\nMelhor Solucao Baseada em Melhor Beneficio\n");
    imprimeVetorSolucao(dados.n, solucao);
    printf("Peso: %d\t", peso);
    printf("Beneficio: %d\t", beneficio);
    printf("Tempo de execucao: %lf ms", ((double)tempo) / ((CLOCKS_PER_SEC / 1000)));

    free(solucao);
}

// Função responsável por gerar o resultado da heurística de menor peso
void solucao_melhorPeso(Mochila dados){
    clock_t tempo; // Variável para armazenar o tempo.

    int peso = 0;
    int beneficio = 0;
    int *solucao;

    solucao = (int*) calloc(dados.n, sizeof(int));

    tempo = clock(); // Armazena o tempo

    CalculaSolucao_melhorPeso(dados, solucao); 
    peso = pesoSolucao(solucao, dados);
    beneficio = beneficioSolucao(solucao, dados);

    tempo = clock() - tempo; // Tempo final - tempo inicial

    printf("\nMelhor Solucao Baseada em Menor Peso\n");
    imprimeVetorSolucao(dados.n, solucao);
    printf("Peso: %d\t", peso);
    printf("Beneficio: %d\t", beneficio);
    printf("Tempo de execucao: %lf ms", ((double)tempo) / ((CLOCKS_PER_SEC / 1000)));

    free(solucao);
}

// Função responsável por gerar o resultado da heurística de melhor custo benefício
void solucao_custoBeneficio(Mochila dados){
    clock_t tempo; // Variável para armazenar o tempo.

    int peso = 0;
    int beneficio = 0;
    int *solucao;

    solucao = (int*) calloc(dados.n, sizeof(int));

    tempo = clock(); // Armazena o tempo

    calcula_custoBeneficio(dados, solucao);
    peso = pesoSolucao(solucao, dados);
    beneficio = beneficioSolucao(solucao, dados);

    tempo = clock() - tempo; // Tempo final - tempo inicial

    printf("\nMelhor Solucao Baseada em Melhor Custo Beneficio\n");
    imprimeVetorSolucao(dados.n, solucao);
    printf("Peso: %d\t", peso);
    printf("Beneficio: %d\t", beneficio);
    printf("Tempo de execucao: %lf ms", ((double)tempo) / ((CLOCKS_PER_SEC / 1000)));

    free(solucao);
}