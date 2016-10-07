#include<stdio.h>
#include<stdlib.h>
#define STACK_SIZE 1024 * 1024
struct thread
{
unsigned char* stack_pointer;
void* initial_argument;
void (*initial_function)(void*);
};

int factorial(int n) {
    return n == 0 ? 1 : n * factorial(n-1);
}

void fun_with_threads(void * arg) {
    int n = *(int*) arg;
    printf("%d! = %d\n", n, factorial(n));
}

struct thread * current_thread  ;
struct thread * inactive_thread;
void thread_switch(struct thread * old, struct thread * new);
void thread_start(struct thread * old, struct thread * new );

void  main() {
 current_thread = (struct thread*) malloc (sizeof(struct thread));
 inactive_thread = (struct thread*) malloc (sizeof(struct thread));
 current_thread->initial_function = fun_with_threads;
 int * p = malloc(sizeof(int));
 *p = 5;
 current_thread->initial_argument = p;
 current_thread->stack_pointer = malloc(STACK_SIZE) + STACK_SIZE;
 current_thread->initial_function(current_thread->initial_argument);
 thread_start(inactive_thread , current_thread);
 free(p);
 free(inactive_thread);
 free(current_thread);
}

 void yield() {
  struct thread * temp = current_thread;
  current_thread = inactive_thread;
  inactive_thread = temp;
  thread_switch(inactive_thread, current_thread);
}

void thread_wrap() {
  current_thread->initial_function(current_thread->initial_argument);
  yield();
}

