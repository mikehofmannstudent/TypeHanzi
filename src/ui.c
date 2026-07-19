#include <stdio.h>

#include "ui.h"

void draw_screen(
    const char *input,
    Candidate candidates[],
    int count)
{
    printf("=== TypeHanzi ===\n");
    printf("ESC to quit\n\n");

    printf("Pinyin: %s\n\n", input);

    printf("Candidates:\n");

    for (int i = 0; i < count; i++)
    {
        printf("%d. %s (%d)\n",
               i + 1,
               candidates[i].word,
               candidates[i].freq);
    }
}