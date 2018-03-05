#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>

/* 
   DSO - Context switches
   Exercise 1

   Expected output:

   Swap from the main context to the func1 context
   func1: started
   func1: returning
   When the func1 finished the context is switched again to main which now exits
*/

static ucontext_t uctx_main, uctx_func1;

#define handle_error(msg)					\
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

static void func1(void)
{
  printf("func1: started\n");
  printf("func1: returning\n");
}

int main(int argc, char *argv[])
{
  char func1_stack[16384];
/* Intialize uctx_func1 to the current active context */
  if (getcontext(&uctx_func1) == -1) handle_error("getcontext");
  /* Intialize stack of the func 1 context*/
  uctx_func1.uc_stack.ss_sp =  func1_stack; 
  uctx_func1.uc_stack.ss_size = 16384; 
  /* Define that uctx_main will be the new context that will be called when the func1 returns */
  uctx_func1.uc_link = &uctx_main; 
  /* Create the new context */
  makecontext(&uctx_func1, func1, 0); 
  printf("Swap from the main context to the func1 context\n");
  if (swapcontext(&uctx_main, &uctx_func1) == -1) handle_error("main: exiting\n");
  
  printf("When the func1 finished the context is switched again to main which now exits\n");
  exit(EXIT_SUCCESS);
}
