#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "database.h"
#include "terminal.h"
#include "ui.h"

void cleanup(int sig)
{
    terminal_shutdown();
    exit(sig);
}

int main(void)
{
    sqlite3 *db = db_open("data/ime_dict.db");

    Candidate candidates[MAX_CANDIDATES];

    char input[100] = "";
    int len = 0;

    signal(SIGINT, cleanup);   // Ctrl+C
	signal(SIGTERM, cleanup);  // kill
	signal(SIGSEGV, cleanup);  // segmentation fault

    terminal_init();

    while (1)
    {
        terminal_clear();

        int count =
            db_lookup(
                db,
                input,
                candidates,
                MAX_CANDIDATES);

        draw_screen(
            input,
            candidates,
            count);

        char c = getchar();

        if (c == 27)   // ESC
            break;

        else if (c == 127 || c == '\b')
        {
            if (len > 0)
            {
                len--;
                input[len] = '\0';
            }
        }
		else if (c == '\n' || c == '\r')
		{
			if (count == 0)
                continue;
			
			terminal_shutdown();

			int choice;
			printf("\nSelect candidate (1-%d): ", count);
			scanf("%d", &choice);

			terminal_init();

			if (choice >= 1 && choice <= count)
			{
				int id = candidates[choice - 1].id;

				db_increment_frequency(db, id);

				printf("Selected: %s\n", candidates[choice - 1].word);

				getchar();

				input[0] = '\0';
                len = 0;
			}
		}
		else if (c >= 32 && c <= 126)
        {
            if (len < sizeof(input)-1)
            {
                input[len++] = c;
                input[len] = '\0';
            }
        }
    }

    terminal_shutdown();

    db_close(db);

    return 0;
}