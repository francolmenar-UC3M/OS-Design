#include <sys/types.h>
#include <signal.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static void sig_usr1(int signo)
{
   printf("caught SIGUSR1\n");
}

int main(void) 
{
  struct sigaction abc;
  int i;
  pid_t pid;

  if ((pid=fork()) == 0) /* Child */
  {
    abc.sa_handler = sig_usr1;
    sigemptyset(&abc.sa_mask);
    abc.sa_flags = 0;
    sigaction(SIGUSR1, &abc, NULL);
    for(;;) 
      ;
  } 
  else /* Parent */
  {
    sleep(1);
    for(i=0; i<5; i++) 
    {
      kill(pid, SIGUSR1);
      sleep(1);
    }
  }

  return 0;
}

