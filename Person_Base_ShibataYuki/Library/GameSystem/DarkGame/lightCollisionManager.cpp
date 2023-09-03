//==========================================================
// [lightCollisionManager.h]
//----------------------------------------------------------
// 作成:2023/06/01
//----------------------------------------------------------
// ライトの範囲確認クラスを管理する
//==========================================================

//--- インクルード部
#include <GameSystem/DarkGame/lightCollisionManager.h>
#include <GameSystem/DarkGame/lightCollision.h>

using namespace DarkGame;

//==========================================================
// コンストラクタ
//==========================================================
CLightCollisionManager::CLightCollisionManager()
{

}

//==========================================================
// コンストラクタ
//==========================================================
CLightCollisionManager::CLightCollisionManager(CGameObject::Ptr ptr)
	:CComponent(ptr)
{

}

//==========================================================
// デストラクタ
//==========================================================
CLightCollisionManager::~CLightCollisionManager()
{
	m_aLights.clear();
}

//==========================================================
// 生成時処理
//==========================================================
void CLightCollisionManager::Awake()
{
	CTag::CreateTag(this->TAG);
	GetOwner()->SetObjTag(this->TAG);
}

//==========================================================
// 初期化
//==========================================================
void CLightCollisionManager::Init()
{

}

//==========================================================
// 更新
//==========================================================
void CLightCollisionManager::Update()
{

	for (auto it = m_aLights.begin(); it != m_aLights.end(); ++it)
	{
		// 取得
		const auto& a = (*it);
		// リフレッシュ処理
		a->Refresh();

		for (auto itB = m_aLights.begin(); itB != m_aLights.end(); ++itB)
		{
			if (it == itB)
				continue;

			const auto& b = *(*itB);
			// 範囲確認
			a->Check(b);
		}
	}


}
