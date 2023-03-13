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

			// *@指定したlayerと一致しているvolumeを探す
			// *@探したvolumeのbitを格納し、返す
			// *@目的:GBufferを作成する、しないの目安
			const int GetBit(const int layer);

			// *@レイヤーと一致したvolumeへIDを追加する
			void AddRendererCash(const int nLayer, const int nID);

			// *@所持しているVolumeコンポーネントのキャッシュをクリア
			void ResetRendererCash();
		};

	}
}

#endif // !__VOLUME_MANAGER_H__
