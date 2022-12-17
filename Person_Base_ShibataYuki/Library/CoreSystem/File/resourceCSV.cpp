//=========================================================
//作成:2022/04/24
// CSV読み込み : 派生クラス
//=========================================================
#define _CRT_SECURE_NO_WARNINGS

// --- インクルード部 ---
#include <CoreSystem/File/resourceCSV.h>

// コンストラクタ
CResourceCSV::CResourceCSV()
{
}

// デストラクタ
CResourceCSV::~CResourceCSV()
{
}

bool CResourceCSV::Load(std::string fileName)
{
	FILE* fp; // ファイルポインタ
	fp = fopen(fileName.c_str(), "rt");

	if (!fp)
	{	// 読み込み失敗
		return false;
	}

	// ファイルサイズを調べる
	size_t fileSize;
	fseek(fp, 0, SEEK_END);	// ﾌｧｲﾙ終端までポインタを移動
	fileSize = ftell(fp);	// ﾌｧｲﾙのｻｲｽﾞを取得
	fseek(fp, 0, SEEK_SET);	// ﾌｧｲﾙの先頭まで移動

	// ﾌｧｲﾙの中身を全て吸い出す
	std::string text;
	text.resize(fileSize);	// ファイルサイズ分調整
	fread(&text.front(), fileSize, 1, fp);

	// ﾌｧｲﾙを閉じる
	fclose(fp);

	// ﾌｧｲﾙ解析
	// 1行ずつ
	std::vector<std::string> lines;
	int startIndex = 0;
	int endIndex = 0;
	// findの返値がnposと一致しない限り繰り返す
	while (endIndex != std::string::npos)
	{
		// '\n'(改行の文字記号が出てくる位置を探索)
		endIndex = (int)text.find("\n", startIndex);
		if (endIndex != std::string::npos)
		{
			// 探索開始位置から見つかった部分までの文字を切り出し
			lines.push_back(text.substr(startIndex, endIndex - startIndex));
		}
		// 次の探索位置を更新
		startIndex = endIndex + 1;
	}

	// 1セルずつ区切ったデータを格納
	std::vector<std::string>::iterator lineIt = lines.begin();
	while (lineIt != lines.end())
	{
		Column column;
		startIndex = 0;
		endIndex = 0;
		// 先ほど取り出した行を'.'で区切って取り出す
		while (endIndex != std::string::npos)
		{
			// ','(改行の文字記号が出てくる位置を探索)
			endIndex = (int)lineIt->find(",", startIndex);

			// nposが入っていなければ
			if (endIndex != std::string::npos)
			{
				// 探索開始位置から見つかった部分までの文字を切り出し
				column.push_back(lineIt->substr(startIndex, endIndex - startIndex));
			}
			else
			{
				column.push_back(lineIt->substr(startIndex));
			}
			// 次の探索位置を更新
			startIndex = endIndex + 1;
		}
		// セルごとに区切った(column)を全体のﾃﾞｰﾀ(grid)へ追加
		m_grid.push_back(column);

		// 次の行
		++lineIt;
	}
	return true;

}
// 
std::string CResourceCSV::GetStr(int y, int x)
{
	/*if ((int)m_grid.size() <= y || (int)m_grid[0].size() < x)
	{
		return "\n";
	}*/
	return m_grid[y][x];
}
int CResourceCSV::GetInt(int y, int x)
{
	return atoi(m_grid[y][x].c_str());
}
float CResourceCSV::GetFloat(int y, int x)
{
	return static_cast<float>(atof(m_grid[y][x].c_str()));
}
std::vector<std::vector<int>> CResourceCSV::GetintArray()
{
	TwoArray intArray;		// int二次元配列
	int y, x;
	intArray.resize(m_grid.size());
	y = x = 0;
	// 縦列最大数より低い限り繰り返す
	while (y < (int)m_grid.size())
	{
		// 横列最大数より低い限り繰り返す
		while (x < (int)m_grid[y].size())
		{
			intArray[y].push_back(atoi(m_grid[y][x].c_str()));
			++x;	// 横列移動
		}
		++y;	// 縦列移動
		x = 0;	// 横列初期化
	}

	return intArray;
}

std::vector<std::vector<int>> CResourceCSV::GetintArray(int start, int range)
{
	TwoArray intArray;		// int二次元配列
	int y, x;
	intArray.resize(range + 1);
	y = 0;
	x = 0;
	// 縦列最大数より低い限り繰り返す
	while (y < (int)m_grid.size())
	{
		// 横列最大数より低い限り繰り返す
		while (x < (int)m_grid[y].size())
		{
			intArray[y].push_back(atoi(m_grid[start + y][x].c_str()));
			++x;	// 横列移動
		}
		++y;	// 縦列移動
		x = 0;	// 横列初期化
		--range;
		if (range <= 0)
		{
			break;
		}
	}

	return intArray;
}
