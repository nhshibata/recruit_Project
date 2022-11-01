//=========================================================
//作成:2022/04/19 (火曜日)
// レイヤーｸﾗｽ : 部品ｸﾗｽ
//=========================================================

//--- インクルードガード
#ifndef __LAYER_H__
#define __LAYER_H__

//--- インクルード部
#include <CoreSystem/Util/cerealCommon.h>

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
			enum class E_Layer	// レイヤーを増やす
			{
				SKY = 0, 
				BG = 1,			// 背景
				DEFAULT = 2,	// デフォルト
				MODEL = 30,			// UI
				UI = 50,			// UI
				FOG = 99,		// 前景(フェード等)
			};
		private:
			int m_nLayer;
		public:
			CLayer() :m_nLayer(static_cast<int>(E_Layer::DEFAULT)) {};
			CLayer(E_Layer layer) :m_nLayer((int)layer) {};
			~CLayer() {};
			inline int* GetLayer() { return &m_nLayer; };
			inline void SetLayer(int layer) { m_nLayer = layer; };
			inline void SetLayer(E_Layer layer) { m_nLayer = static_cast<int>(layer); };
		};
	}
}

#endif // !__LAYER_H__
