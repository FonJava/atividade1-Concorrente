#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_INGRESSOS 10
#define NUM_PESSOAS 15
#define NUM_PRODUCERS 2
#define MAX_INGRESSOS_POR_COMPRA 4

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_not_empty = PTHREAD_COND_INITIALIZER;

int ingressos_disponiveis = NUM_INGRESSOS;
int queue[NUM_PESSOAS];
int queue_count = 0;

void *compra_ingresso(void *thread_id) {
    int tid = *((int *)thread_id);
    int reserva = (rand() % MAX_INGRESSOS_POR_COMPRA) + 1;

    pthread_mutex_lock(&mutex);
    while (ingressos_disponiveis < reserva) {
        printf("Pessoa %d: Ingressos insuficientes. Aguardando...\n", tid);
        pthread_cond_wait(&cond_not_empty, &mutex);
    }

    printf("Pessoa %d: Reservando %d ingressos.\n", tid, reserva);
    queue[queue_count++] = reserva;
    ingressos_disponiveis -= reserva;
    printf("Ingressos disponíveis após a reserva: %d\n", ingressos_disponiveis);
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond_not_full);

    pthread_exit(NULL);
}

void *produtor_de_ingressos(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (queue_count == 0) {
            printf("Produtor de Ingressos: Nenhuma compra na fila. Aguardando...\n");
            pthread_cond_wait(&cond_not_full, &mutex);
        }

        int ingressos = queue[--queue_count];
        ingressos_disponiveis += ingressos;
        printf("Produtor de Ingressos: Adicionados %d ingressos. Total agora: %d\n", ingressos, ingressos_disponiveis);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond_not_empty);
        sleep(rand() % 3); // Espera um tempo aleatório antes de adicionar mais ingressos
    }
}

int main() {
    pthread_t compradores[NUM_PESSOAS];
    pthread_t produtores[NUM_PRODUCERS];

    srand(time(NULL));

    for (int i = 0; i < NUM_PESSOAS; i++) {
        int *pessoa_id = malloc(sizeof(int));
        *pessoa_id = i + 1;
        pthread_create(&compradores[i], NULL, compra_ingresso, pessoa_id);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_create(&produtores[i], NULL, produtor_de_ingressos, NULL);
    }

    for (int i = 0; i < NUM_PESSOAS; i++) {
        pthread_join(compradores[i], NULL);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_cancel(produtores[i]);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_not_full);
    pthread_cond_destroy(&cond_not_empty);

    return 0;
}
