#include <stdio.h>
#include <stdlib.h>
#include "intercalacao.h"
#include "heap.h"

void criaFitas(){
    FILE *arq;
    char nome[] = "bin/fita00.bin";

    for(int i=0; i < NUM_FITAS; i++){
        
        nome[8] = i/10 + '0';
        nome[9] = i%10 + '0';

        arq = fopen(nome,"wb");

        if(arq==NULL){
            printf("Erro ao criar fita %d\n",i);
            return;
        }
        fclose(arq);
    }
}

void cria_Blocos_Ordenados(){
    ///criacao dos blocos ordenados para começar a intercalação
    
    /// simulação da memória interna
    tItem registros[TAM_MEM_INTERNA];

    /// ponteiro para abertura do arquivo onde armazena os dados
    FILE* arquivoProvao = fopen("PROVAO.TXT","r");
    if(arquivoProvao == NULL) printf("\nFalha ao abrir o arquivo\n");
    
    FILE* arquivoFita;

    char nomeDasFitas[] = "bin/fita00.bin";
    int totalReg = 0;

    
    for(int k = 0; k < (NUM_REGISTROS/TAM_MEM_INTERNA)+1; k++){
        
        //Recebe os registros do arquivo de acordo com o tamanho da memoria interna
        for (int i = 0; i < TAM_MEM_INTERNA ; i++)
        {
            if(feof(arquivoProvao)){
                //printf("\nFim do arquivo.");
                continue;
            }
            if(totalReg == NUM_REGISTROS){
                //printf("\nAtingiu o total dos n regs ordenados.");
                break;
            }
            //printf("\tle reg ");
            registros[i] = leitor_de_registros(arquivoProvao);
            
            totalReg++;

        }

        size_t tamanhoVetor = sizeof(registros)/sizeof(registros[0]);
        
        if((k+1)*TAM_MEM_INTERNA > NUM_REGISTROS)
            tamanhoVetor = NUM_REGISTROS % TAM_MEM_INTERNA; 

        //Ordenação do Vetor
        for(size_t i=0;i<tamanhoVetor-1;i++){
           
            for(int j=i+1;j<TAM_MEM_INTERNA;j++){
                
                if(registros[i].nota > registros[j].nota){
                    tItem aux = registros[i];
                    registros[i] = registros[j];
                    registros[j] = aux;
                }
            }
        }

        
        int tamanhoVetorInt;
        tamanhoVetorInt = (int)tamanhoVetor;
        printf("\ntamanho vet:%d",tamanhoVetorInt);
        
        //abertura da fita para salvar o tamanho do bloco ordenado 
        arquivoFita = fopen(nomeDasFitas,"ab");
        printf("\nFita %s aberta\n",nomeDasFitas);
        fwrite(&tamanhoVetorInt,sizeof(int),1,arquivoFita);
        
        
        printf("\n\nCriando bloco ordenado: %s",nomeDasFitas);

        //salvo cada registro da memoria externa ordenado
        for(int i=0; i < tamanhoVetorInt; i++){
            fwrite(&registros[i],sizeof(tItem),1,arquivoFita);
            printf("\n%.2lf",registros[i].nota);
        }
        incrementa_Nome(nomeDasFitas);
        if( (int)(k+1) % (int)(NUM_FITAS/2) == 0 ){
            nomeDasFitas[9] = '0';
        }
    
        fclose(arquivoFita);
    }

    fclose(arquivoProvao);
}

int somatorioVetor(int vetor[],int n){
    int somatorio = 0;
    for (int i = 0; i < n; i++)
    {
        somatorio = somatorio + vetor[i];
    }
    return somatorio;
}


void intercalacao_fitas_entrada(){
    char nomeArquivo[] = "bin/fita00.bin";
    char nomeArquivoSaida[] = "bin/fita00.bin";
    define_Nome_Fita_Saida(nomeArquivoSaida); 

    //calculo do nome de arquivo saida
    
    FILE *arquivosEntrada[NUM_FITAS/2];
    FILE *arquivosSaida[NUM_FITAS/2];

    int tamanhoDoBloco[NUM_FITAS/2],quantidadeTotal=0;
    

    //printf("\n\n\n\n");


    //ABRE OS ARQUIVOS DE ENTRADA NO MODO LEITURA
    for(int i = 0; i < NUM_FITAS/2; i++){
        arquivosEntrada[i] = fopen(nomeArquivo,"rb");
        incrementa_Nome(nomeArquivo); 
        
        if(arquivosEntrada[i] == NULL){
            printf("\nabertura de arquivo falhou!");
            return;
        }
    }

    //ABRE OS ARQUIVOS DE SAIDA NO MODO SOBRESCRITA
    for (int i = 0; i < NUM_FITAS/2; i++){
        //Zera o arquivo
        arquivosSaida[i] = fopen(nomeArquivoSaida,"wb");
        fclose(arquivosSaida[i]);

        //abre o arquivo 
        arquivosSaida[i] = fopen(nomeArquivoSaida,"ab");
        //printf("\nabriu o arquivo: %s", nomeArquivoSaida);
        if(arquivosSaida[i] == NULL)
           printf("\nfalha na abertura do arquivo: %s\n",nomeArquivoSaida);
        incrementa_Nome(nomeArquivoSaida); 
    }
    
    int a = 0,tamanhoInicial=0;

    //for(int temp = 0;temp < 1; temp++){
    do{
        
        //RECEBE O TAMANHO DE CADA BLOCO
        for(int i = 0; i < NUM_FITAS/2; i++){
            tamanhoDoBloco[i] = 0;
            fread(&tamanhoDoBloco[i],sizeof(int),1,arquivosEntrada[i]);
            printf("\t%d",tamanhoDoBloco[i]);
            incrementa_Nome(nomeArquivo);
            quantidadeTotal += tamanhoDoBloco[i];   
        }
        tamanhoInicial=somatorioVetor(tamanhoDoBloco,NUM_FITAS/2);
        
        
        //DECLARA A QUANTIDADE DE ITENS NO BLOCO
        //printf("\nprinta na fita %d",a%(NUM_FITAS/2));
        fwrite(&quantidadeTotal,sizeof(int),1,arquivosSaida[a%(NUM_FITAS/2)]);
        
        //CRIO MINHA "MEMORIA INTERNA"
        tItem registros[NUM_FITAS/2];
        nomeArquivo[9] = '0';
        nomeArquivo[8] = '0';


        //PEGANDO UM DE CADA FITA
        for(int i=0; i<NUM_FITAS/2; i++){
            printf("\nnome do arquivo: %s", nomeArquivo);
            if(tamanhoDoBloco[i] == 0)
                registros[i].nota = 0;
            else fread(&registros[i],sizeof(tItem),1,arquivosEntrada[i]);
            printf("  %f  ",registros[i].nota);
            incrementa_Nome(nomeArquivo);
            
        }


        //ALGORITMO DE INTERCALAÇÃO E ORDENAÇÃO DOS BLOCOS
        while(quantidadeTotal != 0){  
              
                //recebo um elemento de cada,altero o tamanho do bloco atual
                //auxiliares para poder atuar na repeticao
                tItem aux;
                int marcador = 0;
                aux = registros[0];
                for(int i=0; i<NUM_FITAS/2;i++){
                    printf("\nComparacoes:\t%f",registros[i].nota);
                    if(tamanhoDoBloco[i] <= 0) printf("(nao pode ser escolhido)");
                }
                printf("\n");

                for (int i = 0; i < NUM_FITAS/2; i++)
                {
                    
                   if(tamanhoDoBloco[i] <= 0){
                        //printf("\nO %d nao pode ser escolhido...\n",i);
                        continue;;
                   }else{
                        aux = registros[i];
                        marcador = i;
                        //printf("%lf foi atribuido a variavel aux\n",aux.nota);
                        break;
                   }
                }
                

                //escolhe o menor, em que o tamanho de seu bloco nao seja 0
                for(int i=0; i<NUM_FITAS/2;i++){
                    //printf("%lf < %lf e %d\n",registros[i].nota,aux.nota,tamanhoDoBloco[i]);
                    if(registros[i].nota < aux.nota && tamanhoDoBloco[i] > 0){
                        //printf("Ordenacao escolheu o do bloco %d", i);
                        aux = registros[i];
                        marcador = i;
                    }
                }

                //atualizando as quantidades de cada bloco
                incrementa_Nome(nomeArquivo);
                //printf("\nnome do arquivo: %s",nomeArquivo);
                //printf("\nNumero selecionado: %f",registros[marcador].nota);
                //printf("\nmarcador: %d", marcador);
                //printf("\ntamanho do bloco acessado:%d", tamanhoDoBloco[marcador]);
                //printf("\tquantidade total: %d", quantidadeTotal);
                //printf("\n");

                
                fwrite(&registros[marcador],sizeof(tItem),1,arquivosSaida[a % (NUM_FITAS/2)]);
                tamanhoDoBloco[marcador]--;
                if(tamanhoDoBloco[marcador] != 0){
                    fread(&registros[marcador],sizeof(tItem),1,arquivosEntrada[marcador]);
                    //printf("\nNovo valor para comparar: %f", registros[marcador].nota);
                    
                }
                //printf("\n------------------------------------------------\n");

                
                quantidadeTotal--;
        }
        a++;
    }while(tamanhoInicial>0);
    
    
    
    for(int i = 0; i < NUM_FITAS/2; i++){
        tamanhoDoBloco[i] = 0;
        fread(&tamanhoDoBloco[i],sizeof(int),1,arquivosEntrada[i]);
        //printf("\n\n\nTamanho do bloco fita %d: %d",i,tamanhoDoBloco[i]);
        incrementa_Nome(nomeArquivo);
        quantidadeTotal += tamanhoDoBloco[i];   
    }
    
    for(int i=0;i<NUM_FITAS/2;i++){
        fclose(arquivosEntrada[i]);
        fclose(arquivosSaida[i]);
    }
}

void intercalacao_fitas_saida(){
    //PONTEIROS DE ARQUIVOS DE FITAS QUE SERÃO UTILIZADOS
    FILE *arquivosEntradas[NUM_FITAS/2];
    FILE *arquivosSaidas[NUM_FITAS/2];
    
    //NOMES PARA ABERTURA DO ARQUIVO
    char nomeArquivoEntrada[]="bin/fita00.bin";
    
    char nomeArquivoSaida[] = "bin/fita00.bin";
    define_Nome_Fita_Saida(nomeArquivoSaida);
   

    //ABERTURA DOS ARQUIVOS DE FITA DE ENTRADA E SAIDA
    for (int i = 0; i < NUM_FITAS/2; i++)
    {
        arquivosEntradas[i] = fopen(nomeArquivoEntrada,"wb");
        if(arquivosEntradas[i] == NULL) printf("\n\nerro na abertura do arquivo: %s", nomeArquivoEntrada);
        incrementa_Nome(nomeArquivoEntrada);

        arquivosSaidas[i] = fopen(nomeArquivoSaida,"rb");
        if(arquivosSaidas[i] == NULL) printf("\n\nerro na abertura do arquivo: %s", nomeArquivoSaida);
        //printf("%s (aberto)", nomeArquivoSaida);
        incrementa_Nome(nomeArquivoSaida);

    }

    int a = 0,tamanhoInicial=0;
    int tamanhoDoBloco[NUM_FITAS/2], quantidadeTotal = 0;
    nomeArquivoSaida[9] = '0' + (NUM_FITAS/2);
    //for(int rep = 0;rep<3;rep++){
    do{
        //RECEBE A QUANTIDADE DE CADA ITEM DO BLOCO INICIAL
        for(int i = 0; i < NUM_FITAS/2; i++){
            tamanhoDoBloco[i] = 0;
            fread(&tamanhoDoBloco[i] , sizeof(int) , 1 ,arquivosSaidas[i]);
            //printf("\nTamanho do bloco lido(%s): %d",nomeArquivoSaida, tamanhoDoBloco[i]);
            incrementa_Nome(nomeArquivoSaida);
            quantidadeTotal += tamanhoDoBloco[i];
        }

        tamanhoInicial = quantidadeTotal;
        //printf("\n\nPRINTA NO ARQUIVO: %s\n",nomeArquivoEntrada);
        //salvo o tamanho do bloco ja nas fitas de entrada
        //printf("%d", quantidadeTotal);
        fwrite(&quantidadeTotal,sizeof(int),1,arquivosEntradas[a%(NUM_FITAS/2)]);
        
        tItem registros[NUM_FITAS/2];
        nomeArquivoSaida[9] = '0' + NUM_FITAS/2;


        //LEIO MINHA PRIMEIRA LEVA DE ITENS
        for(int i=0; i<NUM_FITAS/2; i++){
          //  printf("\nnome do arquivo: %s", nomeArquivoSaida);
            if(tamanhoDoBloco[i] == 0)
                registros[i].nota = 0;
            else fread(&registros[i],sizeof(tItem),1,arquivosSaidas[i]);
            //printf("  %f  ",registros[i].nota);
            incrementa_Nome(nomeArquivoSaida);
        }

        while(quantidadeTotal != 0){  
              
                //recebo um elemento de cada,altero o tamanho do bloco atual
                //auxiliares para poder atuar na repeticao
                tItem aux;
                int marcador = 0;
                aux = registros[0];
/*
                for(int i=0; i<NUM_FITAS_SAIDA;i++){
                  printf("\nComparacoes:\t%f",registros[i].preco);
                 if(tamanhoDoBloco[i] < 0) printf("(nao pode ser escolhido)");
                }
                printf("\n");
*/
                for (int i = 0; i < NUM_FITAS/2; i++)
                {
                    
                   if(tamanhoDoBloco[i] <= 0){
                        //printf("\nO %d nao pode ser escolhido...\n",i);
                        continue;;
                   }else{
                        aux = registros[i];
                        marcador = i;
                        //printf("%lf foi atribuido a variavel aux\n",aux.nota);
                        break;
                   }
                }
  
  
                //escolhe o menor, em que o tamanho de seu bloco nao seja 0
                for(int i=0; i<NUM_FITAS/2;i++){
                    //printf("%lf < %lf e %d\n",registros[i].nota,aux.nota,tamanhoDoBloco[i]);
                    if(registros[i].nota < aux.nota && tamanhoDoBloco[i] > 0){
                      //  printf("Ordenacao escolheu o do bloco %d", i);
                        aux = registros[i];
                        marcador = i;
                    }
                }

                //atualizando as quantidades de cada bloco
                nomeArquivoSaida[9] = marcador + '0' + (NUM_FITAS/2);


                
                fwrite(&registros[marcador],sizeof(tItem),1,arquivosEntradas[a % (NUM_FITAS/2)]);
                tamanhoDoBloco[marcador]--;
                if(tamanhoDoBloco[marcador] != 0){
                    fread(&registros[marcador],sizeof(tItem),1,arquivosSaidas[marcador]);
                }

                
                quantidadeTotal--;
                //printf("condicao do while: %d\n", quantidadeTotal);
        }


        //printf("condicao do do-while: %d",tamanhoInicial);
        incrementa_Nome(nomeArquivoSaida);
    }while(tamanhoInicial>0);
    //}
    for(int i=0;i<NUM_FITAS/2;i++){
        fclose(arquivosEntradas[i]);
        fclose(arquivosSaidas[i]);
    }
    

}


int validador(){

    char nomeArquivo[]="bin/fita00.bin";
    nomeArquivo[9] = '0' + (NUM_FITAS/2);
    FILE* arquivo = fopen(nomeArquivo,"rb");
    if(arquivo == NULL) return 0;
    
    int quantidade;
    fread(&quantidade,sizeof(int),1,arquivo);
    if(quantidade == NUM_REGISTROS){
        printf("Quantidade: %d", quantidade);
        return 1;
    }else return 0;
}



void selecao_por_substituicao(){
    
    Heap *heap;
    heap = criarHeap(TAM_MEM_INTERNA);
    
    //heap construido, agora recebe os registros e joga pro heap
    FILE *arquivoProvao = fopen("PROVAO.TXT","r");
    char nomeFitaEntrada[] = "bin/fita00.bin";
    FILE *arquivoFita = fopen(nomeFitaEntrada,"ab");
    
    if(arquivoProvao == NULL) printf("\nFalha ao abrir o arquivo\n");
    if(arquivoFita == NULL) printf("\nFalha ao abrir o arquivo\n");
    

    int  contadorRegistros=0;
    //recebe os itens do provao, adiciona no registro com o marcador 0 e insere no heap
    for(int i = 0; i < TAM_MEM_INTERNA; i++){
        tItem itemAux;
        tRegistro regAux;
        
        itemAux = leitor_de_registros(arquivoProvao);
        contadorRegistros++;
        regAux.item = itemAux;
        regAux.marcador = 0;
        inserir(heap,regAux);
        

    }

    int contaItensMarcados = 0;
    tRegistro novoReg,minReg;


    do{
        do{
            //comparando o menor do heap com o novo registro para saber se o novo sera marcado
            minReg = extrairMinimo(heap);
            printf("\nAdicionado: %.2lf - %s",minReg.item.nota,nomeFitaEntrada);
            fwrite(&minReg.item,sizeof(tItem),1,arquivoFita);
            novoReg.item = leitor_de_registros(arquivoProvao);
            contadorRegistros++;
            
            if(minReg.item.nota > novoReg.item.nota){
                novoReg.marcador = 1;
                contaItensMarcados++;
            }else novoReg.marcador = 0;

            inserir(heap,novoReg);
        }while(contaItensMarcados < TAM_MEM_INTERNA || minReg.marcador == 1);
        contaItensMarcados = 0;
        
        //Desmarco o Heap
        desmarcaHeap(heap);
        
        
        //printa um marcador de fim de bloco, onde a inscricao eh -1
        tItem marcaFim = {-1,0,"","",""};
        fwrite(&marcaFim,sizeof(tItem),1,arquivoFita);
        
        //fecha a fita e abre a proxima
        fclose(arquivoFita);
        if(nomeFitaEntrada[9] == '0' + ((NUM_FITAS/2)-1)){
            nomeFitaEntrada[9] = '0';
        }else incrementa_Nome(nomeFitaEntrada);
        fopen(nomeFitaEntrada,"ab");

        printf("\n\n\n");
    }while(contadorRegistros < NUM_REGISTROS);
    fclose(arquivoProvao);
    fclose(arquivoFita);
    desalocaHeap(heap);

}

int calculaQuantidadeBlocos(FILE* arquivo){
    int quantidade = 0;
    tItem itemAux;
    while(fread(&itemAux,sizeof(tItem),1,arquivo) != 0){
        if(itemAux.nota == -1) break;
        quantidade++;
    }
    return quantidade;
    
}