//=========================================================
// �Q�[���I�u�W�F�N�g : ���N���X
//---------------------------------------------------------
// �쐬:2022/04/19 (�Ηj��)
// �X�V:2022/05/11 �^�O����v����I�u�W�F�N�g��S�Ď擾����֐��쐬
//			�ۑ�: ����̃R���|�[�l���g���g���ꍇ�Amap�͓K�؂ł͂Ȃ�
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Transform/transform.h>

#include <ImGui/imgui.h>

#include <GameSystem/Manager/sceneManager.h>
using namespace MySpace::SceneManager;
using namespace MySpace::Game;

//--- �ÓI�����o�ϐ�
//CGameObject::GAMEOBJECT CGameObject::m_pObjects;

#ifdef _DEBUG

static inline void CheckObj(CGameObject* obj)
{
	auto name = obj->GetName();
	auto tag = obj->GetTagPtr()->GetTag();
}
#endif // _DEBUG

[[maybe_unused]] int g_nObjNum;

// �R���X�g���N�^
CGameObject::CGameObject()
	:m_eState(E_ObjectState::ACTIVE)
{
	m_Tag = std::make_shared<CTag>();
	m_Layer = std::make_shared<CLayer>();
}
CGameObject::CGameObject(std::string name)
	:m_eState(E_ObjectState::ACTIVE)
{
	SetName(name);
	m_Tag = std::make_shared<CTag>();
	m_Layer = std::make_shared<CLayer>();
}
// �R�s�[�R���X�g���N�^
CGameObject::CGameObject(const CGameObject & object)
{
	this->m_Transform = object.m_Transform;
	this->m_eState = object.m_eState;
	this->m_Layer = object.m_Layer;
	this->m_Tag = object.m_Tag;
	this->m_pComponent = object.m_pComponent;
	CBaseObject::CBaseObject(object);	// ���N���X�̃R�s�[
}
CGameObject::~CGameObject()
{
	m_pComponent.clear();
}
void CGameObject::OnLoad()
{
	m_Transform = GetComponent<CTransform>();

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
	if (m_Transform.lock())
		return;

	m_Transform = AddComponent<CTransform>();
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
	for (auto & scene : CSceneManager::Get().GetAllScene()) 
	{
		auto obj = scene->GetObjManager()->FindGameObj(name.c_str());
		if (obj.lock()) 
		{
			return obj;
		}
	}
	return std::shared_ptr<CGameObject>();
}
std::weak_ptr<CGameObject> CGameObject::FindGameObjectWithTag(std::string name)
{
	for (auto & scene : CSceneManager::Get().GetAllScene())
	{
		auto obj = scene->GetObjManager()->FindGameObjWithTag(name);
		if (obj.lock())
		{
			return obj;
		}
	}
	return std::shared_ptr<CGameObject>();
}
std::weak_ptr<CGameObject> CGameObject::FindGameObjectWithTag(CTag tag)
{
	for (auto & scene : CSceneManager::Get().GetAllScene())
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
	for (auto & scene : CSceneManager::Get().GetAllScene())
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
	auto scene = CSceneManager::Get().GetActiveScene();
#endif // _DEBUG

	//if(auto scene = CSceneManager::Get().GetActiveScene();scene)
		//return scene->GetObjManager()->CreateGameObject();
	//return std::weak_ptr<CGameObject>();
	return CSceneManager::Get().GetActiveScene()->GetObjManager()->CreateGameObject(pObj);
}
void CGameObject::Destroy(std::weak_ptr<CGameObject> pObj)
{
	// �j����ԂɕύX
	// manager�X�V���̊m�F�Ŕj�������
	pObj.lock()->SetState(E_ObjectState::DESTROY);
	/*for (auto & scene : CSceneManager::Get().GetAllScene())
	{
		if (scene->GetObjManager()->DestroyObject(pObj))
		{
			return;
		}
	}*/
}
void CGameObject::DontDestroy(std::weak_ptr<CGameObject> pObj)
{
	CSceneManager::Get().GetActiveScene()->GetObjManager()->DontDestroy(pObj);
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

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("State"))
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

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("tag"))
		{
			auto tagList = CTagName::Get().GetList();
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
