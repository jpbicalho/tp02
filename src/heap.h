#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include "arquivo.h"


typedef struct{
    tItem item;
    int marcador;
}tRegistro;

typedef struct {
    tRegistro* array;
    int tamanho;
    int capacidade;
} Heap;

Heap* criarHeap(int capacidade);

void desalocaHeap(Heap* heap);

void trocar(tRegistro* a, tRegistro* b) ;

void minHeapify(Heap* heap, int indice,int*);

tRegistro extrairMinimo(Heap* heap,int*);

void inserir(Heap* heap, tRegistro elemento,int*);

int marcaRegistro(tRegistro,tRegistro);

void imprimirHeap(Heap* heap);

void desmarcaHeap(Heap* heap);


#endif
