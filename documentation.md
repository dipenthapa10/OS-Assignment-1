# Project Description

This project is a solution to the Producer-Consumer problem in Operating Systems. The producer and consumer are two different programs 
that run at the same time and share information through a shared memory buffer. Semaphores are used so that both processes do not access the buffer 
at the same time. This prevents data corruption and shows how synchronization works in real systems.

# how the results looks like 
Producer: Starting...
Producer: Shared memory and semaphores initialized
Producer: Starting production...
Consumer: Starting...
Consumer connected to shared memory and semaphores
Consumer starting consumption...

Producer produced 1 at position 0
Producer produced 2 at position 1
Consumer consumed 1 from position 0
Producer produced 3 at position 0
Consumer consumed 2 from position 1
Producer produced 4 at position 1
Consumer consumed 3 from position 0
Producer produced 5 at position 0
Consumer consumed 4 from position 1
Producer produced 6 at position 1
Consumer consumed 5 from position 0
Producer produced 7 at position 0
Consumer consumed 6 from position 1

... (this pattern continues forever until I stop the programs)

# Explanation of Key Components

Shared Memory - Our Shared Table
What I implemented: I created a shared memory space that acts like a physical table between two rooms. Both my producer 
and consumer programs can access this same "table" even though they're separate programs. I used shm_open() to create this shared space 
and mmap() to let both programs see it. This table can only hold 2 items at a time, exactly like the assignment required. I made sure to initialize
it properly so both programs start with a clean slate.

Semaphores
 I built a three-semaphore system that works like smart traffic lights. First, I created SEM_EMPTY to count empty spots - 
 it starts at 2 since our table begins empty. Second, I made SEM_FULL to track occupied spots - it starts at 0. Third, I added SEM_MUTEX as a
 lock to prevent both programs from using the table simultaneously. I used sem_open() to create these and made sure they coordinate perfectly - 
 the producer waits when the table is full, and the consumer waits when it's empty.

 Mutual Exclusion 
i implemented mutual exclusion using the SEM_MUTEX semaphore to ensure only one program touches the table at a time. Before either program accesses
the shared buffer, they call sem_wait(mutex) to get exclusive access. After they finish, they call sem_post(mutex) to release it.
This was crucial because without this lock, both programs might try to modify the table simultaneously, causing data corruption or race conditions.

Process Communication
 Instead of using threads within one program, I created two separate processes that communicate through shared memory.
 This was important because the assignment specifically required process-level communication. My producer and consumer are completely independent programs 
 that only talk through our shared table and semaphores. If one crashes, the other just waits patiently, demonstrating true process independence while maintaining perfect coordination.
