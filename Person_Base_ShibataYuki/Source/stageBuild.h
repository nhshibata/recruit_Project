//========================================================
// [stageBuild.h]
//------------------------
// 作成:2023/01/02
//------------------------
//
//========================================================

#ifndef __STAGE_BUILD_H__
#define __STAGE_BUILD_H__

//--- インクルード部
#include <CoreSystem/File/resourceCSV.h>
#include <CoreSystem/Math/MyMath.h>

namespace Spell
{

	enum class EStageNo
	{
		NONE = -1,

		MAX
	};

	class CStageBuild
	{
	private:
		MySpace::MyMath::Vector3 m_vStartPos;
		float m_fMarzin;

	public:
		CStageBuild(MySpace::MyMath::Vector3 startPos, float marzin);
		~CStageBuild();

		bool Build(CResourceCSV* csv);

	};
}

#endif // !__STAGE_BUILD_H__
