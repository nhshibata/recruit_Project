//=========================================================
// [typeSaveManager.h]
// �쐬�F2022/07/10
//
// �^��ۑ�����׽�B����ڰĂ��w�肷�邱�ƂŁA�^�̐��|�C���^��SP�̐������s��
// �݌v��A�L�^���邽�߂ɂ̓O���[�o���ϐ��錾���Ȃ���΂Ȃ�Ȃ�
//=========================================================

#include <DebugSystem/typeSaveManager.h>
#include <CoreSystem/File/cerealize.h>

//#ifdef BUILD_MODE

using namespace MySpace::Debug;

#define TYPE_PATH		FORDER_DIR(data/.type)


//=========================================================
CTypeSaveManager::~CTypeSaveManager()
{
	Uninit();
}
void CTypeSaveManager::Uninit()
{
	for (auto it = g_StockType.begin(); it != g_StockType.end(); ++it)
	{
		if ((*it).second)
			delete (*it).second;
	}
}
bool CTypeSaveManager::IsType(std::string name) {
	if (auto it = g_StockType.find(name); it != g_StockType.end()) 
		return true;

	return false;
}
std::vector<std::string> CTypeSaveManager::GetTypeNameList()
{
	std::vector<std::string> list;
	auto it = g_StockType.begin();
	for (; it != g_StockType.end(); ++it)
	{
		list.push_back((*it).first);
	}
	return list;
}
template <class T>
T* CTypeSaveManager::GetTypeSave()
{
	auto it = g_StockType.begin();
	for (; it != g_StockType.end(); ++it)
	{
		// �L���X�g
		CTypeSave<T> save = dynamic_cast<T>(*it);
		if (!save)continue;

		return save.CreateType();
	}
	return nullptr;
}

template <class T>
std::shared_ptr<T> CTypeSaveManager::MakeType()
{
	auto it = g_StockType.begin();
	for (; it != g_StockType.end(); ++it)
	{
		// �L���X�g
		CTypeSave<T> save = dynamic_cast<T>(*it);
		if (!save)continue;
		return save.MakeType();
	}
	return std::shared_ptr<T>;
}
CTypeSaveManager::StockSharedMap CTypeSaveManager::Convert()
{
	StockSharedMap ret;
	/*for (auto it = g_StockType.begin(); it != g_StockType.end(); ++it)
	{
		std::shared_ptr<CTypeSaveBase> sp = std::make_shared<CTypeSaveBase>((*it).second);
		ret.insert(StockSharedPair((*it).first, sp));
	}*/

	return ret;
}

//#endif BUILD_MODE