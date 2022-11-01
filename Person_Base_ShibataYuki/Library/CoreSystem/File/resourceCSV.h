//=========================================================
//作成:2022/04/24
// CSV読み込み : 派生クラス
//=========================================================
#ifndef __RESOURCE_CSV_H__
#define __RESOURCE_CSV_H__

// --- インクルード部 ---
#include <CoreSystem/File/resource.h>
#include <vector>


// --- クラス定義 ---
class CResourceCSV :public CResource
{
private:
	using Cell = std::string;					// 1マスごとの区切り
	using Column = std::vector <std::string>;	// 1列（縦の並びが横に伸びている)
	using Row = std::vector<Column>;			// 1行（横に伸びる配列が縦に複数個

	using Array = std::vector<int>;
	using TwoArray = std::vector<Array>;

	// 読み込んだcsvﾌｧｲﾙのﾃﾞｰﾀ格納先
	Row m_grid;
public:
	CResourceCSV();
	~CResourceCSV();

	bool Load(std::string fileName);
	//	bool Save();

	std::string GetStr(int y, int x);
	int GetInt(int y, int x);
	float GetFloat(int y, int x);
	std::vector<std::vector<int>> GetintArray();
	std::vector<std::vector<int>> GetintArray(int start, int range);

};

#endif // !__RESOURCE_CSV_H__

