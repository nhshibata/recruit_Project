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

			// *@�w�肵��layer�ƈ�v���Ă���volume��T��
			// *@�T����volume��bit���i�[���A�Ԃ�
			// *@�ړI:GBuffer���쐬����A���Ȃ��̖ڈ�
			const int GetBit(const int layer);

			// *@���C���[�ƈ�v����volume��ID��ǉ�����
			void AddRendererCash(const int nLayer, const int nID);

			// *@�������Ă���Volume�R���|�[�l���g�̃L���b�V�����N���A
			void ResetRendererCash();
		};

	}
}

#endif // !__VOLUME_MANAGER_H__
