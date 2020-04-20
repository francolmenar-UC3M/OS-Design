#include <signal.h>
#include <stdio.h>
#include <unistd.h>

/*
   Exercise 2. Signals. Understanding sigwait

   Complete "..." with the corresponding parameters.
   Afterwards check if it works well!

   Description:

   This program uses sigwait to count the number of times
   the SIGINT signal is delivered to the process. Notice
   that no signal handler is necessary, since the signal
   is always blocked.
*/

int main(void) {
    int signalcount = 0;
    int signo;
    int signum = SIGINT;
    sigset_t sigset;

    // Block only signal sigum
    if ((sigemptyset( &sigset ) == -1) ||
          (sigaddset( &sigset, signum ) == -1) ||
          (sigprocmask( SIG_SETMASK, &sigset, NULL ) == -1))
        perror("Failed to block signals before sigwait");

    fprintf(stderr, "This process has ID %ld\n", (long)getpid());
    for ( ; ; ) {
        // Suspend execution until signum becomes pending;
        // The signal received should be stored in signo
        if (sigwait( &sigset, &signum ) == -1) {
            perror("Failed to wait using sigwait");
            return 1;
        }
        signalcount++;
        fprintf(stderr, "Number of signals so far: %d\n", signalcount);
    }
}
