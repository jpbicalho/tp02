#include <stdio.h>
#include "intercalacao.h"
#include "arquivo.h"
#include "heap.h"
int main(){
/*
    printf("\nCriando as fitas...");
    criaFitas();
    
    printf("\n--------------------------------------------\n\n\n\n");
    
    printf("\nCriacao dos Blocos Ordenados:\n");
    cria_Blocos_Ordenados();
    printa_Todas_Fitas();
    
    printf("\n--------------------------------------------\n\n\n\n");
    printf("\nInicio da intercalacao:\n");
    do{
        intercalacao_fitas_entrada();
        intercalacao_fitas_saida();
    }while(validador()!=1);
    

    printf("\n\n\tOrdenacao concluida... :\n");
    printa_Todas_Fitas();
    */
   criaFitas();
   selecao_por_substituicao();
   return 0;
}