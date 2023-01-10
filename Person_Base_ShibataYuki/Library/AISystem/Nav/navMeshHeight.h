//==========================================================
// [navMeshHeight.h]
//----------------
// 作成:2022/12/28
//----------------
// 高さ用
// ひとまず分割
//==========================================================

#ifndef __NAV_MESH_HEIGHT_H__
#define __NAV_MESH_HEIGHT_H__

//--- インクルード部
#include <AISystem/Nav/navMeshBake.h>

namespace AI
{
	class CNavMeshHeight : public CNavMeshBake
	{
	private:
		//--- メンバ変数
		float m_fHeight;

	private:
		// *@ポイント間の距離取得関数
		float CalcDist(Vector3 a, Vector3 b);
		// *@ウェイト値計算
		float CalcStepWeight(Vector3 a, Vector3 b, float height);

	public:
		//--- メンバ関数
		CNavMeshHeight();
		CNavMeshHeight(Vector3 pos, int grid, float margin, float height);
		~CNavMeshHeight();

		void Bake(const float startPosY, const float endPosY);

		// *@マップﾃﾞｰﾀからルートを構築
		MapRoute GetRoute(Vector3 start, Vector3 end, float height);

		inline void SetHeight(const float height) { m_fHeight = height; }
	};

}

#endif // !__NAV_MESH_HEIGHT_H__


