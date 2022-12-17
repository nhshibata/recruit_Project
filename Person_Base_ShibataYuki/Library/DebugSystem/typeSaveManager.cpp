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
	for (auto it = m_aStockType.begin(); it != m_aStockType.end(); ++it)
	{
		if ((*it).second)
			delete (*it).second;
	}
	m_aStockType.clear();
}
bool CTypeSaveManager::IsType(std::string name) {
	if (auto it = m_aStockType.find(name); it != m_aStockType.end()) 
		return true;

	return false;
}
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
template <class T>
T* CTypeSaveManager::GetTypeSave()
{
	auto it = m_aStockType.begin();
	for (; it != m_aStockType.end(); ++it)
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
	auto it = m_aStockType.begin();
	for (; it != m_aStockType.end(); ++it)
	{
		// �L���X�g
		CTypeSave<T> save = dynamic_cast<T>(*it);
		if (!save)continue;
		return save.MakeType();
	}
	return std::shared_ptr<T>;
}

//#endif BUILD_MODE