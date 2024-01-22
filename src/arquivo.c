#include "arquivo.h"
#include "intercalacao.h"

void printa_Arquivo(tItem reg){
    printf("\n");
    
    printf("Inscricao: %ld\t Nota:%lf",reg.inscricao,reg.nota);
    
    printf("\n");

    printf("%s - %s",reg.cidade,reg.estado);

    printf("\n");

    printf("Curso: %s",reg.curso);

    printf("\n");
}

tItem leitor_de_registros(FILE* arquivo){
    tItem registro;
    char backspace;

    //lê inscricao
    fscanf(arquivo, "%8ld", &registro.inscricao);
    
    //lê o espaco
    fscanf(arquivo, "%c", &backspace);
    
    //lê a nota
    fscanf(arquivo, "%5lf", &registro.nota);
    
    fscanf(arquivo, "%c", &backspace);
    
    fscanf(arquivo,"%2s", &registro.estado);
    
    fscanf(arquivo, "%c", &backspace);
    
    //fscanf(arquivo,"%50s", &registro.cidade);
    fgets(registro.cidade,50,arquivo);

    fscanf(arquivo, "%c", &backspace);

    //fscanf(arquivo, "%30s", &registro.curso);
    fgets(registro.curso,30,arquivo);
    
    //printa_Arquivo(registro);

    return registro;
}

char* incrementa_Nome(char* nome){
    int i = 0;
    i = nome[9] - '0' + 1;
    nome[8] = i/10 + '0';
    nome[9] = i%10 + '0';
    return nome;
}

void printa_Fita(int i){
    
    char nomeArquivo[] = "bin/fita00.bin";
    for(int k = 0; k< i; i++){
        incrementa_Nome(nomeArquivo);
    }
    printf("\nLendo: %s",nomeArquivo);
    FILE* arquivo = fopen(nomeArquivo,"rb");
    if(arquivo == NULL){
        printf ("\n\n\nFalha em abrir o arquivo");
        return;
    }
    tItem reg;
    int quantidade;

    fseek(arquivo, 0, SEEK_SET);
    printf("\nFITA: %s",nomeArquivo);
    while(!feof(arquivo)){
        quantidade = 0;
        fread(&quantidade,sizeof(int),1,arquivo);
        printf("\nTamanho do bloco:%d\n",quantidade);
        for(int k=0;k<quantidade;k++){
            reg.nota = 0;
            fread(&reg,sizeof(tItem),1,arquivo);
            //printa_Arquivo(reg);
            printf("\nNOTA:%lf",reg.nota);
        }
    }

    fclose(arquivo);
}

void printa_Todas_Fitas(){
    for(int i = 0;i< NUM_FITAS;i++){
        printa_Fita(i);
    }
}

void define_Nome_Fita_Saida(char nome[]){
    int numFitasSaida =( NUM_FITAS / 2);
    if(numFitasSaida > 9){
        nome[8] = nome[8] + (numFitasSaida/10);
        nome[9] = nome[9] + (numFitasSaida%10);
    }else{
        printf("\n%c + %d",nome[9],numFitasSaida%10);
        nome[9] += numFitasSaida;
    }
}




/*
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
*/