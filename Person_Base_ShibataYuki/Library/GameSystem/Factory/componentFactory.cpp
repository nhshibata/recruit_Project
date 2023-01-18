//=========================================================
// �쐬:2022/04/22
// Factory���\�b�h
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Factory/componentFactory.h>

#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>
#include <GameSystem/components.h>
#include <DebugSystem/typeSaveManager.h>

using namespace MySpace::Game;
using namespace MySpace::Debug;

//==========================================================
// �R���X�g���N�^
//==========================================================
CComponentFactory::CComponentFactory()
{

}

//==========================================================
// �^�̖��O�擾
//==========================================================
std::vector<std::string> CComponentFactory::GetNameList()
{
	CreateComponentType();
	return CTypeSaveManager::Get()->GetTypeNameList();
}

//==========================================================
// �I�u�W�F�N�g�Ɍ^��������߰��Ă�ǉ�
//==========================================================
bool CComponentFactory::ObjSetComponent(CGameObject& obj, std::string name)
{
	if (auto type = CTypeSaveManager::Get()->GetTypeSave(name); type)
	{
		std::string setName = type->Set(&obj);
		if (setName != name)
		{
			return false;
		}
		return true;
	}
	return false;
}

//==========================================================
// ���߰��č쐬
//==========================================================
//std::shared_ptr<CComponent> CComponentFactory::CreateComponent(std::string name)
//{
//	CTypeSave<CComponent>* com = dynamic_cast<CTypeSave<CComponent>*>(CTypeSaveManager::Get()->GetTypeSave(name));
//	
//	return com->MakeType();
//}

//==========================================================
// ���߰��č쐬
//==========================================================
//template <class T>
//T* CComponentFactory::CreateComponent()
//{
//	return CTypeSaveManager::Get()->GetTypeSave<T>();
//}

//==========================================================
// ���߰��č쐬
//==========================================================
//template <class T>
//std::shared_ptr<T> CComponentFactory::MakeComponent()
//{
//	return CTypeSaveManager::Get()->MakeType<T>();
//}