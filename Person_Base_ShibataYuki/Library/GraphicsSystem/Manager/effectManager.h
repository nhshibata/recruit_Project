//==============================================
// [effect.h]
//-----------------------
// 2021/12/13 作成 柴田勇貴
//				とりあえず描画確認
// 2021/12/17 更新 引き数でエフェクトをロードできるように変更
// 2022/07/12 グループ制作で作成したcpp,h等ライブラリを移植
//					
//==============================================

#ifndef __EFFECT_MANAGER_H__
#define __EFFECT_MANAGER_H__

// --- インクルード部 ---
//#include <Application/main.h>
#include <GraphicsSystem/Manager/resourceMap.h>
#include <CoreSystem/Singleton.h>
#include <CoreSystem/Math/MyMath.h>
#include <CoreSystem/Util/stl.h>

#include <Effekseer/Effekseer.h>
#include <Effekseer/EffekseerRendererDX11.h>

// Release用(DEBUGではない)
#ifndef _DEBUG

#pragma comment(lib, "Effekseer.lib")
#pragma comment(lib, "EffekseerRendererDX11.lib")

#else
// DEBUG用

#pragma comment(lib, "EffekseerD.lib")
#pragma comment(lib, "EffekseerRendererDX11D.lib")

#endif // !_DEBUG

#define EFFECT_PATH				u"../Data/effect/"
//#define EFFECT_PATH(name)  std::u16string("a")
//#define EFFECT_PATH3(name)  u\FORDER_DIR(Data/effect/\name)

namespace MySpace
{
	namespace Graphics
	{
		class CDXDevice;
	}
}

namespace MySpace
{
	namespace System
	{
		// エイリアス
		using MySpace::Graphics::CResourceMap;

		// クラスの前方参照
		class CEffekseerParam;	
		// --- クラス定義 ---
		class CEffekseer : public CSingleton<CEffekseer>, public CResourceMap<std::u16string, Effekseer::EffectRef>
		{
			friend class CSingleton<CEffekseer>;

		private:
			// 頂点シェーダに送る変換行列
			struct Matrix
			{
				DirectX::XMFLOAT4X4 world;
				DirectX::XMFLOAT4X4 view;
				DirectX::XMFLOAT4X4 proj;
			};
		private:
			// --- エイリアス ---
			using EffectMap = std::map<std::u16string, Effekseer::EffectRef>;			// エフェクトの種類別データ格納
			using EffectMapPair = std::pair<std::u16string, Effekseer::EffectRef>;	// パイル
			using EffectParam = std::vector<CEffekseerParam>;						// 生成するエフェクトのパラメータ

			// --- メンバ変数 ---
			//static CEffekseer* m_pInstance;
			DirectX::XMFLOAT4 m_psLight;
			DirectX::XMFLOAT4 m_psParameter;
			Matrix m_vsMatrix;

			EffekseerRendererDX11::RendererRef m_renderer;
			Effekseer::ManagerRef m_manager;
			// 
			EffectParam m_effectParam;
			int m_nTimer;

			// --- メンバ関数 ---
			void SetMatrix(Effekseer::Matrix43*, XMFLOAT4X4);
			CEffekseer();
			~CEffekseer();
		public:
			//~CEffekseer();

			void Init(ID3D11Device* device, ID3D11DeviceContext* context);
			void Uninit();
			void Update();
			void Draw();

			inline bool Exists(int handle) { return m_manager->Exists(handle);};	// falseは再生していない
			bool Load(std::u16string fileName);
			bool Load(std::vector<std::string> EffectName, std::vector<std::u16string> fileName);	// 2バイト文字の取得
			int Play(std::u16string EffectName, XMFLOAT3 pos);
			int Play(std::u16string EffectName, XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT4 rot = XMFLOAT4(0, 0, 0, 0));
			int Stop(int handle);
			int Move(int handle, XMFLOAT3 pos);

		/*	std::vector<std::u16string> GetNameList() {
				std::vector<std::u16string> ret;
				std::copy(m_ResourceMap.begin(),
					m_ResourceMap.end(),
					std::back_inserter(ret));
				return ret;
			}*/
		};

		// ｸﾗｽ定義
		// パラメータｸﾗｽ
		class CEffekseerParam
		{
		private:
			Effekseer::EffectRef* m_pEffect;	// 描画するエフェクトの種類ポインタ
			Effekseer::Handle m_handle;			// エフェクトのハンドル(マネジャが認識する変数)
			XMFLOAT3 m_vPos;					// 
			XMFLOAT4 m_vRot;					// 
			XMFLOAT3 m_vScale;					// サイズ
			int m_nMaxFream;					// 描画最大時間
			int m_nCurrentFream;				// エフェクト描画カウント
		public:
			CEffekseerParam() :m_vPos(0, 0, 0), m_vRot(0, 0, 0, 0), m_vScale(1, 1, 1)
			{};
			CEffekseerParam(Effekseer::EffectRef* effect, Effekseer::Handle handle, int time)
				:m_vPos(0, 0, 0), m_vRot(0, 0, 0, 0), m_vScale(1, 1, 1), m_pEffect(effect), m_handle(handle), m_nCurrentFream(0), m_nMaxFream(time)
			{};
			~CEffekseerParam()
			{};

			inline void SetPos(float x, float y, float z) { m_vPos = XMFLOAT3(x, y, z); };
			inline void SetPos(XMFLOAT3 pos) { m_vPos = pos; };
			inline void SetRot(XMFLOAT4 rot) { m_vRot = rot; };
			inline void SetScale(XMFLOAT3 size) { m_vScale = size; };
			inline void SetTime(int time) { m_nMaxFream = time; };
			inline void SetHandle(Effekseer::Handle handle) { m_handle = handle; };
			inline XMFLOAT3 GetPos()const { return m_vPos; };
			inline XMFLOAT4 GetRot()const { return m_vRot; };
			inline XMFLOAT3 GetScale()const { return m_vScale; };
			inline Effekseer::EffectRef* GetEffect()const { return m_pEffect; };
			inline Effekseer::Handle GetHandle()const { return m_handle; };
			inline bool IsPlay() { if (++m_nCurrentFream >= m_nMaxFream) { return false; } return true; };

		};
	}
}

#endif // !__EFFEKSEER_MANAGER_H__
