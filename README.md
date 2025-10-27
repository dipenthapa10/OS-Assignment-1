# Producer-Consumer Program using Shared Memory and Semaphores

# Project Description

This project is a solution to the Producer-Consumer problem in Operating Systems. The producer and consumer are two different programs
that run at the same time and share information through a shared memory buffer. Semaphores are used so that both processes do not access 
the buffer at the same time. This prevents data corruption and shows how synchronization works in real systems.


#Files in This Repository

File Name

producer.c :  Creates items and adds them to the shared memory buffer. Uses semaphores to wait if the buffer is full. 
consumer.c : Removes items from the shared memory buffer and prints them. Waits if the buffer is empty. 
README.md : Instructions on how to compile, run and understand the project. 
screenshots/ : Folder containing pictures of program output to show correct execution. 


# What the Program Does

- The producer makes integer items one at a time.
- It puts each item into a shared buffer with only 2 spaces.
- The consumer takes items from that buffer and prints them.
- If buffer is full, the producer must wait.
- If buffer is empty, the consumer must wait.
- Semaphores control this waiting so the program runs safely.
  
This shows how Operating Systems prevent race conditions and manage communication between processes.


# How to Compile the Programs

- download virtual machine and setup
- next setup the gcc in terminal with : sudo apt install gcc
- then create directory with mkdir
- create the prodcuer and consumer files and compile those with the commands
   Compile the producer program
gcc producer.c -pthread -lrt -o producer

  Compile the consumer program
gcc consumer.c -pthread -lrt -o consumer
- run the program with ./producer & ./consumer &
- to stop the program pkill producer
pkill consumer or ctrl + C



