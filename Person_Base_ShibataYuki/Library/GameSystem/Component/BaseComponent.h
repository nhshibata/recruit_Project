//=========================================================
// [BaseComponent.h]
//---------------------------------------------------------
// �쐬:2022/06/07
//---------------------------------------------------------
// ���N���X
// �X�}�[�g�|�C���^���g���₷�����邽�߂ɍ쐬�����׽
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __BASE_COMPONENT_H__
#define __BASE_COMPONENT_H__

//--- �C���N���[�h��
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
			// �V���A���C�Y
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
			// *@�׽���擾
			_NODISCARD inline std::string GetName() { return typeid(*this).name(); }
			_NODISCARD inline const type_info& GetType() { return typeid(*this); }

#ifdef BUILD_MODE
		public:
			// ImGui���g�����f�o�b�O�\���p�֐�
			virtual void ImGuiDebug() {};

#endif // BUILD_MODE

		};
	}
}

#endif // __BASE_COMPONENT_H__