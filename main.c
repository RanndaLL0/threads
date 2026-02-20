#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

#define N 1000000000
#define NUMBER_OF_THREADS 8

typedef struct {
    int *array;
    int start_index;
    int end_index;
    long long partial_sum;
} ThreadData;

void* sum_array_segment(void* args) {

    ThreadData* thread_data = (ThreadData*)args;
    thread_data->partial_sum = 0;

    for(int i = thread_data->start_index; i < thread_data->end_index; i++) {
        thread_data->partial_sum += thread_data->array[i];
    }
    pthread_exit(NULL);
}

int main () {
    int *arr = malloc(N * sizeof(int));
    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        int r = rand() % 10 + 1;
        arr[i] = r;
    }


    long long sinc_sum = 0;

    clock_t start = clock();
    for (int i = 0; i < N; i++) {
        sinc_sum += arr[i];
    }
    clock_t end = clock();

    double time_diff = (double)(end - start) / CLOCKS_PER_SEC;

    printf("diferenca de tempo do sincrono: %.2f \n", time_diff);
    printf("resultado da soma: %lld \n\n", sinc_sum);

    long long async_sum;
    pthread_t threads[NUMBER_OF_THREADS];
    ThreadData thread_data[NUMBER_OF_THREADS];

    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        thread_data[i].array = arr;
        thread_data[i].start_index = i * (N/NUMBER_OF_THREADS);
        thread_data[i].end_index = (i + 1) * (N/NUMBER_OF_THREADS);
        pthread_create(&threads[i], NULL, sum_array_segment, (void *)&thread_data[i]);
    }

    async_sum = 0;
    clock_t start_async = clock();
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_join(threads[i], NULL);
        async_sum += thread_data[i].partial_sum;
    }
    clock_t end_async = clock();

    double time_diff_async = (double)(end_async - start_async) / CLOCKS_PER_SEC;
    printf("diferenca de tempo do asyncrono: %.2f \n", time_diff_async);
    printf("resultado da soma do asyncrono: %lld \n", async_sum);

    free(arr);
    return 0;   
}