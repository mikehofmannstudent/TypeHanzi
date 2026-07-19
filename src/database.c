#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"

sqlite3 *db_open(const char *filename) 
{
    sqlite3 *db = NULL;
    

    if (sqlite3_open(filename, &db) != SQLITE_OK)
    {
        fprintf(stderr, "%s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    return db;
}


void db_close(sqlite3 *db) 
{
    sqlite3_close(db);
}

int db_lookup(
    sqlite3 *db,
    const char *pinyin,
    Candidate candidates[],
    int max_candidates
){
    sqlite3_stmt *stmt;

    const char *sql =
        "SELECT id, word, frequency "
        "FROM ime_dict "
        "WHERE pinyin LIKE ? "
        "ORDER BY frequency DESC;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Prepare failed: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    char query[110];
    snprintf(query, sizeof(query), "%s%%", pinyin);

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
    sqlite3_bind_text(stmt, 1, query, -1, SQLITE_TRANSIENT);

    int count = 0;

    while (
        sqlite3_step(stmt) == SQLITE_ROW && 
        count < max_candidates
    ){
		candidates[count].id =
			sqlite3_column_int(stmt, 0);

		strcpy(candidates[count].word,
			(const char *)sqlite3_column_text(stmt, 1));

		candidates[count].freq =
			sqlite3_column_int(stmt, 2);

		count++;
	}
	
    sqlite3_finalize(stmt);

	return count;
}

void db_increment_frequency(
    sqlite3 *db,
    int id)
{
    char sql[128];

    snprintf(sql,
             sizeof(sql),
             "UPDATE ime_dict "
             "SET frequency = frequency + 1 "
             "WHERE id = %d;",
             id);
    
    sqlite3_exec(db,
                 sql,
                 NULL,
                 NULL,
                 NULL);
}