#ifndef INTERCALACAO_H
#define INTERCALACAO_H

#include "arquivo.h"
#include "stdlib.h"

/// @brief Numero de fitas que serão utilizadas
#define NUM_FITAS 20

#define TAM_MEM_INTERNA 10

#define NUM_REGISTROS 100
//criar enum de marcadores



/// @brief Criação das fitas para manipulação dos dados
void criaFitas();


/// criacao dos blocos ordenados por meio do metodo de selecao por substituicao
void selecao_por_substituicao();

int somatorioVetor(int vetor[],int n);

int validador();

int calculaQuantidadeBlocos(FILE*);

void intercalacao_Balanceada_Fitas_entrada();

void intercalacao_Balanceada_Fitas_Saida();

int confere_Intercalacao();

void imprime_conteudo_fita();

#endif