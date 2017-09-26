#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include <gpio.h>

#ifdef LCD_DISPLAY
    #include <lcd.h>
#endif

#define GPIO_LED    67
#define GPIO_BUTTON 68

int main(int argc, char **argv)
{
    int button_fd;
    struct pollfd pfd;
    char buf[MAX_BUF];
    int rc;
    int len;
    struct timeval start, end;
    int time_mili;
    int game_on = 0;
    int button_value;
#ifdef LCD_DISPLAY
    int lcd_fd = lcd_open("/dev/lcd0");

    lcd_clear_and_home(lcd_fd);

    lcd_message(lcd_fd, "Welcome! #UFRN\n"
                        "Press button.\n");
#endif

    /* Initialize random seed */
    srand(time(NULL));

    /* Setting up LED */
    gpio_export(GPIO_LED);
    gpio_set_dir(GPIO_LED, 1);

    /* Setting up button */
    gpio_export(GPIO_BUTTON);
    gpio_set_dir(GPIO_BUTTON, 0);

    /* Start of program. There are two flows, controlled by game_on.
       
       The first flow is wait for user to press and release button to
       then start the second flow, which is to turn on LED and wait for
       user to react.
     */
    while(1) {
        /* Turn on LED */
        gpio_set_value(GPIO_LED, 1);

        /* Start counting time */
        if (game_on) 
            gettimeofday(&start, NULL);

        /* Wait for user to press button */
        do {
            gpio_get_value(GPIO_BUTTON, &button_value);
        } while(button_value);

        if (game_on) {
            /* Get the time user responded */
            gettimeofday(&end, NULL);
            time_mili = (end.tv_sec * 1000 + end.tv_usec / 1000)
                        - (start.tv_sec * 1000 + start.tv_usec / 1000);
        #ifdef LCD_DISPLAY
            lcd_clear_and_home(lcd_fd);
            
            lcd_message(lcd_fd, 
                        "Response\n"
                        "    Time: %.1fs\n"
                        "Press again.", 
                        time_mili*0.001);
        #endif
            printf("Time elapsed: %.1fs", time_mili*0.001);
        }
        
        printf("\nButton pressed!\n");

        /* Wait for user to release button */
        do {
            gpio_get_value(GPIO_BUTTON, &button_value);
        } while(!button_value);

        /* Turn off LED */
        gpio_set_value(GPIO_LED, 0);

        if (!game_on) { 
        #ifdef LCD_DISPLAY
            lcd_clear_and_home(lcd_fd);
            lcd_message(lcd_fd, "Wait for it.."); 
        #endif
            /* Wait randomly between 0 and 3s to turn on LED */
            printf("Wait for it..\n");
            usleep(rand()%3000000);
            printf("PRESS OR DIE !!!\n");
        #ifdef LCD_DISPLAY
            lcd_clear_and_home(lcd_fd);
            lcd_message(lcd_fd, "PRESS OR DIE !!!");
        #endif
        }

        game_on = game_on ? 0 : 1;
    }

    exit(0);
}
