//=========================================================
// [imageResourceManager.cpp] 
// �쐬:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <GraphicsSystem/Manager/modelManager.h>
#include <CoreSystem/Util/define.h>

using namespace MySpace::System;
using namespace MySpace::Graphics;

//==========================================================
// �R���X�g���N�^
//==========================================================
CModelManager::CModelManager()
{
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CModelManager::~CModelManager()
{
	UnloadAll();
}

//==========================================================
// �ǂݍ���
//==========================================================
bool CModelManager::Load(std::string name)
{
	// �������m��
	ModelSharedPtr addModel = std::make_shared<CAssimpModel>();

	// ���f���ǂݍ���
	if (!addModel->Load(Application::Get()->GetDevice(), Application::Get()->GetDeviceContext(),
		name))
	{
		return false;
	}
	m_aResourceMap.insert(CModelManager::Pair(name, addModel));

	return true;
}

//==========================================================
// �ް��j��
//==========================================================
bool CModelManager::Unload(std::string name)
{
	if (!m_aResourceMap.count(name))
		return false;

	m_aResourceMap[name]->Release();
	m_aResourceMap[name].reset();
	
	//--- ���X�g����j��
	auto it = m_aResourceMap.find(name);
	m_aResourceMap.erase(it);
	return true;
}

//==========================================================
// �S�J��
//==========================================================
void CModelManager::UnloadAll()
{
	for (auto it = m_aResourceMap.begin(); it != m_aResourceMap.end(); ++it)
	{
		(*it).second->Release();
		(*it).second.reset();
	}
	m_aResourceMap.clear();
}

//==========================================================
// �V�[���j�����m�F
//==========================================================
int CModelManager::SceneUnload()
{
#if _DEBUG	// �m�F
	int cnt = static_cast<int>(m_aResourceMap.size());
	for (auto it = m_aResourceMap.begin(); it != m_aResourceMap.end();)
	{
		// �����������g�����Ȃ���
		if ((*it).second.use_count() == 1)
		{
			(*it).second->Release();
			(*it).second.reset();
			it = m_aResourceMap.erase(it);
			--cnt;
			continue;
		}
		++it;
	}
	return cnt;

#else
	for (auto it = m_aResourceMap.begin(); it != m_aResourceMap.end();)
	{
		// �����������g�����Ȃ���
		if ((*it).second.use_count() == 1)
		{
			(*it).second->Release();
			(*it).second.reset();
			it = m_aResourceMap.erase(it);
			continue;
		}
		++it;
	}
	
#endif // _DEBUG

	return 0;
}

//==========================================================
// ���f���擾
// �Ȃ���Γǂݍ���
//==========================================================
ModelSharedPtr CModelManager::GetModel(std::string name)
{
	// ���݂��邩�m�F
	if (auto it = m_aResourceMap.find(name); it == m_aResourceMap.end())
	{
		if (!Load(name))
		{
			return ModelSharedPtr();
		}
	}
	return m_aResourceMap[name];
};

//==========================================================
// ���f���g�p���擾
//==========================================================
int CModelManager::GetModelCnt(std::string name)
{
	// �ǂ݂���ł��邩
	if (m_aResourceMap.count(name))
	{
		return m_aResourceMap[name].use_count();
	}
	return 0;
}

//==========================================================
// �g�p�I����Ăяo��
//==========================================================
void CModelManager::FinishUse(std::string name)
{
	// ���������ǂ݂���ł��邩
	if (m_aResourceMap.count(name))
	{
		// �Ǘ��׽���������҂����Ȃ���
		if (m_aResourceMap[name].use_count() == 1)
		{
			auto it = m_aResourceMap.find(name);
			(*it).second->Release();
			(*it).second.reset();
			it = m_aResourceMap.erase(it);
		}
	}
}