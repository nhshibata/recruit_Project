//=========================================================
// [gameobjectManager.cpp]
// �쐬:2022/06/07
//---------------------------------------------------------
// �Q�[���I�u�W�F�N�g���Ǘ�����׽
// �Ǘ��݂̂Ő����͑��̸׽�ōs��
// 
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Manager/gameObjectManager.h>
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Transform/Tween.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <GameSystem/Component/Camera/stackCamera.h>

using namespace MySpace::Game;

//==========================================================
// �R���X�g���N�^
//==========================================================
CGameObjectManager::CGameObjectManager(std::shared_ptr<CScene> scene)
{
	this->m_pAffiliationScene = scene;
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CGameObjectManager::~CGameObjectManager()
{
}

//==========================================================
// ������
//==========================================================
void CGameObjectManager::Init()
{
	for (auto & obj : m_aGameObjList)
	{
		obj->Init();
	}
}

//==========================================================
// ���
//==========================================================
void CGameObjectManager::Uninit()
{
	// �S�ď��O
	for (auto & obj : m_aGameObjList)
	{
		obj->Uninit();
		obj.reset();
	}
	m_aGameObjList.clear();
	m_aAddObjList.clear();
	m_aTagMap.clear();
}

//==========================================================
// ���
//==========================================================
void CGameObjectManager::AllUninit()
{
	m_aGameObjList.clear();
	m_aAddObjList.clear();
	m_aDontDestroyList.clear();
}

//==========================================================
// �X�V
//==========================================================
void CGameObjectManager::Update()
{
	WeakList aActiveObj(0);		// �A�N�e�B�u�ȃI�u�W�F�N�g���i�[
	WeakList aDestoroyObj(0);	// �j���I�u�W�F�N�g���i�[
	
	//--- �I�u�W�F�N�g�X�V
	// �R���|�[�l���g�̍X�V
	for (auto & obj : m_aGameObjList)
	{
		//--- component���ŃV�[�����j�����ꂽ�ꍇ�A�����𔲂���
		if (MySpace::SceneManager::CSceneManager::Get()->Escape())
			return;

		// ��Ԋm�F
		if (obj->GetState() == CGameObject::E_ObjectState::DESTROY)
			aDestoroyObj.push_back(obj);
		if (obj->GetState() != CGameObject::E_ObjectState::ACTIVE)
			continue;

		//--- �X�V�Ɗi�[
		obj->Update();
		aActiveObj.push_back(obj);
	}
	
	// �Ō�ɍX�V���������̂��X�V
	for (auto & obj : aActiveObj)
	{
		//--- component���ŃV�[�����j�����ꂽ�ꍇ�A�����𔲂���
		if (MySpace::SceneManager::CSceneManager::Get()->Escape())
			return;
		obj.lock()->LateUpdate();
	}
	// �z��̃��Z�b�g
	aActiveObj.clear();

	//--- �I�u�W�F�N�g�̔j��
	for (auto & obj : aDestoroyObj)
	{
		DestroyObject(obj);
		obj.reset();
	}
	// �z��̃��Z�b�g
	aDestoroyObj.clear();

	// �ǉ��I�u�W�F�N�g�̊m�F�A�ǉ�
	if (m_aAddObjList.size() != 0)
		ObjectListAddUpdate();
	
}

//==========================================================
// �f�o�b�O�p�X�V
// �ǉ��Ɣj���A��ݽ̫�т̍X�V���s��
//==========================================================
void CGameObjectManager::UpdateInDebug()
{
	WeakList pDestoroyObj(0);	// �j���I�u�W�F�N�g���i�[

	// ��Ԋm�F�i���͎��̃t���[���܂ł͔j������Ȃ��I�u�W�F�N�g�����邱��?)
	for (auto & obj : m_aGameObjList)
	{
		// ��Ԃɂ�蕪��
		switch (obj->GetState())
		{
		case CGameObject::ACTIVE:				// �ʏ�̍X�V
			// �g�����X�t�H�[���̍X�V
			obj.get()->GetTransform()->Update();
			break;
		case CGameObject::WAIT:					// �ҋ@
			break;
		case CGameObject::DESTROY:				// �폜�I�u�W�F�N�g
			// �i�[
			pDestoroyObj.push_back(obj);
			break;
		case CGameObject::TAKEOVER:				// �����p���҂�
			break;
		case CGameObject::MAX_OBJECT_STATE:		// �ő吔
			break;
		default:
			break;
		}
	}

	// �I�u�W�F�N�g�̔j��
	for (auto & obj : pDestoroyObj)
	{
		DestroyObject(obj);
		obj.reset();
	}

	// �z��̃��Z�b�g
	pDestoroyObj.clear();

	// �ǉ��I�u�W�F�N�g�̊m�F�A�ǉ�
	if(m_aAddObjList.size() != 0)
		ObjectListAddUpdate();	
}

//==========================================================
// �Œ�X�V
//==========================================================
void CGameObjectManager::FixedUpdate()
{
	// �I�u�W�F�N�g�X�V
	// �R���|�[�l���g�̍X�V
	for (auto & obj : m_aGameObjList)
	{
		//--- component���ŃV�[�����j�����ꂽ�ꍇ�A�����𔲂���
		if (MySpace::SceneManager::CSceneManager::Get()->Escape())
			return;

		if (obj->GetState() == CGameObject::ACTIVE) 
		{
			obj->FixedUpdate();
		}
	}
}

//==========================================================
// �I�u�W�F�N�g�ǉ�
//==========================================================
bool CGameObjectManager::ObjectListAddUpdate()
{
	// �ǉ��I�u�W�F�N�g����łȂ���
	// �r���ǉ��̃I�u�W�F������ꍇ�̂���while
	while(m_aAddObjList.size() != 0)
	{
		auto addList = m_aAddObjList;	// �󂯎��
		m_aAddObjList.clear();			// �z������

		// �i�[���A�������Ăяo��
		for (auto & addObj : addList)
		{
			if (addObj)
			{
				addObj->Awake();
				SetGameObject(addObj);
			}
		}
		
		//--- �������Ăяo��
		for (auto & addObj : addList)
		{

#ifdef _DEBUG
			auto name = addObj->GetName();
#endif // !_DEBUG


			addObj->Init();
		}
	}
	
	return false;
}

//==========================================================
// �`��ɕK�v�ȃI�u�W�F�N�g�̍쐬�Z�b�g
//==========================================================
void CGameObjectManager::CreateBasicObject()
{
	std::shared_ptr<CGameObject> pObj = CreateGameObject();
	
	// ���
	auto cam = pObj->AddComponent<CStackCamera>();
	cam->GetOwner()->SetName("MainCamera");

	// ���C�g
	pObj = CreateGameObject();
	auto light = pObj->AddComponent<CDirectionalLight>();
	light->GetOwner()->SetName("DirectionalLight");
}

//==========================================================
// �X�V�z��ւ̒ǉ�
//==========================================================
void CGameObjectManager::SetGameObject(std::shared_ptr<CGameObject> obj)
{
	if (!obj)
		return;
	
	//--- ���ɑ��݂��Ă���Ȃ甲����
	for(auto & pObj : m_aGameObjList)
	{
		if (pObj == obj)
			return;
	}
	//--- �ǉ�
	m_aGameObjList.push_back(obj);
	TagMove(obj->GetTag(), obj);
}

//==========================================================
// tag�p�̘A�z�z����̈ړ�
//==========================================================
void CGameObjectManager::TagMove(std::string NextTag, std::weak_ptr<CGameObject> obj)
{
	// ����^�O
	if (obj.lock()->GetTag() == NextTag)
	{
		if (!m_aTagMap.count(NextTag))
		{
			m_aTagMap[NextTag].list.push_back(obj);
		}
		else
		{
			auto list = m_aTagMap[NextTag];
			auto it = list.FindObj(obj.lock());
			// ������Ȃ������̂Œǉ�
			if (it == list.list.end())
				m_aTagMap[NextTag].list.push_back(obj);
		}
		return;
	}

	//--- ���݂�tag���珜�O
	const auto tag = obj.lock()->GetTag();
	for (auto it = m_aTagMap[tag].list.begin(); it != m_aTagMap[tag].list.end();)
	{
		if (obj.lock() == (*it).lock())
		{
			m_aTagMap[tag].list.erase(it);
			break;
		}
		++it;
	}
	
	// �ύX���tag�ֈړ�
	m_aTagMap[NextTag].list.push_back(obj);
}

//==========================================================
// �I�u�W�F�N�g����
//==========================================================
std::shared_ptr<CGameObject> CGameObjectManager::CreateGameObject(CGameObject* pObj)
{
	static int nCreateObjNum = 0;

	std::shared_ptr<CGameObject> pNewObj;
	//--- ��߰�m�F
	if (pObj)
	{
		pNewObj = std::make_shared<CGameObject>(*pObj);
	}
	else
	{
		pNewObj = std::make_shared<CGameObject>();
		// ������
		pNewObj->SetName(std::string("GameObj_" + std::to_string(++nCreateObjNum)));
	}
	
	// �����̏����V�[����������
	pNewObj.get()->SetScene(m_pAffiliationScene);

	// ���g��weakPtr��n��
	pNewObj.get()->SetPtr(pNewObj);
	if (!pObj)
		TagMove(CTagDefault::DEFAULT, pNewObj);
	else
		TagMove(pObj->GetTag(), pNewObj);

	pNewObj.get()->Awake();	// ����OnCreate�ȋC������
	AddGameObject(pNewObj);	// �ǉ��҂����X�g�ɒǉ�

	return pNewObj;
};

//==========================================================
// �I�u�W�F�N�g�j��
//==========================================================
bool CGameObjectManager::DestroyObject(std::weak_ptr<CGameObject> pObj)
{
	// ����
	auto it = std::find(m_aGameObjList.begin(), m_aGameObjList.end(), pObj.lock());
	if (it == m_aGameObjList.end())
		return false;	// �z��ɋ��Ȃ�

	{
		// tag�Ǘ���������O
		auto weakMap = m_aTagMap[pObj.lock()->GetTag()];
		auto mapIt = weakMap.FindObj(pObj.lock());
		if (mapIt != weakMap.list.end())
			weakMap.list.erase(mapIt);
	}

	// ��j�󌟍�
	if (auto itD = std::find(m_aDontDestroyList.begin(), m_aDontDestroyList.end(), pObj.lock()); itD != m_aDontDestroyList.end())
		m_aDontDestroyList.erase(itD);	// FIXME: ��������������邩�v�m�F

	// �����I�Ȕj��
	pObj.lock().reset();

	// �z�񂩂�̏��O
	m_aGameObjList.erase(it);
	
	// �Ď��Ώۂ��c���Ă���z�肵�Ȃ��G���[
	if (pObj.lock()) 
		return false;
	
	return true;
}

//=========================================================
// �I�u�W�F�N�g�㏑��
// ��j��I�u�W�F�N�g�n��
//=========================================================
void CGameObjectManager::SetObjList(std::list<std::shared_ptr<CGameObject>> list, bool addDvive)
{
	if (addDvive)
	{
		auto work = m_aDontDestroyList;
		Uninit();
		m_aGameObjList = list;
		for (auto & obj : work)
		{
			m_aGameObjList.push_back(obj);
		}
	}
	else
	{
		Uninit();
		m_aGameObjList = list;
	}

}

//=========================================================
// �I�u�W�F�N�g����(���O)
//=========================================================
std::weak_ptr<CGameObject> CGameObjectManager::FindGameObj(std::string name)
{
	for (GameObjList::iterator it = m_aGameObjList.begin(); it != m_aGameObjList.end(); ++it)
	{
		if ((*it).get()->GetName() == name)
		{
			return (*it);
		}
	}
	return std::shared_ptr<CGameObject>();
}

//=========================================================
// �I�u�W�F�N�g����(�^�O��)
//=========================================================
std::weak_ptr<CGameObject> CGameObjectManager::FindGameObjWithTag(std::string tag)
{
	// �Ȃ�
	if (m_aTagMap.count(tag) == 0)
	{
		return std::weak_ptr<CGameObject>();
	}
	return m_aTagMap[tag].list.begin()->lock();
}

//=========================================================
// �I�u�W�F�N�g����(��޸׽)
//=========================================================
std::weak_ptr<CGameObject> CGameObjectManager::FindGameObjWithTag(CTag tag)
{
	GameObjList::iterator it = m_aGameObjList.begin();
	for (; it != m_aGameObjList.end(); ++it)
	{
		if ((*it).get()->GetTagPtr()->Compare(tag.GetTag()))
		{
			return (*it);
		}
	}
	return std::shared_ptr<CGameObject>();
}

//=========================================================
// �I�u�W�F�N�g����(��޸׽)
//=========================================================
std::list<std::weak_ptr<CGameObject>> CGameObjectManager::FindGameObjctsWithTag(std::string tag)
{
	// �Ȃ�
	if (m_aTagMap.count(tag) == 0)
	{
		m_aTagMap[tag] = STGameObjWeakList();
		return std::list<std::weak_ptr<CGameObject>>();
	}
	return m_aTagMap[tag].list;
}

//=========================================================
// ��j��I�u�W�F�N�g�̈ړ�
//=========================================================
void CGameObjectManager::PassDontDestroyList(CGameObjectManager* mgr)
{
	for (auto & obj : m_aDontDestroyList)
	{
		mgr->SetGameObject(obj);		// ���ڒǉ�
		mgr->DontDestroy(obj);			// �o�^
		obj->SetScene(mgr->GetScene());	// TODO: DontDestroyScene�����?
	}
}