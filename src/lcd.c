#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "lcd.h"

int lcd_open(char *devpath) {
    int lcd_fd = open(devpath, O_WRONLY);
    if (lcd_fd < 0) {
        perror("lcd_open: ");
        exit(1);
    }
    return lcd_fd;
}

void lcd_message(int lcd_fd, char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    lcd_len = vsnprintf(lcd_buf, sizeof(lcd_buf), 
                       fmt, vl);
    va_end(vl);
    write(lcd_fd, lcd_buf, lcd_len);
}

void lcd_clear_screen(int lcd_fd) {
    lcd_message(lcd_fd, "\033[2J");
}

void lcd_go_home(int lcd_fd) {
    lcd_message(lcd_fd, "\033[H");
}

void lcd_clear_and_home(int lcd_fd) {
    lcd_clear_screen(lcd_fd);
    lcd_go_home(lcd_fd);
}
