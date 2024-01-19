#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>

typedef struct{
    long inscricao;
    double nota;
    char estado[3],cidade[51],curso[31];
}tItem;




void printa_Arquivo(tItem);

tItem leitor_de_registros(FILE*);

char* incrementa_Nome(char*);

void define_Nome_Fita_Saida(char[]);

void printa_Fita(int);

void printa_Todas_Fitas();

#endif
