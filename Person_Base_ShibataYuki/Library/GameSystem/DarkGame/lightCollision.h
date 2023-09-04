//==========================================================
// [lightCollision.h]
//----------------------------------------------------------
// 作成:2023/05/19 
//----------------------------------------------------------
// ライトの範囲確認用
// 派生するための基底クラス
//==========================================================

#ifndef __LIGHT_COLLISION_H__
#define __LIGHT_COLLISION_H__

#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Light/pointLight.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CLightCollision : public CComponent
	{
	private:
		std::weak_ptr<CPointLight> m_pLight;

	public:
		//--- メンバ関数
		CLightCollision();
		CLightCollision(CGameObject::Ptr ptr);
		virtual ~CLightCollision();

		void OnLoad()override;
		virtual void Awake();
		void Init();
		virtual void Update();

		// *@情報のクリア
		virtual void Refresh() = 0;

		// *@条件を満たしているか
		virtual bool IsRange(std::string spec) = 0;

		// *@Hit時に外部から呼び出してもらう
		// *@継承した際にtagで判定を行う想定
		//virtual void SendHit(std::string hit) = 0;

		// *@範囲確認
		// *@確認に条件や同時に行いたい処理がある場合はこの関数をオーバライドする
		virtual bool Check(const CLightCollision& other);

		// *@ライトの取得
		// *@同一オブジェクトにアタッチされている（していなければおかしい)
		CPointLight* GetLightPtr()const { return m_pLight.lock().get(); }
	};

}

#endif // !__LIGHT_COLLISION_H__
