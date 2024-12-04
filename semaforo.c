#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 5

sem_t start_semaphore; // Semáforo para sinalizar o início da corrida
sem_t finish_semaphore; // Semáforo para controlar a linha de chegada
int finished_position = 1; // Posição atual na linha de chegada

void* runner(void* arg) {
    int id = *(int*)arg;
    free(arg); // Liberar a memória alocada para o ID

    printf("Corredor %d está na posição de largada.\n", id);

    // Espera o semáforo liberar para começar
    sem_wait(&start_semaphore);

    printf("Corredor %d começou a correr!\n", id);

    // Simula o tempo de corrida
    for (int i = 1; i <= 3; i++) {
        //sleep(rand() % 3 + 1);  Tempo aleatório entre 1 e 3 segundos por "volta" para ter uma diferença de tempo mais notavel nos resultados
        printf("Corredor %d completou a volta %d.\n", id, i);
    }
    
    // Tentar cruzar a linha de chegada
    sem_wait(&finish_semaphore);
    printf("Corredor %d cruzou a linha de chegada na posição %d!\n", id, finished_position++);
    sem_post(&finish_semaphore);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    sem_init(&start_semaphore, 0, 0); // Inicializa o semáforo com valor 0
    sem_init(&finish_semaphore, 0, 1); // Semáforo para controle da linha de chegada

    printf("Preparando para a corrida...\n");

    // Cria as threads
    for (int i = 0; i < NUM_THREADS; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1; // Identificador do corredor
        if (pthread_create(&threads[i], NULL, runner, id) != 0) {
            perror("Erro ao criar thread");
            return 1;
        }
    }

    sleep(2); // Simula tempo de preparação
    printf("Atenção, largada!\n");

    // Libera todas as threads ao mesmo tempo
    for (int i = 0; i < NUM_THREADS; i++) {
        sem_post(&start_semaphore);
    }

    // Aguarda todas as threads terminarem
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Limpeza
    sem_destroy(&start_semaphore);
    sem_destroy(&finish_semaphore);

    printf("Corrida finalizada! Todos os corredores cruzaram a linha de chegada.\n");

    return 0;
}
