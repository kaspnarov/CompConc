/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: "Hello World" usando threads em C */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS 2
int Vetor[10000];

//funcao de uma thread que incrementa em 1 as ultimas 5000 casas do Vetor
void *ContaSup (void *arg){
    for(int i=5000; i<10000; i++){
        Vetor[i] += 1;
    }
    pthread_exit(NULL);
}

//funcao de uma thread que incrementa em 1 as primeiras 5000 casas do Vetor
void *ContaInf (void *arg){
    for(int i=0; i<5000; i++){
        Vetor[i] += 1;
    }
    pthread_exit(NULL);
}

//--funcao principal do programa
int main(void) {
    pthread_t tid[NTHREADS]; //identificadores das threads no sistema
    int TudoTop = 1; //variavel para confirmacao
    for(int i=0; i<10000; i++){ //inicializacao do Vetor
        Vetor[i] = 0;
    }

    if (pthread_create(&tid[0], NULL, ContaInf, NULL)) {
        printf("--ERRO: pthread_create() ContaInf\n"); exit(-1);
    }
    if (pthread_create(&tid[1], NULL, ContaSup, NULL)) {
        printf("--ERRO: pthread_create() ContaSup\n"); exit(-1);
    }

    for(int i=0; i<NTHREADS; i++){
        if(pthread_join(tid[i], NULL))
            printf("--ERRO: pthread_join()\n");
    }
    for(int i=0; i<10000; i++){ //checa se o Vetor foi incrementado em 1
        if(Vetor[i] != 1){
            printf("%d", Vetor[i]);
            TudoTop = 0;
        }
    }
    if(TudoTop) //se o Vetor tiver sido incrementado, imprime uma mensagem positiva
        printf("Tudo top rs\n");
    else //senão, imprime uma mensagem negatia
        printf("Deu ruim :(\n");
        
    printf("--Thread principal terminou\n");

    pthread_exit(NULL);
}
