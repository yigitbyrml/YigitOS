#include "kernel.h"
#include <stddef.h>
#include <stdint.h>
#include "idt/idt.h"
#include "io/io.h"

uint16_t* video_memory = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t print_char(char word,char colour){
    return (colour << 8) | word;
}

void terminal_putchar(int x, int y, char c, char colour)
{
    video_memory[(y * VGA_WIDTH) + x] = print_char(c, colour);
}
void terminal_writechar(char c, char colour)
{
    if (c == '\n')
    {
        terminal_row += 1;
        terminal_col = 0;
        return;
    }

    terminal_putchar(terminal_col, terminal_row, c, colour);
    terminal_col += 1;
    if (terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row += 1;
    }
}

void terminal_initialize()
{
    video_memory = (uint16_t*)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_putchar(x, y, ' ', 0);
        }
    }   
}

size_t strlen(const char* str)
{
    size_t len = 0;
    while(str[len])
    {
        len++;
    }

    return len;
}

void print(const char* str)
{
    size_t len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        terminal_writechar(str[i], 15);
    }
}


void kernel_main()
{
    terminal_initialize();
    print("Hello world!\ntest");

    // Initialize the interrupt descriptor table
    idt_init();
    outb(0x60, 0xff);
}