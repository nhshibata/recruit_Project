//=========================================================
// [sceneSerealize.h]
//---------------------------------------------------------
// �쐬:2023/03/17
//---------------------------------------------------------
// �V�[���̃V���A���C�Y�𕪊�
// cereal�̊֌W��A�V���A���C�Y�A�f�V���A���C�Y�͌Ăяo�����ł��ׂ�
//=========================================================

#ifndef __SCENE_SEREALIZE_H__
#define __SCENE_SEREALIZE_H__

//--- �C���N���[�h��
#include <CoreSystem/Util/cerealCommon.h>
#include <GameSystem/Scene/scene.h>
#include <GameSystem/Manager/instantResourceManager.h>
#include <GameSystem/components.h>

namespace MySpace
{
	namespace SceneManager
	{
		// �V���A���C�Y�p�׽
		// �ꎞ�I�ɑޔ�������Memento�p�^�[��?
		struct CSceneSerealizeData
		{
#pragma region Cereal
			friend cereal::access;
			template <typename Archive>
			void save(Archive & archive, std::uint32_t const version) const
			{
				archive(CEREAL_NVP(m_SceneName), CEREAL_NVP(m_aGameObjectManager), CEREAL_NVP(m_resource));
			}
			template <typename Archive>
			void load(Archive & archive, std::uint32_t const version)
			{
				archive(CEREAL_NVP(m_SceneName), CEREAL_NVP(m_aGameObjectManager), CEREAL_NVP(m_resource));
			}
#pragma endregion

			std::string m_SceneName;
			std::list<std::shared_ptr<MySpace::Game::CGameObject>> m_aGameObjectManager;
			MySpace::Game::CInstantResourceManager m_resource;

			void SaveStorage(CScene* scene);

			bool ReadDataPass(std::shared_ptr<CScene> scene);

		};
	}
}

#endif // !__SCENE_SEREALIZE_H__


