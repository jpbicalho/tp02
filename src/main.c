#include <stdio.h>
#include "intercalacao.h"
#include "arquivo.h"
#include "heap.h"
#include <time.h>
int main()
{
    // variaveis para calcular o tempo
    clock_t inicio, fim;
    inicio = clock();

    // variavei para calcular a contagem
    int numLeitura = 0, numEscrita = 0, numComparacao = 0;
    
    // nome do arquivo que vai ser lido e quantidade de registros
    int numReg = 20;
    int i=0;
    char nomeBase[]="PROVAO.TXT";

    printf("\nCriacao das fitas para intercalacao...");
    criaFitas();

    printf("\n\n... Fase de selecao por substituicao ... ");
    selecao_por_substituicao(&numLeitura, &numEscrita, &numComparacao,nomeBase,numReg);

    printf("\n\n... Fase de intercalacao ... ");
    do
    {
        intercalacao_Balanceada_Fitas_entrada(&numLeitura, &numEscrita, &numComparacao);
        intercalacao_Balanceada_Fitas_Saida(&numLeitura, &numEscrita, &numComparacao);
        i++;
    } while (confere_Intercalacao() != numReg);

    fim = clock();
    printf("\nnum de repeticoes: %d", i);
    printf("\n\nTempo de execucao: %4f segundos\n\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
    printf("\nNumero de leitura: %d\n", numLeitura);
    printf("\nNumero de escrita: %d\n", numEscrita);
    printf("\nNumero de comparacoes: %d\n", numComparacao);
    printf("\n\n\n...Gerando registros ordenados...");
    
    gera_registros_ordenados();

    return 0;
}