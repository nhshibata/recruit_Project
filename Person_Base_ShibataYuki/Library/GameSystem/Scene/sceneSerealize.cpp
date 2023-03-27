//=========================================================
// [sceneSerealize.h]
//---------------------------------------------------------
// �쐬:2023/03/17
//---------------------------------------------------------
// �V�[���̃V���A���C�Y�𕪊�
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Scene/sceneSerealize.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/components.h>
#include <CoreSystem/File/cerealize.h>

using namespace MySpace::SceneManager;


//=========================================================
// Scene�̕ۑ����������̂��i�[
//=========================================================
void CSceneSerealizeData::SaveStorage(CScene* scene)
{
	this->m_resource.Save();						// Resource���i�[
	this->m_SceneName = scene->GetSceneName();		// ���O�i�[
	auto objs = scene->GetObjManager()->GetList();	// �I�u�W�F�N�g���i�[
	for (auto & obj : objs)
	{
		//if (obj->GetTransform()->GetParent().lock())
		//	continue;	// �e�������

		this->m_aGameObjectManager.emplace_back(obj);
	}

	CLayer::SaveSystem();
	CTag::SaveSystem();
}

//=========================================================
// �ǂ݂��܂ꂽ�f�[�^�������n��
//=========================================================
bool CSceneSerealizeData::ReadDataPass(std::shared_ptr<CScene> newScene)
{
	if (!newScene)
		return false;

	this->m_resource.Load();						// ���[�h����Resource��ǂݍ���
	newScene->Init(newScene);
	newScene->SetSceneName(this->m_SceneName);		// ���O�ݒ�
	
	// ���[�h������
	for (auto & obj : this->m_aGameObjectManager)
	{
		obj->SetPtr(obj);
		obj->SetScene(newScene);

		auto coms = obj->GetComponentList();
		for (auto & com : coms)
		{
			com->SetOwner(obj);
		}
	}
	newScene->GetObjManager()->SetObjList(this->m_aGameObjectManager);	// �I�u�W�F�N�g�ݒ�

	// ���[�h������
	for (auto & obj : this->m_aGameObjectManager)
	{
		obj->OnLoad();
	}

	return true;
}