#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

#include "sqlite3.h"

#define MAX_CANDIDATES 10
#define MAX_INPUT 100

static struct termios orig_termios;

typedef struct {
    int id;
    char word[64];
    int freq;
} Candidate;

void disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;

    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void clear_screen()
{
    printf("\033[2J");
    printf("\033[H");
}

void cleanup(int sig)
{
    disable_raw_mode();
    exit(sig);
}

int load_candidates(sqlite3_stmt *stmt, Candidate candidates[])
{
	int count = 0;

	while (sqlite3_step(stmt) == SQLITE_ROW && count < 10)
	{
		candidates[count].id =
			sqlite3_column_int(stmt, 0);

		strcpy(candidates[count].word,
			(const char *)sqlite3_column_text(stmt, 1));

		candidates[count].freq =
			sqlite3_column_int(stmt, 2);

		count++;
	}

	printf("\nCandidates:\n");

	for (int i = 0; i < count; i++)
	{
		printf("%d. %s (%d)\n",
			i + 1,
			candidates[i].word,
			candidates[i].freq);
	}

	return count;
}

int main(void)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;

	Candidate candidates[MAX_CANDIDATES];

    char input[100] = "";
    int len = 0;

    if (sqlite3_open("data/ime_dict.db", &db) != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    const char *search_sql =
        "SELECT id, word, frequency "
        "FROM pinyin_dict "
        "WHERE pinyin LIKE ? "
        "ORDER BY frequency DESC;";

    if (sqlite3_prepare_v2(db, search_sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Prepare failed: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

	signal(SIGINT, cleanup);   // Ctrl+C
	signal(SIGTERM, cleanup);  // kill
	signal(SIGSEGV, cleanup);  // segmentation fault

    enable_raw_mode();

    while (1)
    {
        clear_screen();

        printf("=== TypeHanzi ===\n");
        printf("ESC to quit\n\n");

        printf("Pinyin: %s\n\n", input);

        char query[110];
        sprintf(query, "%s%%", input);

        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
        sqlite3_bind_text(stmt, 1, query, -1, SQLITE_TRANSIENT);

		int candidate_count = load_candidates(stmt, candidates);

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
			if (candidate_count == 0)
                continue;
			
			disable_raw_mode();

			int choice;
			printf("\nSelect candidate (1-%d): ", candidate_count);
			scanf("%d", &choice);

			enable_raw_mode();

			if (choice >= 1 && choice <= candidate_count)
			{
				int id = candidates[choice - 1].id;

				char sql[128];

				snprintf(sql,
                         sizeof(sql),
                         "UPDATE pinyin_dict "
                         "SET frequency = frequency + 1 "
                         "WHERE id = %d;",
                         id);
				
				sqlite3_exec(db,
                             sql,
                             NULL,
                             NULL,
                             NULL);

				printf("Selected: %s\n", candidates[choice - 1].word);

				getchar();

				input[0] = '\0';
                len = 0;
			}

			enable_raw_mode();
		}
		else if (c >= 32 && c <= 126)
        {
            if (len < MAX_INPUT - 1)
            {
                input[len++] = c;
                input[len] = '\0';
            }
        }
    }

    disable_raw_mode();

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}