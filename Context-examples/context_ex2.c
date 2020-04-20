#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>

/* 
   DSO - Context switches
   Exercise 2

   Expected output:

   main: swapcontext from main to func2
   func2: started
   func2: swapcontext from func2 to func1
   func1: started
   func1: swapcontext from func1 to func2
   func2: returning
   func1: returning
   Exit main
*/

static ucontext_t uctx_main, uctx_func1, uctx_func2;

#define handle_error(msg)					\
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

static void func1(void)
{
  printf("func1: started\n");
  printf("func1: swapcontext from func1 to func2\n");
  if (swapcontext(&uctx_func1, &uctx_func2) == -1) handle_error("swapcontext");
  printf("func1: returning\n");
}

static void
func2(void)
{
  printf("func2: started\n");
  printf("func2: swapcontext from func2 to func1\n");
  if (swapcontext(&uctx_func2, &uctx_func1) == -1) handle_error("swapcontext");
  printf("func2: returning\n");
}

int
main(int argc, char *argv[])
{
  char func1_stack[16384];
  char func2_stack[16384];
  
  /* Get context from func1 */
  if (getcontext(&uctx_func1) == -1) handle_error("getcontext");
  /* Set stack parameters for func1 */
  uctx_func1.uc_stack.ss_sp = func1_stack;
  uctx_func1.uc_stack.ss_size = 16384;
  /* Successor context for func1 is main */
  uctx_func1.uc_link = ;
  /* Make context from func1 */
  makecontext(&uctx_func1, );
  
  /* Get context from func2 */
  // if (getcontext( ... ) == -1) handle_error("getcontext");
  /* Set stack parameters for func1 */
  // uctx_func2.uc_stack.ss_sp = ...;
  // uctx_func2.uc_stack.ss_size = ...;
  /* Successor context for func2 is func1 */
  // uctx_func2.uc_link = ...;
  /* Make context from func2 */
  // makecontext( ... );
  
  printf("main: swapcontext to from main to func2\n");
  // if (swapcontext(...) == -1) handle_error("main: exiting\n");

  printf("Exit main\n");
  exit(EXIT_SUCCESS);
}
