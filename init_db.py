import sqlite3

def init_database():
	conn = sqlite3.connect('ime_dict.db')

	cursor = conn.cursor()

	create_table_sql = """
	CREATE TABLE IF NOT EXISTS pinyin_dict(
		id INTEGER PRIMARY KEY AUTOINCREMENT,
		pinyin TEXT NOT NULL,
		word TEXT NOT NULL,
		frequency INTEGER DEFAULT 0
	);
	"""

	cursor.execute(create_table_sql)

	test_data = [
		('nihao','你好',100),
		('zhongwen', '中文', 200),
		('kaifa', '开发', 50),
		('pingguo', '苹果', 300)
	]

	cursor.executemany(
		"INSERT INTO pinyin_dict(pinyin,word,frequency)VALUES(?,?,?)",
		test_data
	)

	conn.commit()
	conn.close()

if __name__ == "__main__":
	init_database()
