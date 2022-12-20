//=========================================================
// [CGameObject.cpp]
//---------------------------------------------------------
// �쐬:2022/04/19 (�Ηj��)
// �X�V:2022/05/11 �^�O����v����I�u�W�F�N�g��S�Ď擾����֐��쐬
//			�ۑ�: ����̃R���|�[�l���g���g���ꍇ�Amap�͓K�؂ł͂Ȃ�
// �X�V:2022/11/14 �R�s�[�R���X�g���N�^�������A����̃R���|�[�l���g�𐶐�����悤�ɕύX
//---------------------------------------------------------
// �Q�[���I�u�W�F�N�g : ���N���X
// 
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Transform/transform.h>

#include <ImGui/imgui.h>

#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Factory/componentFactory.h>

using namespace MySpace::SceneManager;
using namespace MySpace::Game;

//--- �ÓI�����o�ϐ�
//CGameObject::GAMEOBJECT CGameObject::m_pObjects;

#ifdef _DEBUG
// �I�u�W�F�N�g�̖��O��tag���m�肽������
static inline void CheckObj(CGameObject* obj)
{
	auto name = obj->GetName();
	auto tag = obj->GetTagPtr()->GetTag();
}
#endif // _DEBUG

// �R���X�g���N�^
CGameObject::CGameObject()
	:m_eState(E_ObjectState::ACTIVE)
{
	m_pTag = std::make_shared<CTag>();
	m_pLayer = std::make_shared<CLayer>();
}
CGameObject::CGameObject(std::string name)
	:m_eState(E_ObjectState::ACTIVE)
{
	SetName(name);
	m_pTag = std::make_shared<CTag>();
	m_pLayer = std::make_shared<CLayer>();
}
// �R�s�[�R���X�g���N�^
CGameObject::CGameObject(const CGameObject & object)
{
#if BUILD_MODE
	this->m_objName = object.m_objName + std::to_string(++m_nCopyCnt);
#else
	this->m_objName = object.m_objName;
#endif // BUILD_MODE

	this->m_pTransform = object.m_pTransform;
	this->m_eState = object.m_eState;
	this->m_pLayer = object.m_pLayer;
	this->m_pTag = object.m_pTag;
	//this->m_pComponent = object.m_pComponent;
	// ���߰��Ă̖��O���瓯�����߰��Ă�ǉ�
	// TODO: �e���߰��Ă̒l�̺�߰�͍s���Ȃ�
	for (auto & component : object.m_pComponent)
	{
		// �R���|�[�l���g��ۑ����Ă���׽�ɒǉ����Ă��炤
		if (CComponentFactory::ObjSetComponent(*this, component->GetName()))
		{
			// �Ō���ɒǉ�����Ă���R���|�[�l���g���擾
			//auto com = this->GetComponentList().back();
			//com->Init();
		}
	}
}
CGameObject::~CGameObject()
{
	for (auto & com : m_pComponent)
		com.reset();
	m_pComponent.clear();
}
void CGameObject::OnLoad()
{
	m_pTransform = GetComponent<CTransform>();

	// �R���|�[�l���g�̃��[�h�Ăяo��
	for (COMPONENT::iterator it = m_pComponent.begin(); it != m_pComponent.end(); ++it)
	{
		(*it)->OnLoad();
	}
}
// *���������O�ɌĂяo������
// *�I�u�W�F�N�g��R���|�[�l���g��ǉ�����Ȃ炱��
// *���O��^�O�A���C���[�̐ݒ�������ōs��
void CGameObject::Awake()
{
	// Awake�ͺ��߰��Ēǉ����ɌĂяo�����d�l
	if (m_pTransform.lock())
		return;

	m_pTransform = AddComponent<CTransform>();
}
// *������
// *�I�u�W�F�N�g��R���|�[�l���g��T���Ȃ炱��
void CGameObject::Init()
{
	// �R���|�[�l���g�̏�����
	for (auto & com : m_pComponent)
	{
		com->Init();
	}
}
// �R���|�[�l���g�̍X�V
// ���̍X�V���ŃI�u�W�F�N�g���j���͂���Ȃ�
void CGameObject::Update()
{
	for (auto & com : m_pComponent)
	{
	
#if _DEBUG
		auto name = com->GetName();
#endif // _DEBUG

		if (!com->IsActive())
		{
			continue;
		}

		com->Update();
	}
}
// �Q�[���I�u�W�F�N�g���Ƃ̒x���X�V
void CGameObject::LateUpdate()
{
	for (auto & com : m_pComponent)
	{
		if (!com->IsActive())
		{
			continue;
		}

		com->LateUpdate();
	}
}
// ��莞�Ԃ̍X�V
void CGameObject::FixedUpdate()
{
	for (auto & com : m_pComponent)
	{
		if (!com->IsActive())
		{
			continue;
		}

		com->FixedUpdate();
	}
}
// �R���|�[�l���g�̒ǉ�
std::shared_ptr<CComponent> CGameObject::AddComponent(std::shared_ptr<CComponent> com)
{
	m_pComponent.push_back(com);
	com->SetOwner(GetPtr()); 

	return com; 
}
// �R���|�[�l���g�̔j��
template <class T>
bool CGameObject::RemoveComponent(std::string comName)
{
	COMPONENT::iterator it;
	for (it = m_pComponent.begin(); it != m_pComponent.end(); ++it)
	{
		if (typeid(T).name == (*it)->GetName())
		{
			m_pComponent.erase(it);
			return true;
		}
	}
	return false;
}
// �R���|�[�l���g�j��
bool CGameObject::RemoveComponent(std::weak_ptr<CComponent> com)
{
	COMPONENT::iterator it;
	for (it = m_pComponent.begin(); it != m_pComponent.end(); ++it)
	{
		if ((*it) == com.lock())
		{
			m_pComponent.erase(it);
			return true;
		}
	}
	return false;
}
// inline?
void CGameObject::SetState(const E_ObjectState state)
{
	m_eState = state;
}
// �^�O�̈ړ�
// �����ł��̂͊ԈႢ?
void CGameObject::SetTag(const std::string tag) 
{ 
	if(auto scene = GetScene(); scene)
		scene->GetObjManager()->TagMove(tag, GetPtr());

	m_pTag->SetTag(tag);
};
// �Փ�
void CGameObject::OnCollisionEnter(CGameObject* obj)
{
#ifdef _DEBUG
	CheckObj(obj);
#endif // _DEBUG

	for (COMPONENT::iterator it = m_pComponent.begin(); it != m_pComponent.end(); ++it)
	{
		(*it)->OnCollisionEnter(obj);
	}
}
void CGameObject::OnCollisionStay(CGameObject* obj)
{
	COMPONENT::iterator it = m_pComponent.begin();
	for (; it != m_pComponent.end(); ++it)
	{
		(*it)->OnCollisionStay(obj);
	}
}
void CGameObject::OnCollisionExit(CGameObject* obj)
{
	COMPONENT::iterator it = m_pComponent.begin();
	for (; it != m_pComponent.end(); ++it)
	{
		(*it)->OnCollisionExit(obj);
	}
}
void CGameObject::OnTriggerEnter(CGameObject* obj)
{
	COMPONENT::iterator it = m_pComponent.begin();
	for (; it != m_pComponent.end(); ++it)
	{
		(*it)->OnTriggerEnter(obj);
	}
}
void CGameObject::OnTriggerStay(CGameObject* obj)
{
	COMPONENT::iterator it = m_pComponent.begin();
	for (; it != m_pComponent.end(); ++it)
	{
		(*it)->OnTriggerStay(obj);
	}
}
void CGameObject::OnTriggerExit(CGameObject* obj)
{
	COMPONENT::iterator it = m_pComponent.begin();
	for (; it != m_pComponent.end(); ++it)
	{
		(*it)->OnTriggerExit(obj);
	}
}
//--- �ÓI�����o�֐�
std::weak_ptr<CGameObject> CGameObject::FindGameObject(std::string name)
{
	for (auto & scene : CSceneManager::Get()->GetAllScene()) 
	{
		auto obj = scene->GetObjManager()->FindGameObj(name.c_str());
		if (obj.lock()) 
		{
			return obj;
		}
	}
	return std::shared_ptr<CGameObject>();
}
std::weak_ptr<CGameObject> CGameObject::FindGameObjectWithTag(std::string tag)
{
	for (auto & scene : CSceneManager::Get()->GetAllScene())
	{
		auto obj = scene->GetObjManager()->FindGameObjWithTag(tag);
		if (obj.lock())
		{
			return obj;
		}
	}
	return std::shared_ptr<CGameObject>();
}
std::list<std::weak_ptr<CGameObject>> CGameObject::FindGameObjectsWithTag(std::string tag)
{
	std::list<std::weak_ptr<CGameObject>> ret;
	for (auto & scene : CSceneManager::Get()->GetAllScene())
	{
		auto objs = scene->GetObjManager()->FindGameObjctsWithTag(tag);
		// �i�[
		for (auto & obj : objs)
		{
			ret.push_back(obj);
		}
	}
	return ret;
}
std::weak_ptr<CGameObject> CGameObject::FindGameObjectWithTag(CTag tag)
{
	for (auto & scene : CSceneManager::Get()->GetAllScene())
	{
		auto obj = scene->GetObjManager()->FindGameObjWithTag(tag);
		if (obj.lock())
		{
			return obj;
		}
	}
	return std::shared_ptr<CGameObject>();
}
std::list<std::weak_ptr<CGameObject>> CGameObject::FindGameObjectsWithTag(CTag tag)
{
	std::list<std::weak_ptr<CGameObject>> retList;
	for (auto & scene : CSceneManager::Get()->GetAllScene())
	{
		auto obj = scene->GetObjManager()->FindGameObjWithTag(tag);
		if (obj.lock())
		{
			retList.push_back(obj);
		}
	}
	return std::list<std::weak_ptr<CGameObject>>();
}
std::weak_ptr<CGameObject> CGameObject::CreateObject(CGameObject* pObj)
{
#ifdef _DEBUG
	auto scene = CSceneManager::Get()->GetActiveScene();
#endif // _DEBUG
	
	// ��߰�ł͂Ȃ�����
	if (!pObj) 
	{
		return CSceneManager::Get()->GetActiveScene()->GetObjManager()->CreateGameObject();
	}
	return CSceneManager::Get()->GetActiveScene()->GetObjManager()->CreateGameObject(pObj);
}
// ��邾���ŊǗ��͎󂯎�������ɈϔC
std::shared_ptr<CGameObject> CGameObject::CreateDebugObject(std::shared_ptr<CGameObject> pObj)
{
	// ���g��weakPtr��n��
	pObj.get()->SetPtr(pObj);

	// ������
	pObj.get()->Awake();	// ����OnCreate�ȋC������
	pObj.get()->Init();
	return pObj;
}
void CGameObject::Destroy(std::weak_ptr<CGameObject> pObj)
{
	// �j����ԂɕύX
	// manager�X�V���̊m�F�Ŕj�������
	pObj.lock()->SetState(E_ObjectState::DESTROY);
	/*for (auto & scene : CSceneManager::Get()->GetAllScene())
	{
		if (scene->GetObjManager()->DestroyObject(pObj))
		{
			return;
		}
	}*/
}
void CGameObject::DontDestroy(std::weak_ptr<CGameObject> pObj)
{
	CSceneManager::Get()->GetActiveScene()->GetObjManager()->DontDestroy(pObj);
}

#ifdef BUILD_MODE

void CGameObject::ImGuiDebug()
{
	static const char* szState[CGameObject::MAX_OBJECT_STATE] = {
		"ACTIVE",				// �X�V���
		"WAIT",				// �ҋ@
		"DESTROY",			// �폜
		"TAKEOVER",			// �����p���i�g���܂킵)
		"STOP",				// �f�o�b�O?
	};

	if (ImGui::BeginMenuBar()) 
	{
		if (ImGui::BeginMenu(u8"State"))
		{
			for (int state = 0; state < E_ObjectState::MAX_OBJECT_STATE; ++state)
			{
				if (ImGui::MenuItem(szState[state])) {
					m_eState = (E_ObjectState)state;
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::BeginMenuBar()) 
	{
		if (ImGui::BeginMenu(u8"tag"))
		{
			auto tagList = CTagManager::Get()->GetNameList();
			for (int state = 0; state < static_cast<int>(tagList.size()); ++state)
			{
				auto tagName = tagList[state].c_str();
				if (ImGui::MenuItem(tagName)) {
					GetTagPtr()->SetTag(tagName);
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	
	char name[256] = "";
	strcpy_s(name, GetName().c_str());
	ImGui::InputText(u8"*���O:", name, 256);
	SetName(name);
	ImGui::SameLine();
	ImGui::Text(u8"*Tag:%s", GetTagPtr()->GetTag().c_str());
	ImGui::SameLine();
	if (ImGui::BeginCombo(u8"*Layer:%d", "1"))
	{
		// TODO: �r��
		for (int cnt = 0; cnt < 5; cnt++)
		{
			auto label = std::to_string(cnt).c_str(); 
			
			if (bool bg = ImGui::Selectable(label, &bg) ; bg)
			{
				GetLayerPtr()->SetLayer(cnt);
			}
		}
		
		ImGui::EndCombo();
	}
	ImGui::Text(u8"*���߰��Đ�:%d", GetComponentList().size());
	ImGui::SameLine();
	ImGui::Text(u8"*state:%s", szState[m_eState]);
	//ImGui::RadioButton("state", &m_eState, E_ObjectState::ACTIVE);
}

#endif // BUILD_MODE
