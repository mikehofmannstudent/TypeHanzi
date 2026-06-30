import sqlite3

def search_pinyin(pinyin_in):
	conn = sqlite3.connect('ime_dict.db')
	cursor = conn.cursor()

	sql = "SELECT id,word,frequency FROM pinyin_dict WHERE pinyin LIKE ? ORDER BY frequency DESC"

	cursor.execute(sql,(pinyin_in + '%',))

	results = cursor.fetchall()

	conn.close()

	return results


def learn_word(word_id):
	conn = sqlite3.connect('ime_dict.db')

	cursor = conn.cursor()

	sql = "UPDATE pinyin_dict SET frequency = frequency + 1 WHERE id = ?"

	cursor.execute(sql,(word_id,))

	conn.commit()

	conn.close()


if __name__ == "__main__":
	print("=== 具备学习功能的输入法引擎 ===")
	while True:
		user_input = input("\n请输入拼音 (输入 q 退出): ").strip().lower()

		if user_input == 'q':
			break

		candidates = search_pinyin(user_input)

		if candidates:
			print(f"找到 {len(candidates)} 个候选词：")

			for idx,(w_id,word,freq) in enumerate(candidates):
				print(f"{idx + 1}. {word} (词频: {freq})")

			choice = input("请选择序号 (跳过请按回车): ")
			if choice.isdigit():
				choice_idx = int(choice) - 1

				if 0 <= choice_idx < len(candidates):
					selected_id = candidates[choice_idx][0]
					selected_word = candidates[choice_idx][1]

					learn_word(selected_id)
					print(f"『{selected_word}』权重已提升！下一次它会排得更靠前。")

		else:
			print(f"未找到拼音为 '{user_input}' 的词。")

	print("查询引擎已关闭。")

