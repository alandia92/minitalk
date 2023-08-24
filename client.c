#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>

void handle_signal(int signal, siginfo_t *info, void *context)
{
    (void)info;
    (void)context;

    if (signal == SIGUSR1)
        printf("%d\n", signal);
}

void  message(int pid, char *str)
{
    int		i;
    char	c;

    while (*str)
    {
        i = 8;
        c = *str;

        str++;
        while (i--)
        {
            if (c >> i & 1)
                kill(pid, SIGUSR2);
            else
                kill(pid, SIGUSR1); // envia SIGUR1 cuando el bit es cero
            usleep(100);
        }
    }
    i = 8;
    while (i--)
    {
        kill(pid, SIGUSR1);
        usleep(100);
    }

}


int main(int argc, char *argv[])
{
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handle_signal;
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        write(1, "Error\n", 6);
    message(atoi(argv[1]), argv[2]);
    return (0);
}
