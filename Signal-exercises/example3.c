#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static void sig_int(int signo)
{
  signal(SIGINT, sig_int); /* To ensure that the handler is reset. Not guranteed to work. */
  printf("caught SIGINT\n");
}

int main(void) 
{
  sigset_t newmask, oldmask, pendmask;

  if (signal(SIGINT, sig_int) == SIG_ERR) /* Specify a SIGINT handler */
    perror("signal");

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

  return 0;
}
