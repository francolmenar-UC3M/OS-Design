#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* 
   Exercise 1. Signals. Understanding sigpending

   Complete "..." with the corresponding parameters.
   Afterwards check if it works well!

   Description:

   The following program shows a correct way to wait for a 
   single signal. Assume that a signal handler has been set up 
   for the signum signal and that the signal handler sets 
   sigreceived to 1.

   This program uses three signal sets to control the blocking 
   and unblocking of signals at the appropriate time. It sets 
   maskall to contain all signals and maskmost to contain all 
   signals but signum. Next it blocks all signals, tests 
   sigreceived, and suspends the process if the signal has not 
   yet been received. 
*/

static volatile sig_atomic_t sigreceived = 0;

void sighandler(int signo)
{ 
  sigreceived = 1;
  printf("SIGINT received!\n");
}

int main() {
  sigset_t maskall, maskmost, maskold;
  int signum = SIGINT;

  signal(SIGINT, sighandler); /*set the handler for the signal*/

  sleep(1);

  /* Fill mask maskall */
  sigfillset( &maskall );
  /* Fill mask maskmost */
  sigfillset( &maskmost );
  /* Delete signum from maskmost */
  sigdelset( &maskmost, signum );

  /* Block all signals in maskall and store current in maskold */
  sigprocmask( SIG_BLOCK, &maskall, &maskold );

  if (sigreceived == 0){
     printf("SIGINT was not yet received!\n");

     /*  No signals can be caught between the testing and the suspending, 
         since the signal is blocked at this point. The process signal mask 
         has the value maskmost while the process is suspended, so only 
         signum is not blocked. When sigsuspend returns, the signal must 
         have been received. 
     */
 
     sigsuspend( &maskmost );
  }

  /* Restore process signal mask set in maskold */
  sigprocmask( SIG_SETMASK, &maskold, NULL );

  return 0;
}
  
