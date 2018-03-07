#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static void sig_int(int signo)
{
  printf("caught SIGINT\n");
}

int main(void) 
{
  sigset_t newmask, oldmask, pendmask;
  struct sigaction abc;

  abc.sa_handler = sig_int;
  sigemptyset(&abc.sa_mask); 
  abc.sa_flags = 0; 

  sigaction(SIGINT, &abc, NULL); /* Specify a SIGINT handler */

  sigemptyset(&newmask);   
  sigaddset(&newmask, SIGINT);
  if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) /* Block SIGINT */
    perror("sigprocmask");

  printf("sleep 5\n");
  sleep(5);
  printf("wakeup\n");

  if (sigpending(&pendmask) < 0) /* Check for pending, blocked signals */
    perror("sigpending");
  if (sigismember(&pendmask, SIGINT)) 
    printf("SIGINT pending\n");

  if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) /* Unblock SIGINT */
    perror("sigprocmask");

  printf("SIGINT unblocked\n");
  sleep(10); /* Can be used to demonstrate that sigint continues to be caught */
  
  return 0;
}

