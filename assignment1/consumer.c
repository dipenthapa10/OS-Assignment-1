
// consumer.c
// Dipen Thapa
// 2025-10-26
// homework assignment 1 - Operating Systems

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> // Needed for semaphore functions
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

// Names for shared memory and semaphores (must match producer)
#define SHM_NAME "/prodcon_shm"
#define SEM_EMPTY "/sem_empty"
#define SEM_FULL "/sem_full"
#define SEM_MUTEX "/sem_mutex"
#define BUFFER_SIZE 2

// Same structure as producer - this is our shared "table"
typedef struct
{
    int buffer[BUFFER_SIZE]; // The table that holds items
    int in;                  // Where producer puts items
    int out;                 // Where consumer takes items from
} buffer_t;

int main()
{
    printf("Consumer: Starting...\n");

    // Open the existing shared memory
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open failed");
        printf("consumer: mmake sure producer is running first!\n");
        exit(1);
    }

    // Map shared memory into our process
    buffer_t *buf = mmap(NULL, sizeof(buffer_t),
                         PROT_READ | PROT_WRITE, MAP_SHARED,
                         shm_fd, 0);
    if (buf == MAP_FAILED)
    {
        perror("map failed");
        exit(1);
    }

    // Open the existing semaphores (created by producer)
    sem_t *empty = sem_open(SEM_EMPTY, 0); // Tracks empty slots
    sem_t *full = sem_open(SEM_FULL, 0);   // Tracks full slots
    sem_t *mutex = sem_open(SEM_MUTEX, 0); // For mutual exclusion

    if (empty == SEM_FAILED || full == SEM_FAILED || mutex == SEM_FAILED)
    {
        perror("sem_open failed");
        printf("Consumer make sure producer is running first!\n");
        exit(1);
    }

    printf("Consumer connected to shared memory and semaphores\n");
    printf("Consumer starting consumption...\n");

    // Consumer process - keeps consuming items forever
    while (1)
    {
        // Wait for a full slot (if buffer empty, wait here)
        sem_wait(full);
        // Get the lock to access the buffer safely
        sem_wait(mutex);

        // Take item from buffer
        int item = buf->buffer[buf->out];
        printf("Consumer consumed %d from position %d\n", item, buf->out);
        // Move to next position
        buf->out = (buf->out + 1) % BUFFER_SIZE;

        // Release the lock
        sem_post(mutex);
        // Signal that an empty slot
        sem_post(empty);

        sleep(2); // here its waiting 2 seconds
    }

    // Cleanup
    munmap(buf, sizeof(buffer_t));
    close(shm_fd);
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);

    return 0;
}