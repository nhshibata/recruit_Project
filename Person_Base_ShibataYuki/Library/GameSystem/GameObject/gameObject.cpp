//=========================================================
// [CGameObject.cpp]
//---------------------------------------------------------
// �쐬:2022/04/19 (�Ηj��)
// �X�V:2022/05/11 �^�O����v����I�u�W�F�N�g��S�Ď擾����֐��쐬
//			�ۑ�: ����̃R���|�[�l���g���g���ꍇ�Amap�͓K�؂ł͂Ȃ�
// �X�V:2022/11/14 �R�s�[�R���X�g���N�^�������A����̃R���|�[�l���g�𐶐�����悤�ɕύX
// �X�V:2023/02/13 TODO:�_�׽�����[���BFactory�N���X�Ȃǂ����ׂ�
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

#include <CoreSystem/File/cerealize.h>
#include <DebugSystem/imGuiPackage.h>

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
	:m_eState(E_ObjectState::ACTIVE), m_bCameraVisible(false)
{
	if(!m_pTag)
		m_pTag = std::make_shared<CTag>();
	if(!m_pLayer)
		m_pLayer = std::make_shared<CLayer>();
}

//==========================================================
// �������t���R���X�g���N�^
//==========================================================
CGameObject::CGameObject(std::string name)
	:m_eState(E_ObjectState::ACTIVE), m_bCameraVisible(false)
{
	SetName(name);
	m_pTag = std::make_shared<CTag>();
	m_pLayer = std::make_shared<CLayer>();
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
	this->m_pLayer = object.m_pLayer;
	this->m_pTag = object.m_pTag;
	this->m_aComponent = object.m_aComponent;
	this->m_aComponent.clear();

	// ���߰��Ă̖��O���瓯�����߰��Ă�ǉ�
	// TODO: �e���߰��Ă̒l�̺�߰�͍s���Ȃ�
	for (auto & component : object.m_aComponent)
	{
		// �R���|�[�l���g��ۑ����Ă���׽�ɒǉ����Ă��炤
		if (CComponentFactory::ObjSetComponent(*this, component->GetName()))
		{
			// �Ō���ɒǉ�����Ă���R���|�[�l���g���擾
			auto com = this->GetComponentList().back();
			// Transform�m�F
			if (com->GetName() == this->m_pTransform.lock()->GetName())
				this->m_pTransform = com->BaseToDerived<CTransform>();
			//com->Init();

		}
	}
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CGameObject::~CGameObject()
{
	Uninit();
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

//=========================================================
// ���
//=========================================================
void CGameObject::Uninit()
{
	for (auto & com : m_aComponent)
		com.reset();
	m_aComponent.clear();
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
// �������u��
//==========================================================
void CGameObject::CameraTest(const bool test)
{
	// �O��ʂ��Ă��鎞
	if (m_bCameraVisible && test)
	{
		OnWillRenderObject();
	}
	// �O��ʂ��Ă��Ȃ��A���A�f������
	else if(!m_bCameraVisible && test)
	{
		OnBecameVisible();
	}
	// �ʂ��Ă��Ȃ���
	if (!test)
	{
		OnBecameInvisible();
	}

	m_bCameraVisible = test;
}

//==========================================================
// �������u��
//==========================================================
void CGameObject::OnBecameVisible()
{
	for (auto & com : m_aComponent)
	{
		if (!com->IsActive())
		{
			continue;
		}

		com->OnBecameVisible();
	}
}

//==========================================================
// �����Ȃ��Ȃ����u��
//==========================================================
void CGameObject::OnBecameInvisible()
{
	for (auto & com : m_aComponent)
	{
		if (!com->IsActive())
		{
			continue;
		}

		com->OnBecameInvisible();
	}
}

//==========================================================
// �����Ă��鎞
//==========================================================
void CGameObject::OnWillRenderObject()
{
	for (auto & com : m_aComponent)
	{
		if (!com->IsActive())
		{
			continue;
		}

		com->OnWillRenderObject();
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
	for (auto it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		// �O�̂��ߊm�F
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
	for (auto it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
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
// �R���|�[�l���g�j��
//==========================================================
bool CGameObject::SetComponent(std::shared_ptr<CComponent> com)
{
	// �����false
	for (auto & it : m_aComponent)
	{
		if (com->GetName() == it->GetName())
			return false;
	}

	com->SetOwner(GetPtr());
	 m_aComponent.push_back(com); 
	 return true;
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
	for (Component_List::iterator it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		(*it)->OnCollisionStay(obj);
	}
}

//==========================================================
// �ՓˏI�����Ăяo��
//==========================================================
void CGameObject::OnCollisionExit(CGameObject* obj)
{
	for (Component_List::iterator it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		(*it)->OnCollisionExit(obj);
	}
}

//==========================================================
// �Փˎ��Ăяo���i�ضް)
//==========================================================
void CGameObject::OnTriggerEnter(CGameObject* obj)
{
	for (Component_List::iterator it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		(*it)->OnTriggerEnter(obj);
	}
}

//==========================================================
// �Փ˒��Ăяo���i�ضް)
//==========================================================
void CGameObject::OnTriggerStay(CGameObject* obj)
{
	for (Component_List::iterator it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
	{
		(*it)->OnTriggerStay(obj);
	}
}

//==========================================================
// �ՓˏI�����Ăяo���i�ضް)
//==========================================================
void CGameObject::OnTriggerExit(CGameObject* obj)
{
	for (Component_List::iterator it = m_aComponent.begin(); it != m_aComponent.end(); ++it)
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
	if (auto all = CSceneManager::Get()->GetAllScene(); all.size() != 0)
	{
		for (auto & scene : all)
		{
			auto obj = scene->GetObjManager()->FindGameObj(name.c_str());
			if (obj.lock())
			{
				return obj;
			}
		}
	}
	return std::shared_ptr<CGameObject>();
}

//==========================================================
// �^�O������v����I�u�W�F�N�g�T��
//==========================================================
std::weak_ptr<CGameObject> CGameObject::FindGameObjectWithTag(std::string tag)
{
	if (auto all = CSceneManager::Get()->GetAllScene(); all.size() != 0)
	{
		for (auto & scene : all)
		{
			auto obj = scene->GetObjManager()->FindGameObjWithTag(tag);
			if (obj.lock())
			{
				return obj;
			}
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
	if (auto all = CSceneManager::Get()->GetAllScene(); all.size() != 0)
	{
		for (auto & scene : all)
		{
			auto objs = scene->GetObjManager()->FindGameObjctsWithTag(tag);
			// �i�[
			for (auto & obj : objs)
			{
				ret.push_back(obj);
			}
		}
	}
	return ret;
}

//==========================================================
// �^�O�׽����v����I�u�W�F�N�g�擾
//==========================================================
std::weak_ptr<CGameObject> CGameObject::FindGameObjectWithTag(CTag tag)
{
	if (auto all = CSceneManager::Get()->GetAllScene(); all.size() != 0)
	{
		for (auto & scene : all)
		{
			auto obj = scene->GetObjManager()->FindGameObjWithTag(tag);
			if (obj.lock())
			{
				return obj;
			}
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
	if (auto all = CSceneManager::Get()->GetAllScene(); all.size() != 0)
	{
		for (auto & scene : all)
		{
			auto obj = scene->GetObjManager()->FindGameObjWithTag(tag);
			if (obj.lock())
			{
				retList.push_back(obj);
			}
		}
	}
	return std::list<std::weak_ptr<CGameObject>>();
}

//==========================================================
// �I�u�W�F�N�g����
// �Ǘ��N���X���ɓo�^���ꂽ���߲���������n��
//==========================================================
std::weak_ptr<CGameObject> CGameObject::CopyObject(CGameObject::Ptr pObj)
{
	// �V���A���C�Y�N���X�쐬
	CCerealize<std::shared_ptr<CGameObject>> sirial;
	{
		// �ް����O���ۑ�
		auto obj = pObj;
		sirial.OutputFile(obj->GetName(), COPY_DATA_GAME_OBJECT_PATH, obj);
	}

	// �ꎞ�I�ȃI�u�W�F�N�g����
	if (auto work = std::make_shared<CGameObject>(); work)
	{
		// �ް��ǂݍ���
		work = sirial.InputFile(COPY_DATA_GAME_OBJECT_PATH);

		// �V�����I�u�W�F�N�g����
		auto newObj = CGameObject::CreateObject();
		// �ǂ݂��܂ꂽ�R���|�[�l���g�̎󂯓n��
		auto comList = work->GetComponentList();
		for (auto & com : comList)
		{
			newObj.lock()->SetComponent(com);
			//--- �`��Ɠ����蔻��N���X�͗v������K�v�����邽�߁AInit���Ăяo��
			// NOTE: ����I�Ȃ��̂ŁA�����ǂ��Ȃ̂�
			if (com->GetName().find("Renderer") != std::string::npos ||
				com->GetName().find("Collision") != std::string::npos)
			{
				com->Awake();
				com->Init();
			}
		}

		// �I�u�W�F�N�g�j��
		work.reset();

		return newObj;
	}

	// ���s
	return CGameObject::Ptr();
}

//==========================================================
// �I�u�W�F�N�g����
// �Ǘ��N���X���ɓo�^���ꂽ���߲���������n��
//==========================================================
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

//==========================================================
// �Ǘ����Ăق����Ȃ��I�u�W�F�N�g����
// ��邾���ŊǗ��͎󂯎�������ɈϔC
//==========================================================
std::shared_ptr<CGameObject> CGameObject::CreateDebugObject(std::shared_ptr<CGameObject> pObj)
{
	if (!pObj)
		pObj = std::make_shared<CGameObject>();
	// ���g��weakPtr��n��
	pObj.get()->SetPtr(pObj);
	pObj.get()->Awake();
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
	CSceneManager::Get()->GetActiveScene()->GetObjManager()->DontDestroy(pObj);
}


#ifdef BUILD_MODE

void CGameObject::ImGuiDebug()
{
	static std::vector<std::string> aStateName = {
		"ACTIVE",			// �X�V���
		"WAIT",				// �ҋ@
		"DESTROY",			// �폜
		"TAKEOVER",			// �����p���i�g���܂킵)
		"STOP",				// �f�o�b�O?
	};
	static bool isAddTag = false;
	static bool isAddLayer = false;

	//--- ��ԕύX
	if (ImGui::BeginMenuBar()) 
	{
		if (ImGui::BeginMenu(u8"State"))
		{
			for (int state = 0; state < E_ObjectState::MAX_OBJECT_STATE; ++state)
			{
				if (ImGui::MenuItem(aStateName[state].c_str())) 
				{
					m_eState = (E_ObjectState)state;
					break;
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	
	//--- �^�O�ύX
	if (ImGui::BeginMenuBar()) 
	{
		if (ImGui::BeginMenu(u8"Tag"))
		{
			auto tagList = CTag::GetNameList();
			for (int idx = 0; idx < static_cast<int>(tagList.size()); ++idx)
			{
				auto tagName = tagList[idx].c_str();
				if (ImGui::MenuItem(tagName))
				{
					GetTagPtr()->SetTag(tagName);
				}
			}
			ImGui::NewLine();
			if (ImGui::MenuItem("Add Tag"))
			{
				isAddTag = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	CTag::ImGuiTag(isAddTag);
	
	//--- ���C���[�ύX
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(u8"layer"))
		{
			auto layerList = CLayer::GetNameList(true);
			for (int idx = 0; idx < static_cast<int>(layerList.size()); ++idx)
			{
				auto tagName = layerList[idx].c_str();
				if (ImGui::MenuItem(tagName))
				{
					GetLayerPtr()->SetLayer(idx);
					break;
				}
			}
			ImGui::NewLine();
			if (ImGui::MenuItem("Add Layer"))
			{
				isAddLayer = true;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	CLayer::ImGuiLayer(isAddLayer);

	ImGui::PushItemWidth(200);

	//--- ���O�ύX
	char name[56] = "";
	strcpy_s(name, GetName().c_str());
	Debug::SetTextAndAligned("Object Name");
	if(ImGui::InputText("##Name", name, 56))
		SetName(name);

	//--- �\��
	ImGui::Text("State:%s", aStateName[m_eState].c_str());
	ImGui::SameLine();
	{
		Debug::SetControlPosX();
		if (auto select = Debug::DispComboSelect(aStateName, "State", m_eState); select != m_eState)
			m_eState = (CGameObject::E_ObjectState)select;
	}

	ImGui::Text("Tag:%s", GetTagPtr()->GetTag().c_str());
	ImGui::SameLine();
	{
		Debug::SetControlPosX();
		auto tagList = CTag::GetNameList();
		if (auto select = Debug::DispComboSelect(tagList, "Tag", GetTagPtr()->GetTag().c_str()); !select.empty())
			GetTagPtr()->SetTag(select);
	}

	ImGui::Text("Layer:%d", GetLayerPtr()->GetLayer());
	ImGui::SameLine();
	{
		Debug::SetControlPosX();
		auto layerList = CLayer::GetNameList();
		if (auto select = Debug::DispComboSelect(layerList, "Layer", GetLayerPtr()->GetName().c_str()); !select.empty())
			GetLayerPtr()->SetLayer(select);
	}

	ImGui::Text("Component Num:%d", GetComponentList().size());

	ImGui::PopItemWidth();

}

#endif // BUILD_MODE
