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
            char buf_ADC4[MAX_BUFF] = {0};
            snprintf(buf_ADC4, sizeof(buf_ADC4), ADC4);
            char adc4[5] = {0,0,0,0,0}; // four digits and null terminator \0

            char buf_ADC6[MAX_BUFF] = {0};
            snprintf(buf_ADC6, sizeof(buf_ADC6), ADC6);
            char adc6[5] = {0,0,0,0,0}; // four digits and null terminator \0

            while(1)
            {
                fd = open(buf_ADC4, O_RDONLY | O_NONBLOCK);
                read(fd, adc4, 4); // reads 4 bytes from file FD to CH
                close(fd);

                fd = open(buf_ADC6, O_RDONLY | O_NONBLOCK);
                read(fd, adc6, 4); // reads 4 bytes from file FD to CH
                close(fd);

                printf("ADC4: %d\tADC6: %d\n", atoi(adc4), atoi(adc6));

                if( atoi(adc4) > 2280) // > 1 volt
                {
                    // setar filho1 para 5
                    setpriority(PRIO_PROCESS, pid1, 5);
                }
                else
                {
                    // setar filho 1 para 19
                    setpriority(PRIO_PROCESS, pid1, 19);
                }

                if( atoi(adc6) > 2280) // > 1 volt
                {
                    // setar filho2 para 5
                    setpriority(PRIO_PROCESS, pid2, 5);
                }
                else
                {
                    // setar filho2 para 19
                    setpriority(PRIO_PROCESS, pid2, 19);
                }

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
        while(1)
        {
            printf("DarthVader: my priority is %d\n", getpriority(PRIO_PROCESS, 0 ));
            usleep(1000000);
        }
    }
    else
    { // Process creation error

    }

    exit(0);
}
