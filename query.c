#include <stdio.h>
#include <sqlite3.h>
#include <string.h>

int main() {
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char pinyin_input[100];
	char pinyin_query[110];

	if (sqlite3_open_v2("ime_dict.db",&db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
		printf("无法打开数据库\n");
		return 1;
	}

	printf("请输入拼音：");
	scanf("%99s",pinyin_input);

	sprintf(pinyin_query,"%s%%",pinyin_input);

	const char *sql = "SELECT id,word,frequency FROM pinyin_dict WHERE pinyin LIKE ? ORDER BY frequency DESC";

	if (sqlite3_prepare_v2(db,sql,-1,&stmt,NULL) != SQLITE_OK) {
		printf("SQL预处理失败:%s\n",sqlite3_errmsg(db));
	}

	sqlite3_bind_text(stmt, 1, pinyin_query, -1, SQLITE_STATIC);

	printf("\n查询结果如下:\n");
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		int id = sqlite3_column_int(stmt,0);
		const unsigned char *word = sqlite3_column_text(stmt,1);
		int freq = sqlite3_column_int(stmt,2);

		printf("[%d] %s (频率: %d)\n", id, word, freq);
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);

	return 0;

}

