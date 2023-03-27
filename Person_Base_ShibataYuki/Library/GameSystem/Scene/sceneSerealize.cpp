//=========================================================
// [sceneSerealize.h]
//---------------------------------------------------------
// 作成:2023/03/17
//---------------------------------------------------------
// シーンのシリアライズを分割
//=========================================================

//--- インクルード部
#include <GameSystem/Manager/sceneManager.h>
#include <GameSystem/Scene/sceneSerealize.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/components.h>
#include <CoreSystem/File/cerealize.h>

using namespace MySpace::SceneManager;


//=========================================================
// Sceneの保存したいものを格納
//=========================================================
void CSceneSerealizeData::SaveStorage(CScene* scene)
{
	this->m_resource.Save();						// Resourceを格納
	this->m_SceneName = scene->GetSceneName();		// 名前格納
	auto objs = scene->GetObjManager()->GetList();	// オブジェクトを格納
	for (auto & obj : objs)
	{
		//if (obj->GetTransform()->GetParent().lock())
		//	continue;	// 親が居れば

		this->m_aGameObjectManager.emplace_back(obj);
	}

	CLayer::SaveSystem();
	CTag::SaveSystem();
}

//=========================================================
// 読みこまれたデータを引き渡し
//=========================================================
bool CSceneSerealizeData::ReadDataPass(std::shared_ptr<CScene> newScene)
{
	if (!newScene)
		return false;

	this->m_resource.Load();						// ロードしたResourceを読み込み
	newScene->Init(newScene);
	newScene->SetSceneName(this->m_SceneName);		// 名前設定
	
	// ロード時処理
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
	newScene->GetObjManager()->SetObjList(this->m_aGameObjectManager);	// オブジェクト設定

	// ロード時処理
	for (auto & obj : this->m_aGameObjectManager)
	{
		obj->OnLoad();
	}

	return true;
}