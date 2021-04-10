/* Calcula valor de pi com sequência */
/* Aluno Thiago Campbell */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include "timer.h"

long long int dim; //dimensao do vetor de entrada
int nthreads; //numero de threads
double *vetor; //vetor de entrada com dimensao dim 

//fluxo das threads
void * tarefa(void * arg) {
long int id = (long int) arg; //identificador da thread
double *somaLocal; //variavel local da soma de elementos
somaLocal = (double*) malloc(sizeof(double));

if(somaLocal==NULL) {exit(1);}
    long int tamBloco = dim/nthreads;  //tamanho do bloco de cada thread 
    long int ini = id * tamBloco; //elemento inicial do bloco da thread
    long int fim; //elemento final(nao processado) do bloco da thread
    if(id == nthreads-1) fim = dim;
    else fim = ini + tamBloco; //trata o resto se houver
    //soma os elementos do bloco da thread
    for(long int i=ini; i<fim; i++)
        *somaLocal += vetor[i];
    //retorna o resultado da soma local
    pthread_exit((void *) somaLocal); 
}

    //fluxo principal
int main(int argc, char *argv[]) {
    double somaSeq= 0; //soma sequencial
    double somaConc= 0; //soma concorrente
    double ini, fim, tempoSeq, tempoConc; //tomada de tempo
    pthread_t *tid; //identificadores das threads no sistema
    double *retorno; //valor de retorno das threads

    //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
    if(argc < 3) {
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
        return 1; 
    }
    dim = atoll(argv[1]);
    nthreads = atoi(argv[2]);

    GET_TIME(ini);

    //aloca o vetor de entrada
    vetor = (double*) malloc(sizeof(double)*dim);
    if(vetor == NULL) {
        fprintf(stderr, "ERRO--malloc (vetor)\n");
        return 2;
    }
    //preenche o vetor de entrada
    for(long int i=0; i<dim; i++)
        vetor[i] = (pow(-1,i))*(1.0/((2*(i+1))-1));

    GET_TIME(fim);
    //printf("Tempo inicialização:  %lf\n", fim-ini);

    //soma sequencial dos elementos
    GET_TIME(ini);
    for(long int i=0; i<dim; i++){
        somaSeq += vetor[i];
    }   
    GET_TIME(fim);
    tempoSeq = fim-ini;
    //printf("Tempo sequencial:  %lf\n", fim-ini);

    //soma concorrente dos elementos
    GET_TIME(ini);
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if(tid==NULL) {
        fprintf(stderr, "ERRO--malloc (tid)\n");
        return 2;
    }
    //criar as threads
    for(long int i=0; i<nthreads; i++) {
        if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
    //aguardar o termino das threads
    for(long int i=0; i<nthreads; i++) {
        if(pthread_join(*(tid+i), (void**) &retorno)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
        //soma global
        somaConc += *retorno;
    }
    GET_TIME(fim);
    tempoConc = fim-ini;
    //printf("Tempo concorrente:  %lf\n", fim-ini);

    /* //exibir os resultados
    printf("Valor pi seq:  %.15lf\n", 4*somaSeq);
    printf("Valor pi conc: %.15lf\n", 4*somaConc);
    printf("Valor pi: %.15lf\n", M_PI); */

    tempoSeq > tempoConc ? printf("Tempo concorrente foi %lf s mais rápido\n", tempoSeq-tempoConc) : printf("Tempo sequencial foi %lf s mais rápido\n", tempoConc-tempoSeq);
    M_PI - (4*somaSeq) > M_PI - (4*somaConc) ? puts("Concorrente é mais próximo de pi") : puts("Sequencial é mais próximo de pi");

    //libera as areas de memoria alocadas
    free(vetor);
    free(tid);

    return 0;
}
