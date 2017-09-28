#include <stdio.h>  // prinf()
#include <unistd.h> // fork()
#include <stdlib.h> // exit()
#include <sys/resource.h> // setpriority(), getpriority()
#include <time.h> // usleep()
#include <fcntl.h> // O_RDONLY
#include <gpio.h> // gpio functions

#define ADC4 "/sys/bus/iio/devices/iio:device0/in_voltage4_raw"
#define ADC6 "/sys/bus/iio/devices/iio:device0/in_voltage6_raw"
#define MAX_BUFF 64

#define GPIO_LED_SON1 68
#define GPIO_LED_SON2 67

int main(int argc, char **argv)
{
    printf("Priority Control with ADC running...\n");
    pid_t pid1, pid2;
	
    int i;
    int counter = 0;
    pid1 = fork();

    /* Setting up LEDs */
    gpio_export(GPIO_LED_SON1);
    gpio_set_dir(GPIO_LED_SON1, 1);
    gpio_export(GPIO_LED_SON2);
    gpio_set_dir(GPIO_LED_SON2, 1);

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
                read(fd, adc4, 4); // reads 4 bytes from file FD to adc4
                close(fd);

                fd = open(buf_ADC6, O_RDONLY | O_NONBLOCK);
                read(fd, adc6, 4); // reads 4 bytes from file FD to adc6
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
            goto child2;
        }
        else
        { // Process error during creation

        }
    }
    else if (pid1 == 0)
    { // Child process executes
        child1:
        printf("Son1: Noooooooooo!\n");
	i = 0;
        while(1)
        {
            printf("Son1: my priority is %d\n", getpriority(PRIO_PROCESS, 0 ));

            if(i%2 == 0)
            {
                gpio_set_value(GPIO_LED_SON1, 1);
            }
            else
            {
                gpio_set_value(GPIO_LED_SON1, 0);
            }
	    i++;
            usleep(1000000);
        }
        exit(0);

        child2:
        printf("Son2: Noooooooooo!\n");
	i = 0;
        while(1)
        {
            printf("Son2: my priority is %d\n", getpriority(PRIO_PROCESS, 0 ));

            if(i%2 == 0)
            {
                gpio_set_value(GPIO_LED_SON2, 1);
            }
            else
            {
                gpio_set_value(GPIO_LED_SON2, 0);
            }
	    i++;
            usleep(1000000);
        }
        exit(0);
    }
    else
    { // Process creation error

    }

    exit(0);
}
