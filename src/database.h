#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h"
#include "candidate.h"

sqlite3 *db_open(const char *filename);

void db_close(sqlite3 *db);

int db_lookup(
    sqlite3 *db,
    const char *pinyin,
    Candidate candidates[],
    int max_candidates);

void db_increment_frequency(
    sqlite3 *db,
    int id);

#endif