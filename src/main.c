#include <stdio.h>  // prinf()
#include <unistd.h> // fork()
#include <stdlib.h> // exit()
#include <sys/resource.h> // setpriority(), getpriority()
#include <time.h> // usleep()
#include <fcntl.h> // O_RDONLY

#define ADC4 "/sys/bus/iio/devices/iio:device0/in_voltage4_raw"
#define ADC6 "/sys/bus/iio/devices/iio:device0/in_voltage6_raw"
#define MAX_BUFF 64

int main(int argc, char **argv)
{
    printf("Priority Control with ADC running...\n");
    pid_t pid1, pid2;

    int counter = 0;
    pid1 = fork();

    if (pid1 > 0)
    { // Parent process
        pid2 = fork();
        if (pid2 > 0)
        { // Parent process
            printf("Luke, I am your father! \n");

            setpriority(PRIO_PROCESS, 0, 0);
            printf("DarthVader: my priority is %d\n", getpriority(PRIO_PROCESS, 0 ));

            int fd = 0;
            char buf[MAX_BUFF] = {0};
            char ch[5] = {0,0,0,0,0}; // four digits and null terminator \0

            while(1)
            {
                snprintf(buf, sizeof(buf), ADC4);
                printf("%s\n",buf);
                fd = open(buf, O_RDONLY | O_NONBLOCK);
                printf("%d\n",fd );
                read(fd, ch, 4); // reads 4 bytes from file FD to CH
                printf("%d\n", atoi(ch));

                close(fd);
                usleep(500000);
            }
        }
        else if (pid2 == 0)
        { // child process
            goto child;
        }
        else
        { // Process error during creation

        }
    }
    else if (pid1 == 0)
    { // Child process executes
        child:
        printf("Noooooooooo!\n");
    }
    else
    { // Process creation error

    }

    exit(0);
}
