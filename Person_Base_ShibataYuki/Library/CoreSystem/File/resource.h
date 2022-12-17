//=========================================================
//作成:2022/04/24
// ファイル読み込み : 基底クラス(抽象)
//=========================================================
#ifndef __RESOURCE_H__
#define __RESOURCE_H__

//--- インクルード部
#include <string>

class CResource
{
protected:
	//--- メンバ変数
	std::string m_fileName;

public:
	//--- メンバ関数
	CResource() {};
	~CResource() {};

	virtual bool Load(std::string fileName) = 0;
};

#endif // !__RESOURCE_H__