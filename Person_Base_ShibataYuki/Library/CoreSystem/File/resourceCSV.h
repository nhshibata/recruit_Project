//=========================================================
//�쐬:2022/04/24
// CSV�ǂݍ��� : �h���N���X
//=========================================================
#ifndef __RESOURCE_CSV_H__
#define __RESOURCE_CSV_H__

// --- �C���N���[�h�� ---
#include <CoreSystem/File/resource.h>
#include <vector>


// --- �N���X��` ---
class CResourceCSV :public CResource
{
private:
	using Cell = std::string;					// 1�}�X���Ƃ̋�؂�
	using Column = std::vector <std::string>;	// 1��i�c�̕��т����ɐL�тĂ���)
	using Row = std::vector<Column>;			// 1�s�i���ɐL�т�z�񂪏c�ɕ�����

	using Array = std::vector<int>;
	using TwoArray = std::vector<Array>;

	// �ǂݍ���csv̧�ق��ް��i�[��
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

