

// producer.c
// Dipen Thapa
// 2025-10-26
// homework assignment 1 - Operating Systems

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

// Names for shared memory and semaphores
#define SHM_NAME "/prodcon_shm"
#define SEM_EMPTY "/sem_empty"
#define SEM_FULL "/sem_full"
#define SEM_MUTEX "/sem_mutex"
#define BUFFER_SIZE 2

// A shared buffer structure for communicating between processes
typedef struct
{
    int buffer[BUFFER_SIZE]; // The table that holds items that is size 2
    int in;                  // Position where producer puts next item
    int out;                 // Position where consumer takes next item
} buffer_t;

int main()
{
    printf("Producer: Starting...\n");

    // Clean up any existing shared memory and semaphores first
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    sem_unlink(SEM_MUTEX);

    // Create shared memory that both processes can access
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open failed");
        exit(1);
    }

    // Set the size of shared memory
    if (ftruncate(shm_fd, sizeof(buffer_t)) == -1)
    {
        perror("ftruncate failed");
        exit(1);
    }

    // Map shared memory into our process
    buffer_t *buf = mmap(NULL, sizeof(buffer_t),
                         PROT_READ | PROT_WRITE, MAP_SHARED,
                         shm_fd, 0);
    if (buf == MAP_FAILED)
    {
        perror("mmap failed");
        exit(1);
    }

    // Initialize buffer positions to start at 0
    buf->in = 0;
    buf->out = 0;

    // Created three semaphores for synchronization:
    sem_t *empty = sem_open(SEM_EMPTY, O_CREAT, 0666, BUFFER_SIZE); // 2 empty slots
    sem_t *full = sem_open(SEM_FULL, O_CREAT, 0666, 0);             // 0 full slots initially
    sem_t *mutex = sem_open(SEM_MUTEX, O_CREAT, 0666, 1);           // 1 = unlocked

    if (empty == SEM_FAILED || full == SEM_FAILED || mutex == SEM_FAILED)
    {
        perror("sem_open failed");
        exit(1);
    }

    printf("Producer: Shared memory and semaphores initialized\n");
    printf("Producer: Starting production...\n");

    int item = 1;

    // Producer process - keeps producing items forever
    while (1)
    {
        // Wait for an empty slot here if buffer is full, wait here
        sem_wait(empty);
        // Get the lock to access the buffer safely
        sem_wait(mutex);

        // CRITICAL SECTION: Put item in buffer
        buf->buffer[buf->in] = item;
        printf("Producer produced %d at position %d\n", item, buf->in);
        // Move to next position (wrap around if at end)
        buf->in = (buf->in + 1) % BUFFER_SIZE;
        item++; // Prepare next item number

        // Release the lock
        sem_post(mutex);
        // Signal that a new item is available
        sem_post(full);

        sleep(1); // Wait 1 second before producing next item
    }

    // Cleanup (this code will never be reached due to infinite loop)
    munmap(buf, sizeof(buffer_t));
    close(shm_fd);
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);

    return 0;
}