//==========================================================
// [sceneTransitionDetection.h]
// 作成:2022/08/20
// 更新:2022/08/25 再度追加される関数もあるため、変数に代入してから処理を行う
//----------------------------------------------------------
// シーンから呼び出したい関数ポインタを登録するｸﾗｽ
//==========================================================

//--- インクルード部
#include <GameSystem/Scene/sceneTransitionDetection.h>

using namespace MySpace::SceneManager;

//==========================================================
// コンストラクタ
//==========================================================
CSceneTransitionDetection::CSceneTransitionDetection()
{
	m_pChangeFunc.clear();
	m_pLoadFunc.clear();
	m_pUnloadFunc.clear();
}

//==========================================================
// デストラクタ
//==========================================================
CSceneTransitionDetection::~CSceneTransitionDetection()
{
	m_pChangeFunc.clear();
	m_pLoadFunc.clear();
	m_pUnloadFunc.clear();
}

//==========================================================
// Change時呼び出し
//==========================================================
void CSceneTransitionDetection::Call(CScene* prev, CScene* next)
{
	auto list = m_pChangeFunc;
	m_pChangeFunc.clear();

	for (auto it = list.begin(); it != list.end(); ++it)
	{
		(*it).Call(prev,next);
	}
}

//==========================================================
// Load時呼び出し
//==========================================================
void CSceneTransitionDetection::Call(CScene* scene, int mode)
{
	auto list = m_pLoadFunc;
	m_pLoadFunc.clear();
	if (list.size() == 0)
		return;

	for (auto it = list.begin(); it != list.end(); ++it)
	{
		(*it).Call(scene, mode);
	}
}

//==========================================================
// Unload時呼び出し
//==========================================================
void CSceneTransitionDetection::Call(CScene* scene)
{
	auto list = m_pUnloadFunc;
	m_pUnloadFunc.clear();

	for (auto it = list.begin(); it != list.end(); ++it)
	{
		(*it).Call(scene);
	}
}

//==========================================================
// 除外
//==========================================================
bool CSceneTransitionDetection::StopFunc(void* ptr)
{
	//--- 検索
	for (auto it = m_pChangeFunc.begin(); it != m_pChangeFunc.end(); ++it)
	{
		if (ptr == it->GetAdress()) 
		{
			m_pChangeFunc.erase(it);
			return true;
		}
	}

	for (auto it = m_pLoadFunc.begin(); it != m_pLoadFunc.end(); ++it)
	{
		if (ptr == it->GetAdress())
		{
			m_pLoadFunc.erase(it);
			return true;
		}
	}

	for (auto it = m_pUnloadFunc.begin(); it != m_pUnloadFunc.end(); ++it)
	{
		if (ptr == it->GetAdress())
		{
			m_pUnloadFunc.erase(it);
			return true;
		}
	}
	
	// 見つからなかった
	return false;
}