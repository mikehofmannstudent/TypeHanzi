#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "terminal.h"

static struct termios orig_termios;
static int initialized = 0;

void terminal_init(void)
{
    if (!initialized)
    {
        tcgetattr(STDIN_FILENO, &orig_termios);
        atexit(terminal_shutdown);
        initialized = 1;
    }

    struct termios raw = orig_termios;

    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void terminal_shutdown()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void terminal_clear()
{
    printf("\033[2J");
    printf("\033[H");
}