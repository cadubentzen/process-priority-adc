#ifndef LCD_H
#define LCD_H

#define LCD_BUF_SIZE 64

char lcd_buf[LCD_BUF_SIZE];
int lcd_len;

int lcd_open(char* devpath);
void lcd_message(int lcd_fd, char* fmt, ...);
void lcd_clear_screen(int lcd_fd);
void lcd_go_home(int lcd_fd);
void lcd_clear_and_home(int lcd_fd);

#endif
