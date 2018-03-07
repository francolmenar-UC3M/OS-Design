#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void sig_alrm(int signo)
{ 
  static int count = 0;
  printf("caught SIGALRM, count = %d\n", count);
  count++;
  if (count >= 5) 
    exit(0);
  alarm(1);
}

int main() {
  int i;
  struct sigaction act;
  char ch[100];

  ch[0] = 0;
  act.sa_handler = sig_alrm;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(SIGALRM, &act, 0);

  alarm(1);
  for(i=0;;) 
  {
    size_t tt;
    tt=read(STDIN_FILENO, ch, 2);
    ch[2] = '\0';
    printf("i=%d, ch = %s, tt= %d\n", i++, ch, (int)tt);
  } 

  return 0;
}
  
