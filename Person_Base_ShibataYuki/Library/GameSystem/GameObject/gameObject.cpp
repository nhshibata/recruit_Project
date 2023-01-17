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
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Factory/componentFactory.h>

#include <ImGui/imgui.h>

using namespace MySpace::SceneManager;
using namespace MySpace::Game;


#ifdef _DEBUG
// �I�u�W�F�N�g�̖��O��tag���m�肽������
static inline void CheckObj(CGameObject* obj)
{
	auto name = obj->GetName();
	auto tag = obj->GetTagPtr()->GetTag();
}
#endif // _DEBUG

//==========================================================
// �R���X�g���N�^
//==========================================================
CGameObject::CGameObject()
	:m_eState(E_ObjectState::ACTIVE)
{
	m_pTag = std::make_unique<CTag>();
	m_pLayer = std::make_unique<CLayer>();
}

//==========================================================
// �������t���R���X�g���N�^
//==========================================================
CGameObject::CGameObject(std::string name)
	:m_eState(E_ObjectState::ACTIVE)
{
	SetName(name);
	m_pTag = std::make_unique<CTag>();
	m_pLayer = std::make_unique<CLayer>();
}

//==========================================================
// �R�s�[�R���X�g���N�^
//==========================================================
CGameObject::CGameObject(const CGameObject & object)
{

#if BUILD_MODE
	this->m_objName = object.m_objName + std::to_string(++m_nCopyCnt);
#else
	this->m_objName = object.m_objName;
#endif // BUILD_MODE

	this->m_pTransform = object.m_pTransform;
	this->m_eState = object.m_eState;
	this->m_pLayer = std::make_unique<CLayer>();
	this->m_pTag = std::make_unique<CTag>();
	this->m_pLayer.get()->SetLayer(*object.m_pLayer.get()->GetLayer());
	this->m_pTag.get()->SetTag(object.m_pTag.get()->GetTag());

	// ���߰��Ă̖��O���瓯�����߰��Ă�ǉ�
	// TODO: �e���߰��Ă̒l�̺�߰�͍s���Ȃ�
	for (auto & component : object.m_aComponent)
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

//==========================================================
// �f�X�g���N�^
//==========================================================
CGameObject::~CGameObject()
{
	for (auto & com : m_aComponent)
		com.reset();
	m_aComponent.clear();
}

//==========================================================
// �ǂݍ��ݎ��Ăяo��
//==========================================================
void CGameObject::OnLoad()
{
	m_pTransform = GetComponent<CTransform>();

	// �R���|�[�l���g�̃��[�h�Ăяo��
	for (Component_List::iterator it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		(*it)->OnLoad();
	}
}

//==========================================================
// ���������O�ɌĂяo������
// �I�u�W�F�N�g��R���|�[�l���g��ǉ�����Ȃ炱��
// ���O��^�O�A���C���[�̐ݒ�������ōs��
//==========================================================
void CGameObject::Awake()
{
	// Awake�ͺ��߰��Ēǉ����ɌĂяo�����d�l
	if (m_pTransform.lock())
		return;

	m_pTransform = AddComponent<CTransform>();
}

//==========================================================
// ������
// �I�u�W�F�N�g��R���|�[�l���g��T���Ȃ炱��
//==========================================================
void CGameObject::Init()
{
	// �R���|�[�l���g�������Ăяo��
	/*for (auto & com : m_pComponent)
	{
		com->Init();
	}*/
	for (auto it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		(*it)->Init();
	}
}

//==========================================================
// �R���|�[�l���g�̍X�V
// ���̍X�V���ŃI�u�W�F�N�g���j���͂���Ȃ�
//==========================================================
void CGameObject::Update()
{
	for (auto & com : m_aComponent)
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

//==========================================================
// �Q�[���I�u�W�F�N�g���Ƃ̒x���X�V
//==========================================================
void CGameObject::LateUpdate()
{
	for (auto & com : m_aComponent)
	{
		if (!com->IsActive())
		{
			continue;
		}

		com->LateUpdate();
	}
}

//==========================================================
// ��莞�ԍX�V
//==========================================================
void CGameObject::FixedUpdate()
{
	for (auto & com : m_aComponent)
	{
		if (!com->IsActive())
		{
			continue;
		}

		com->FixedUpdate();
	}
}

//==========================================================
// �R���|�[�l���g�ݒ�
//==========================================================
void CGameObject::ComponentAddPreparation(std::shared_ptr<CComponent> com)
{
	//m_pComponent.push_back(com);
	com->SetPtr(com);					// ���g�̃|�C���^�̐ݒ�
	//com->SetOwner(GetPtr().lock());		// ����gameobject�̐ݒ�
	//com->Awake();
}

//==========================================================
// �R���|�[�l���g�̔j��
//==========================================================
template <class T>
bool CGameObject::RemoveComponent(std::string comName)
{
	Component_List::iterator it;
	for (it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		if (typeid(T).name == (*it)->GetName())
		{
			m_aComponent.erase(it);
			return true;
		}
	}
	return false;
}

//==========================================================
// �R���|�[�l���g�j��
//==========================================================
bool CGameObject::RemoveComponent(std::weak_ptr<CComponent> com)
{
	Component_List::iterator it;
	for (it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		if ((*it) == com.lock())
		{
			m_aComponent.erase(it);
			return true;
		}
	}
	return false;
}

//==========================================================
// ��Ԑݒ�
// inline?
//==========================================================
void CGameObject::SetState(const E_ObjectState state)
{
	m_eState = state;
}

//==========================================================
// �^�O�̈ړ�
// �����ł��̂͊ԈႢ?
//==========================================================
void CGameObject::SetObjTag(const std::string tag) 
{ 
	if(auto scene = GetScene(); scene)
		scene->GetObjManager()->TagMove(tag, GetPtr());

	m_pTag->SetTag(tag);
};

//==========================================================
// �Փˎ��Ăяo��
//==========================================================
void CGameObject::OnCollisionEnter(CGameObject* obj)
{
#ifdef _DEBUG
	CheckObj(obj);
#endif // _DEBUG

	for (Component_List::iterator it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		(*it)->OnCollisionEnter(obj);
	}
}

//==========================================================
// �Փ˒��Ăяo��
//==========================================================
void CGameObject::OnCollisionStay(CGameObject* obj)
{
	Component_List::iterator it = m_aComponent.begin();
	for (; it != m_aComponent.end(); ++it)
	{
		(*it)->OnCollisionStay(obj);
	}
}

//==========================================================
// �ՓˏI�����Ăяo��
//==========================================================
void CGameObject::OnCollisionExit(CGameObject* obj)
{
	Component_List::iterator it = m_aComponent.begin();
	for (; it != m_aComponent.end(); ++it)
	{
		(*it)->OnCollisionExit(obj);
	}
}

//==========================================================
// �Փˎ��Ăяo���i�ضް)
//==========================================================
void CGameObject::OnTriggerEnter(CGameObject* obj)
{
	Component_List::iterator it = m_aComponent.begin();
	for (; it != m_aComponent.end(); ++it)
	{
		(*it)->OnTriggerEnter(obj);
	}
}

//==========================================================
// �Փ˒��Ăяo���i�ضް)
//==========================================================
void CGameObject::OnTriggerStay(CGameObject* obj)
{
	Component_List::iterator it = m_aComponent.begin();
	for (; it != m_aComponent.end(); ++it)
	{
		(*it)->OnTriggerStay(obj);
	}
}

//==========================================================
// �ՓˏI�����Ăяo���i�ضް)
//==========================================================
void CGameObject::OnTriggerExit(CGameObject* obj)
{
	Component_List::iterator it = m_aComponent.begin();
	for (; it != m_aComponent.end(); ++it)
	{
		(*it)->OnTriggerExit(obj);
	}
}

//--- �ÓI�����o�֐�

//==========================================================
// ���O����v����I�u�W�F�N�g�T��
//==========================================================
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

//==========================================================
// �^�O������v����I�u�W�F�N�g�T��
//==========================================================
std::weak_ptr<CGameObject> CGameObject::FindGameObjectWithTag(std::string tag)
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

//==========================================================
// �^�O������v����S�I�u�W�F�N�g�擾
//==========================================================
std::list<std::weak_ptr<CGameObject>> CGameObject::FindGameObjectsWithTag(std::string tag)
{
	std::list<std::weak_ptr<CGameObject>> ret;
	for (auto & scene : CSceneManager::Get().GetAllScene())
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

//==========================================================
// �^�O�׽����v����I�u�W�F�N�g�擾
//==========================================================
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

//==========================================================
// �^�O�׽����v����S�I�u�W�F�N�g�擾
//==========================================================
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

//==========================================================
// �I�u�W�F�N�g����
// �Ǘ��N���X���ɓo�^���ꂽ���߲���������n��
//==========================================================
std::weak_ptr<CGameObject> CGameObject::CreateObject(CGameObject* pObj)
{
#ifdef _DEBUG
	auto scene = CSceneManager::Get().GetActiveScene();
#endif // _DEBUG
	
	// ��߰�ł͂Ȃ�����
	if (!pObj) 
	{
		return CSceneManager::Get().GetActiveScene()->GetObjManager()->CreateGameObject();
	}

	return CSceneManager::Get().GetActiveScene()->GetObjManager()->CreateGameObject(pObj);
}

//==========================================================
// �Ǘ����Ăق����Ȃ��I�u�W�F�N�g����
// ��邾���ŊǗ��͎󂯎�������ɈϔC
//==========================================================
std::shared_ptr<CGameObject> CGameObject::CreateDebugObject(std::shared_ptr<CGameObject> pObj)
{
	// ���g��weakPtr��n��
	pObj.get()->SetPtr(pObj);

	// ������
	pObj.get()->Awake();	// ����OnCreate�ȋC������
	pObj.get()->Init();
	return pObj;
}

//==========================================================
// �I�u�W�F�N�g�j��
// manager�X�V���̊m�F�Ŕj�������
//==========================================================
void CGameObject::Destroy(std::weak_ptr<CGameObject> pObj)
{
	// �j����ԂɕύX
	pObj.lock()->SetState(E_ObjectState::DESTROY);
}

//==========================================================
// ��j��o�^
//==========================================================
void CGameObject::DontDestroy(std::weak_ptr<CGameObject> pObj)
{
	CSceneManager::Get().GetActiveScene()->GetObjManager()->DontDestroy(pObj);
}


#ifdef BUILD_MODE

void CGameObject::ImGuiDebug()
{
	const char* szState[CGameObject::MAX_OBJECT_STATE] = {
		"ACTIVE",				// �X�V���
		"WAIT",				// �ҋ@
		"DESTROY",			// �폜
		"TAKEOVER",			// �����p���i�g���܂킵)
		"STOP",				// �f�o�b�O?
	};

	//--- ��ԕύX
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

	//--- �^�O�ύX
	if (ImGui::BeginMenuBar()) 
	{
		if (ImGui::BeginMenu(u8"tag"))
		{
			auto tagList = CTag::GetNameList();
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
	
	//--- ���O�ύX
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
	
}

#endif // BUILD_MODE
