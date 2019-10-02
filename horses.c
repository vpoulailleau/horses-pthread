#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "vt100.h"

typedef struct
{
    unsigned char thread_id;
    char *name;
} horse_param_t;

static unsigned char running = 0;

static pthread_mutex_t mutex_running = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_stdout = PTHREAD_MUTEX_INITIALIZER;

void *horse(void *param_pointer)
{
    unsigned long distance = 0;
    horse_param_t *params = (horse_param_t *)param_pointer;
    unsigned long y = 5 + 2 * params->thread_id;

    while (!running)
    {
        usleep(1000);
    }

    pthread_mutex_lock(&mutex_stdout);
    vt100_move(1, y);
    printf("%.15s\n", params->name);
    pthread_mutex_unlock(&mutex_stdout);

    while (distance < 7901 && running)
    {
        distance += rand() % 60;
        pthread_mutex_lock(&mutex_stdout);
        vt100_move(20 + distance / 100, y);
        putchar('#');
        pthread_mutex_unlock(&mutex_stdout);
        usleep(rand() % 50000 + 5000);
    }
    pthread_mutex_lock(&mutex_running);
    running = 0;
    pthread_mutex_unlock(&mutex_running);
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    unsigned char nb_horses = argc - 1;
    pthread_t threads[nb_horses];
    horse_param_t threads_params[nb_horses];
    int ret_create;
    unsigned char thread_id;

    if ((argc <= 1) || (argc > 20))
    {
        fprintf(
            stderr,
            "Invalid number of horses (should be between 1 and 20)\n"
            "./horses Name1 Name2 Name3 ...\n");
        return EXIT_FAILURE;
    }

    srand(time(NULL));
    for (thread_id = 0; thread_id < nb_horses; thread_id++)
    {
        printf("Horse #%d birth\n", thread_id);
        threads_params[thread_id].thread_id = thread_id;
        threads_params[thread_id].name = argv[thread_id + 1];
        ret_create = pthread_create(
            &threads[thread_id],
            NULL,
            horse,
            (void *)&threads_params[thread_id]);
        if (ret_create)
        {
            fprintf(
                stderr,
                "ERROR: pthread_create() return code is %d\n",
                ret_create);
            return EXIT_FAILURE;
        }
    }

    pthread_mutex_lock(&mutex_stdout);
    vt100_clear_screen();
    vt100_move(10, 2);
    pthread_mutex_unlock(&mutex_stdout);
    puts("°º¤ø,¸¸,ø¤º°`°º¤ø,¸,ø¤°º¤ø,¸¸,ø¤º°     HORSE RACE     °º¤ø,¸¸,ø¤º°`°º¤ø,¸,ø¤°º¤ø,¸¸,ø¤º°");
    pthread_mutex_lock(&mutex_running);
    running = 1;
    pthread_mutex_unlock(&mutex_running);

    while (running)
        ;
    pthread_mutex_lock(&mutex_stdout);
    vt100_move(1, 7 + 2 * nb_horses);
    pthread_mutex_unlock(&mutex_stdout);

    pthread_exit(NULL);
    return EXIT_SUCCESS;
}
