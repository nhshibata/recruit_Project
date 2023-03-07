//==========================================================
// [volumeManager.h]
//---------------------------------------------------------
// 
//==========================================================

#ifndef __VOLUME_MANAGER_H__
#define __VOLUME_MANAGER_H__

//--- �C���N���[�h��
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

			// *@�w�肵��layer�ƈ�v����volume�̎擾
			// *@�D��x�ɕ��ёւ���
			std::vector<CVolume*> GetVolume(const int layer)const;

		};

	}
}

#endif // !__VOLUME_MANAGER_H__
