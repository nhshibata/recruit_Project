//==========================================================
// [volumeManager.h]
//---------------------------------------------------------
// 
//==========================================================

#ifndef __VOLUME_MANAGER_H__
#define __VOLUME_MANAGER_H__

//--- インクルード部
#include <GameSystem/Component/Renderer/volume.h>
#include <GameSystem/Manager/mapSystemBase.h>


namespace MySpace
{
	namespace Game
	{

		class CVolumeManager : public CMapSystemBase<CVolume*>
		{
		public:
			CVolumeManager();
			~CVolumeManager();

			// *@指定したlayerと一致するvolumeの取得
			// *@優先度に並び替える
			std::vector<CVolume*> GetVolume(const int layer)const;

		};

	}
}

#endif // !__VOLUME_MANAGER_H__
