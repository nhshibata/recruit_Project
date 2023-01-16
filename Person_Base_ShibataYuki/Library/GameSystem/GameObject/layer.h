//=========================================================
// [layer.h]
//---------------------------------------------------------
//作成:2022/04/19
//---------------------------------------------------------
// レイヤーｸﾗｽ : 部品ｸﾗｽ
//=========================================================

//--- インクルードガード
#ifndef __LAYER_H__
#define __LAYER_H__

//--- インクルード部
#include <CoreSystem/Util/cerealCommon.h>
#include <string>

#ifndef NAME_TO
#define NAME_TO(name)		#name
#endif // NAME_TO
#define MAP_SET(key, value)	static_cast<int>(key), NAME_TO(value)

namespace MySpace
{
	namespace Game
	{
		class CLayer
		{
		private:
			//--- シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				archive(CEREAL_NVP(m_nLayer));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(CEREAL_NVP(m_nLayer));
			}
		public:
			//--- 列挙体定義
			enum class E_Layer : int	// レイヤーを増やす
			{
				SKY = 0, 
				BG = 1,			// 背景
				DEFAULT = 10,	// デフォルト
				MODEL = 30,
				UI = 50,		// UI
				FOG = 99,		// 前景(フェード等)
			};

		private:
			//--- メンバ変数
			int m_nLayer;

			// 登録用静的メンバ変数
			static std::map<int, std::string> m_aNoList;

		public:
			//--- メンバ関数
			CLayer() :m_nLayer(static_cast<int>(E_Layer::DEFAULT)) {};
			CLayer(E_Layer layer) :m_nLayer((int)layer) {};
			~CLayer() {};

			//--- ゲッター・セッター
			inline int* GetLayer() { return &m_nLayer; };
			inline void SetLayer(int layer) { m_nLayer = layer; };
			inline void SetLayer(E_Layer layer) { m_nLayer = static_cast<int>(layer); };

			static void Regist(int no, std::string registName)
			{
				if(!m_aNoList.count(no))
					m_aNoList[no] = registName;
			}

		};


	}
}

#endif // !__LAYER_H__
