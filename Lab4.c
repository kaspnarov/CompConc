/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */
/* Aluno: Thiago Campbell // DRE: 118060285 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0, y = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;
pthread_cond_t x_cond_2;

/* Thread 4 */
void *A (void *t) {    
    pthread_mutex_lock(&x_mutex);
    if (x == 0) { 
        printf("Seja bem-vindo!\n");
        x++;
        pthread_cond_broadcast(&x_cond);
    }
    pthread_mutex_unlock(&x_mutex); 

    pthread_exit(NULL);
}

/* Thread 3 */
void *B (void *t) {
    pthread_mutex_lock(&x_mutex);
    if (x == 0) { 
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex); 
    x++;
    printf("Sente-se, por favor.\n");
    y++;
    if(y==2){
        pthread_cond_signal(&x_cond_2);
    }
    pthread_exit(NULL);
}

/* Thread 2 */
void *C (void *t) {
    pthread_mutex_lock(&x_mutex);
    if (x == 0) { 
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex); 
    x++;
    printf("Fique à vontade.\n");
    y++;
    if(y == 2){
        pthread_cond_signal(&x_cond_2);
    }
    pthread_exit(NULL);
}

/* Thread 1 */
void *D (void *t) {
    pthread_mutex_lock(&x_mutex);
    if (x < 3) { 
        pthread_cond_wait(&x_cond_2, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex); 
    printf("Volte sempre!\n");
    pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
    pthread_t threads[NTHREADS];

    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init (&x_cond, NULL);
    pthread_cond_init (&x_cond_2, NULL);

    /* Cria as threads */
    pthread_create(&threads[3], NULL, A, NULL);
    pthread_create(&threads[2], NULL, B, NULL);
    pthread_create(&threads[1], NULL, C, NULL);
    pthread_create(&threads[0], NULL, D, NULL);


    /* Espera todas as threads completarem */
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    pthread_cond_destroy(&x_cond_2);
    return 0;
}
