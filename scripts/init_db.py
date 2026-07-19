from pathlib import Path
from pypinyin import lazy_pinyin
import sqlite3

def init_database():
    SCRIPT_DIR = Path(__file__).resolve().parent
    PROJECT_ROOT = SCRIPT_DIR.parent
    DATA_DIR = PROJECT_ROOT / "data"

    # Create data/ if it doesn't exist
    DATA_DIR.mkdir(exist_ok=True)

    DB_PATH = DATA_DIR / "ime_dict.db"

    conn = sqlite3.connect(DB_PATH)
    cursor = conn.cursor()

    cursor.execute("""
        CREATE TABLE IF NOT EXISTS ime_dict (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            pinyin TEXT NOT NULL,
            word TEXT NOT NULL UNIQUE,
            frequency INTEGER DEFAULT 0
        )
    """)

    data_to_insert = []

    with open("all.txt", "r", encoding="utf-8") as f:
        for line in f:
            word = line.strip()

            if not word:
                continue

            pinyin = ''.join(lazy_pinyin(word))
            data_to_insert.append((pinyin, word))

    if data_to_insert:
        cursor.executemany(
            """
            INSERT OR IGNORE INTO ime_dict
            (pinyin, word)
            VALUES (?, ?)
            """,
            data_to_insert
        )

    conn.commit()
    conn.close()
    print(f"成功初始化数据库，并尝试插入 {len(data_to_insert)} 条数据到 ime_dict！")


def update_frequency(word, delta=1):
    conn = sqlite3.connect(DB_NAME)
    cursor = conn.cursor()

    cursor.execute("""
        UPDATE ime_dict
        SET frequency = frequency + ?
        WHERE word = ?
    """, (delta, word))

    conn.commit()
    
    if cursor.rowcount > 0:
        print(f"成功将词语 '{word}' 的 frequency 调整了 {delta}。")
    else:
        print(f"词库中没有找到词语 '{word}'。")
        
    conn.close()


if __name__ == "__main__":
    init_database()
