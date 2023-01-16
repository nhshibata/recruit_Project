//=========================================================
// [BaseComponent.h]
//---------------------------------------------------------
// 作成:2022/06/07
//---------------------------------------------------------
// 基底クラス
// スマートポインタを使いやすくするために作成したｸﾗｽ
//=========================================================

//--- インクルードガード
#ifndef __BASE_COMPONENT_H__
#define __BASE_COMPONENT_H__

//--- インクルード部
#include <typeinfo>
#include <CoreSystem/Util/stl.h>
#include <CoreSystem/Util/cerealCommon.h>

namespace MySpace
{
	namespace Game
	{
		class CBaseComponent
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				//archive(CEREAL_NVP(m_pSelfPtr));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				//archive(CEREAL_NVP(m_pSelfPtr));
			}

		public:
			// *@ｸﾗｽ名取得
			_NODISCARD inline std::string GetName() { return typeid(*this).name(); }
			_NODISCARD inline const type_info& GetType() { return typeid(*this); }

#ifdef BUILD_MODE
		public:
			// ImGuiを使ったデバッグ表示用関数
			virtual void ImGuiDebug() {};

#endif // BUILD_MODE

		};
	}
}

#endif // __BASE_COMPONENT_H__