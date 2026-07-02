from pypinin import lazy_pinyin
import sqlite3

def init_database():
	conn = sqlite3.connect('ime_dict.db')

	cursor = conn.cursor()

	with open("all.txt","r",encoding="utf-8") as f:
		for line in f:
			word = line.strip()

			if not word:
				continue

			pinyin = ''.join(lazy_pinyin(word))

		cursor.executemany(
			"""
			INSERT OR IGNORE INTO pinyin_dict
			(pinyin, word)
			VALUES (?, ?)
			""",
			(pinyin, word)
		)

		conn.commit()
		conn.close()

if __name__ == "__main__":
	init_database()
