#ifndef INTERCALACAO_H
#define INTERCALACAO_H

#include "arquivo.h"
#include "stdlib.h"

/// @brief Numero de fitas que serão utilizadas
#define NUM_FITAS 6

#define TAM_MEM_INTERNA 3

#define NUM_REGISTROS 100
//criar enum de marcadores



/// @brief Criação das fitas para manipulação dos dados
void criaFitas();

/// criacao dos blocos ordenados
void cria_Blocos_Ordenados();

/// criacao dos blocos ordenados por meio do metodo de selecao por substituicao
void selecao_por_substituicao();

int somatorioVetor(int vetor[],int n);

void intercalacao_fitas_entrada();

void intercalacao_fitas_saida();

int validador();

int calculaQuantidadeBlocos(FILE*);


#endif