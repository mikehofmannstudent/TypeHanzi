#include <sqlite3.h>
#include <stdio.h>

int main() {
	sqlite3* db;
	int rc;

	rc = sqlite3_open("ime_dict.db",&db);

	if (rc) {
		fprintf(stderr,"无法打开数据库:%s\n",sqlite3_errmsg(db));
		return 1;
	}

	else {
		printf("成功连接到数据库: ime_dict.db\n");
	}

	return 0;	
}
