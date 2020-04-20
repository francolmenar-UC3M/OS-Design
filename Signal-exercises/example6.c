#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void sig_alrm(int signo)
{ 
  static int count=0;

  printf("caught SIGALRM, count = %d\n", count);
  count++;
  if (count >= 5) 
    exit(0);
  alarm(1);
}

int main(void) 
{
  struct sigaction act;

  act.sa_handler = sig_alrm;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  sigaction(SIGALRM, &act, NULL);
  alarm(1);

  for(;;)
    ;
}
  
