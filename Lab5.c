/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Lab5 - Thiago Duarte Campbell - 118060285 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Variaveis globais */
int bloqueadas = 0, NTHREADS, vetorConc[100000], vetorSeq[100000], vetorAux[10000];
pthread_mutex_t x_mutex;
pthread_mutex_t x_mutex_2;
pthread_cond_t x_cond;

//funcao barreira
void barreira(int NTHREADS) {
    pthread_mutex_lock(&x_mutex); //inicio secao critica
    if (bloqueadas == (NTHREADS-1)) { 
        //ultima thread a chegar na barreira
        for(int i = 0; i < NTHREADS; i++){
            vetorConc[i] = vetorConc[i] + vetorAux[i];
        }
        pthread_cond_broadcast(&x_cond);
        bloqueadas=0;
    } else {
        bloqueadas++;
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex); //fim secao critica
}

//funcao das threads
void *tarefa (void *arg) {
    int id = *(int*)arg;
    int boba1, boba2;

    for (int i=1; i <= NTHREADS; i<<=1) {        
        if(id-i >= 0) {vetorAux[id] = vetorConc[id-i];}
        else {vetorAux[id] = 0;}
        //printf("Thread %d: passo=%d // Aux %d: vetor %d\n", id, i, vetorAux[id], vetor[id]);

        /* faz alguma coisa... */
        boba1=100; boba2=-100; while (boba2 < boba1) boba2++;

        //sincronizacao condicional
        barreira(NTHREADS);
    }
    pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
    if(argc < 3) {
        fprintf(stderr, "Digite: %s <vetor de 2^n entradas separado por espaços>\n", argv[0]);
        return 1; 
    }
    NTHREADS = argc - 1;
    //printf("NTHREADS: %d\n", NTHREADS);
    pthread_t threads[NTHREADS];
    int id[NTHREADS], errado=0;
    for(int i = 0; i < NTHREADS; i++){
        vetorConc[i] = atol(argv[i+1]);
        vetorSeq[i] = vetorConc[i];
        vetorAux[i] = 0;
    }
    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init (&x_cond, NULL);

    /* Cria as threads */
    for(int i=0;i<NTHREADS;i++) {
        id[i]=i;
        pthread_create(&threads[i], NULL, tarefa, (void *) &id[i]);
    }

    /* Espera todas as threads completarem */
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    for(int i = 0; i < NTHREADS-1; i++){ vetorSeq[i+1] += vetorSeq[i]; }

    printf("Vetor Concorrente: [");
    for(int i = 0; i < NTHREADS; i++){
        if(i == NTHREADS-1) printf("%d]\n", vetorConc[i]);
        else printf("%d, ", vetorConc[i]);
        if(vetorConc[i] != vetorSeq[i]) errado++;
    }
    printf("Vetor Sequencial: [");
    for(int i = 0; i < NTHREADS; i++){ 
        if(i == NTHREADS-1) printf("%d]\n", vetorConc[i]);
        else printf("%d, ", vetorSeq[i]); 
    }
    if(errado == 0)
        puts("O cálculo foi executado corretamente! :)");
    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    return 0;
}
