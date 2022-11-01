//=========================================================
//作成:2022/04/24
// ファイル読み込み : 基底クラス(抽象)
//=========================================================
#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <string>

class CResource
{
public:
	CResource() {};
	~CResource() {};

	virtual bool Load(std::string fileName) = 0;
	//virtual bool Save() = 0;

protected:
	std::string m_fileName;
};

#endif // !__RESOURCE_H__