//=========================================================
// [typeSaveManager.h]
// 作成：2022/07/10
//
// 型を保存するｸﾗｽ。ﾃﾝﾌﾟﾚｰﾄを指定することで、型の生ポインタとSPの生成を行う
// 設計上、記録するためにはグローバル変数宣言しなければならない
//=========================================================

#include <DebugSystem/typeSaveManager.h>
#include <CoreSystem/File/cerealize.h>

//#ifdef BUILD_MODE

using namespace MySpace::Debug;

//--- 定数定義
#define TYPE_PATH		FORDER_DIR(data/.type)

//=========================================================

//==========================================================
// デストラクタ
//==========================================================
CTypeSaveManager::~CTypeSaveManager()
{
	Uninit();
}

//==========================================================
// 解放処理
//==========================================================
void CTypeSaveManager::Uninit()
{
	for (auto it = m_aStockType.begin(); it != m_aStockType.end(); ++it)
	{
		if ((*it).second)
			delete (*it).second;
	}
	m_aStockType.clear();
}

//==========================================================
// 一致するものがあるか
//==========================================================
bool CTypeSaveManager::IsType(std::string name) {
	if (auto it = m_aStockType.find(name); it != m_aStockType.end()) 
		return true;

	return false;
}

//==========================================================
// 格納されている型の名前からﾎﾟｲﾝﾀ取得
//==========================================================
CTypeSaveBase* CTypeSaveManager::GetTypeSave(std::string name)
{ 
	if (!m_aStockType.count(name))
		return nullptr;

	return m_aStockType[name]; 
}

//==========================================================
// 格納されている型の名前取得
//==========================================================
std::vector<std::string> CTypeSaveManager::GetTypeNameList()
{
	std::vector<std::string> list;
	auto it = m_aStockType.begin();
	for (; it != m_aStockType.end(); ++it)
	{
		list.push_back((*it).first);
	}
	return list;
}


//template <class T>
//T* CTypeSaveManager::GetTypeSave()
//{
//	auto it = m_aStockType.begin();
//	for (; it != m_aStockType.end(); ++it)
//	{
//		// キャスト
//		CTypeSave<T> save = dynamic_cast<T>(*it);
//		if (!save)continue;
//
//		return save.CreateType();
//	}
//	return nullptr;
//}

//template <class T>
//std::shared_ptr<T> CTypeSaveManager::MakeType()
//{
//	auto it = m_aStockType.begin();
//	for (; it != m_aStockType.end(); ++it)
//	{
//		// キャスト
//		CTypeSave<T> save = dynamic_cast<T>(*it);
//		if (!save)continue;
//		return save.MakeType();
//	}
//	return std::shared_ptr<T>;
//}

//#endif BUILD_MODE