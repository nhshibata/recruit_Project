//========================================================
// [stageBuild]
//------------------------
// 作成:2023/01/02
//------------------------
//
//========================================================

#ifndef __STAGE_MANAGER_COMPONENT_H__
#define __STAGE_MANAGER_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/component.h>
#include <CoreSystem/File/resourceCSV.h>

namespace Spell
{

	class CStageManager : public CComponent
	{
	public:
		struct GridPoint
		{
			int x, y;
			GridPoint() 
			{
				x = y = 0;
			};
			GridPoint(int x1, int y1)
			{
				x = x1;
				y = y1;
			};
		};

	private:
		CResourceCSV* m_pCSV;	// csv読み込み兼保持
		Vector3 m_vCenter;		// 中心座標
		float m_fMargin;		// グリッド毎の差

	public:
		CStageManager();
		CStageManager(CGameObject::Ptr);
		~CStageManager();

		void Awake();

		void StartBuild(std::string file);

		// *@stageData情報取得(Vector3)
		int GetData(const Vector3 pos);
		
		// *@stageData情報取得(int)
		int GetData(const int idx);
		
		// *@インデックスからxy取得
		GridPoint IndexToPoint(int index);

		// *@座標からxyへ
		GridPoint PosToPoint(const Vector3 pos);

		// *@xyからインデックス変換
		int PointToIndex(GridPoint point)const;

		// *@xyから座標変換
		Vector3 PointToPos(GridPoint point);
		
	};

}

#endif //!__STAGE_MANAGER_COMPONENT_H__