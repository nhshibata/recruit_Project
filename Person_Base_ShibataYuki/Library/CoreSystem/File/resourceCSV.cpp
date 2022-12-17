//=========================================================
//�쐬:2022/04/24
// CSV�ǂݍ��� : �h���N���X
//=========================================================
#define _CRT_SECURE_NO_WARNINGS

// --- �C���N���[�h�� ---
#include <CoreSystem/File/resourceCSV.h>

// �R���X�g���N�^
CResourceCSV::CResourceCSV()
{
}

// �f�X�g���N�^
CResourceCSV::~CResourceCSV()
{
}

bool CResourceCSV::Load(std::string fileName)
{
	FILE* fp; // �t�@�C���|�C���^
	fp = fopen(fileName.c_str(), "rt");

	if (!fp)
	{	// �ǂݍ��ݎ��s
		return false;
	}

	// �t�@�C���T�C�Y�𒲂ׂ�
	size_t fileSize;
	fseek(fp, 0, SEEK_END);	// ̧�ُI�[�܂Ń|�C���^���ړ�
	fileSize = ftell(fp);	// ̧�ق̻��ނ��擾
	fseek(fp, 0, SEEK_SET);	// ̧�ق̐擪�܂ňړ�

	// ̧�ق̒��g��S�ċz���o��
	std::string text;
	text.resize(fileSize);	// �t�@�C���T�C�Y������
	fread(&text.front(), fileSize, 1, fp);

	// ̧�ق����
	fclose(fp);

	// ̧�ى��
	// 1�s����
	std::vector<std::string> lines;
	int startIndex = 0;
	int endIndex = 0;
	// find�̕Ԓl��npos�ƈ�v���Ȃ�����J��Ԃ�
	while (endIndex != std::string::npos)
	{
		// '\n'(���s�̕����L�����o�Ă���ʒu��T��)
		endIndex = (int)text.find("\n", startIndex);
		if (endIndex != std::string::npos)
		{
			// �T���J�n�ʒu���猩�����������܂ł̕�����؂�o��
			lines.push_back(text.substr(startIndex, endIndex - startIndex));
		}
		// ���̒T���ʒu���X�V
		startIndex = endIndex + 1;
	}

	// 1�Z������؂����f�[�^���i�[
	std::vector<std::string>::iterator lineIt = lines.begin();
	while (lineIt != lines.end())
	{
		Column column;
		startIndex = 0;
		endIndex = 0;
		// ��قǎ��o�����s��'.'�ŋ�؂��Ď��o��
		while (endIndex != std::string::npos)
		{
			// ','(���s�̕����L�����o�Ă���ʒu��T��)
			endIndex = (int)lineIt->find(",", startIndex);

			// npos�������Ă��Ȃ����
			if (endIndex != std::string::npos)
			{
				// �T���J�n�ʒu���猩�����������܂ł̕�����؂�o��
				column.push_back(lineIt->substr(startIndex, endIndex - startIndex));
			}
			else
			{
				column.push_back(lineIt->substr(startIndex));
			}
			// ���̒T���ʒu���X�V
			startIndex = endIndex + 1;
		}
		// �Z�����Ƃɋ�؂���(column)��S�̂��ް�(grid)�֒ǉ�
		m_grid.push_back(column);

		// ���̍s
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
	TwoArray intArray;		// int�񎟌��z��
	int y, x;
	intArray.resize(m_grid.size());
	y = x = 0;
	// �c��ő吔���Ⴂ����J��Ԃ�
	while (y < (int)m_grid.size())
	{
		// ����ő吔���Ⴂ����J��Ԃ�
		while (x < (int)m_grid[y].size())
		{
			intArray[y].push_back(atoi(m_grid[y][x].c_str()));
			++x;	// ����ړ�
		}
		++y;	// �c��ړ�
		x = 0;	// ���񏉊���
	}

	return intArray;
}

std::vector<std::vector<int>> CResourceCSV::GetintArray(int start, int range)
{
	TwoArray intArray;		// int�񎟌��z��
	int y, x;
	intArray.resize(range + 1);
	y = 0;
	x = 0;
	// �c��ő吔���Ⴂ����J��Ԃ�
	while (y < (int)m_grid.size())
	{
		// ����ő吔���Ⴂ����J��Ԃ�
		while (x < (int)m_grid[y].size())
		{
			intArray[y].push_back(atoi(m_grid[start + y][x].c_str()));
			++x;	// ����ړ�
		}
		++y;	// �c��ړ�
		x = 0;	// ���񏉊���
		--range;
		if (range <= 0)
		{
			break;
		}
	}

	return intArray;
}
