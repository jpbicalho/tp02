#include <stdio.h>
#include "intercalacao.h"
#include "arquivo.h"
#include "heap.h"
int main(){
    printf("\nCriacao das fitas para intercalacao...");
    criaFitas();
    printf("\n\n... Fase de selecao por substituicao ... ");
    selecao_por_substituicao();
    printf("\n\n... Fase de intercalacao ... ");
    do{
        intercalacao_Balanceada_Fitas_entrada();
        intercalacao_Balanceada_Fitas_Saida();
    }while(confere_Intercalacao() != NUM_REGISTROS);

    printf("\n\n\n...Gerando registros ordenados...");
    gera_registros_ordenados();

    return 0;
}