#include <stdio.h>
#include <stdlib.h> 
#include <signal.h> // sigaction(), sigsuspend(), sig*()
#include <unistd.h> // alarm()

void handle_signal(int signal);
void handle_sigalrm(int signal);
void do_sleep(int seconds);

/* 
   Exercise 3. Signals. A more complex exercise with sigpending

   Complete "..." with the corresponding parameters.
   Afterwards check if it works well!

   Usage example:
  
   It will print out its pid. Use it from another terminal to send signals
      $ kill -HUP <pid>
      $ kill -USR1 <pid>
      $ kill -ALRM <pid>
 
   Exit the process with ^C ( = SIGINT) or SIGKILL, SIGTERM
*/

int main() {
    struct sigaction sa;

    // Print pid, so that we can send signals from other shells
    printf("My pid is: %d\n", getpid());
    // Setup the sighub handler with handle_signal function
    sa.sa_handler = ...;
    // Restart the system call, if at all possible
    sa.sa_flags = SA_RESTART;

    // Block every signal during the handler; fill sa.sa_mask
    sigfillset( ... );

    // Intercept SIGHUP and SIGINT
    if (sigaction(SIGHUP, &sa, NULL) == -1) 
       perror("Error: cannot handle SIGHUP"); // Should not happen 

    if (sigaction(SIGUSR1, &sa, NULL) == -1) 
       perror("Error: cannot handle SIGUSR1"); // Should not happen 

    // Will always fail, SIGKILL is intended to force kill your process
    if (sigaction(SIGKILL, &sa, NULL) == -1) {
        perror("Cannot handle SIGKILL"); // Will always happen
        printf("You can never handle SIGKILL anyway...\n");  }

    if (sigaction(SIGINT, &sa, NULL) == -1) 
        perror("Error: cannot handle SIGINT"); // Should not happen

    for (;;) {
        printf("\nSleeping for ~3 seconds\n");
        do_sleep(3); // Later to be replaced with a SIGALRM
    }
}

void handle_signal(int signal) {
    /*
     * Please note that printf et al. are NOT safe to use in signal handlers.
     * Look for async safe functions.
     */
    const char *signal_name;
    sigset_t pending;

    // Find out which signal we're handling
    switch (signal) {
        case SIGHUP:  signal_name = "SIGHUP";  break;
        case SIGUSR1: signal_name = "SIGUSR1"; break;
        case SIGINT:  printf("Caught SIGINT, exiting now\n"); exit(0);
        default:      fprintf(stderr, "Caught wrong signal: %d\n", signal); return;
    }

    printf("Caught %s, sleeping for ~3 seconds\n"
           "Try sending another SIGHUP / SIGINT / SIGALRM "
           "(or more) meanwhile\n", signal_name);
    /*
     * Indeed, all signals are blocked during this handler
     * But, at least on OSX, if you send 2 other SIGHUP,
     * only one will be delivered: signals are not queued
     * However, if you send HUP, INT, HUP,
     * you'll see that both INT and HUP are queued
     * Even more, on my system, HUP has priority over INT
     */
    do_sleep(3);
    printf("Done sleeping for %s\n", signal_name);

    // So what did you send me while I was asleep?

    // Suspend until signals in pending mask are caught
    sigpending( ... );
    if (sigismember(&pending, SIGHUP)) {
        printf("A SIGHUP is waiting\n");
    }
    if (sigismember(&pending, SIGUSR1)) {
        printf("A SIGUSR1 is waiting\n");
    }

    printf("Done handling %s\n\n", signal_name);
}

void handle_sigalrm(int signal) {
    if (signal != SIGALRM) {
        fprintf(stderr, "Caught wrong signal: %d\n", signal);
    }

    printf("Got sigalrm, do_sleep() will end\n");
}

void do_sleep(int seconds) {
    struct sigaction sa;
    sigset_t mask;

    // Set sa_handler to handle_sigalrm
    sa.sa_handler = ...; // Intercept and ignore SIGALRM
    sa.sa_flags = SA_RESETHAND; // Remove the handler after first signal
    // Fill mask sa.sa_mask
    sigfillset( ... );

    sigaction(SIGALRM, &sa, NULL);

    // Get the current signal mask and store it mask
    sigprocmask( ... );

    // Unblock SIGALRM in mask
    sigdelset( ... );

    // Wait with this mask "seconds"
    alarm( ... );
    // Suspend until SIGALRM is caught
    sigsuspend( ... );

    printf("sigsuspend() returned\n");
}
