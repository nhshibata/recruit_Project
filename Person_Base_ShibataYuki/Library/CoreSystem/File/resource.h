//=========================================================
//�쐬:2022/04/24
// �t�@�C���ǂݍ��� : ���N���X(����)
//=========================================================
#ifndef __RESOURCE_H__
#define __RESOURCE_H__

//--- �C���N���[�h��
#include <string>

class CResource
{
protected:
	//--- �����o�ϐ�
	std::string m_fileName;

public:
	//--- �����o�֐�
	CResource() {};
	~CResource() {};

	virtual bool Load(std::string fileName) = 0;
};

#endif // !__RESOURCE_H__