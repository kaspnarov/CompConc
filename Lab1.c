/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS 2
int Vetor[10000];

//funcao que incrementa em 1 metade das casas do Vetor
void *AddUm (void *arg){
    int base = * (int *) arg;
    for(int i=base; i<base+5000; i++){
        Vetor[i] += 1;
    }
    pthread_exit(NULL);
}

//funcao principal do programa
int main(void) {
    pthread_t tid[NTHREADS]; //identificadores das threads no sistema
    int TudoTop = 1; //variavel para confirmacao final
    int baseInf = 0; //base para o parmetro da primeira thread
    int baseSup = 5000; //base para o parametro da segunda thread
    for(int i=0; i<10000; i++){ //inicializacao do Vetor
        Vetor[i] = 0;
    }

    if (pthread_create(&tid[0], NULL, AddUm, (void *)&baseInf)) { //cria a primeira thread
        printf("--ERRO: pthread_create() ContaInf\n"); exit(-1);
    }
    if (pthread_create(&tid[1], NULL, AddUm, (void *)&baseSup)) { //cria a segunda thread
        printf("--ERRO: pthread_create() ContaSup\n"); exit(-1);
    }

    for(int i=0; i<NTHREADS; i++){ //espera as threads terminarem o processo
        if(pthread_join(tid[i], NULL))
            printf("--ERRO: pthread_join()\n");
    }
    for(int i=0; i<10000; i++){ //checa se o Vetor foi incrementado em 1
        if(Vetor[i] != 1){
            TudoTop = 0;
        }
    }
    if(TudoTop) //se o Vetor tiver sido incrementado, imprime uma mensagem positiva
        printf("O Vetor foi incrementado corretamente\n");
    else //senão, imprime uma mensagem negativa
        printf("Deu ruim :(\n");
        

    pthread_exit(NULL);
}
