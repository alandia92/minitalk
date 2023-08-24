#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>

void handle_signal(int signal, siginfo_t *info, void *context)
{
    //FALTA gestion de errores == -1
    int             i;
    unsigned char   c;
    static pid_t    client_pid;
    // pid_t representa el id del proceso

    (void)context;
    i = 0;
    c = 0;
    client_pid = 0;
    if (!client_pid)
        client_pid = info->si_pid; // almacena el ID del proceso que envió la señal, entonces si el valor de client_pid = 0, se le va a asignar el valor de si_pid
    if (signal == SIGUSR2)// cada vez que se recibe señal SUGUSR2 establece el bit más bajo en 1,
        c = (c << 1) | 1; // desplazamos bits hacia la izquierda, luego establece el bit más bajo en 1, sin afectar los otros bits. 00001010 desplazamos a 00010100. Asegurar 00010100 c|1 00010101
    else
        c = (c << 1);// desplazamos bits hacia la izquierda
    i++;
    if (i == 8)
    {
        i = 0; // reiniciamos el contador
        if (c == 0) // si no hay ningun bit, enviamos SIGUR2 al cliente para decirle que no hay datos
        {
            kill(client_pid, SIGUSR2);
            client_pid = 0;  // Reinicia el id del cliente
            return;
            (0);
        } else {
            write(1, &c, 1);
            c = 0;
            kill(client_pid, SIGUSR1);
        }
    }
    else
        c <<= 1;// desplazamos bits a la izquierda
}
int main(void)
{
    struct  sigaction sa;

    int server_pid;

    server_pid = getpid();
    printf("Server PID: %d\n", server_pid);

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handle_signal;
    if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1)
    {
        write(1, "Error\n", 6);
        return 1;
    }
    // FALTA CONTROL DE ERRORES == -1
    while (1)
        pause();// para esperar a la señal
    return (0);

}
