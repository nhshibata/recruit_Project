//==========================================================
// [drawSystem.h]
// �h���N���X
//---------------------------
// �쐬:2022/06/07 �׽���ύX���邩��
// �X�V:2022/11/09 �N���X���ύX(DrawManager) -> (drawSystem)
// TODO: 2D�p�̃R���|�[�l���g���󂯎��d�l�̒ǉ�
// TODO: ���C���[�����B�ʏ�̃��f���Ƃ̊֌W�B�w�i��UI�A�O�i�A�𕪂��ĕ`��
//==========================================================

//--- �C���N���[�h�K�[�h
#ifndef __DRAW_SYSTEM_H__
#define __DRAW_SYSTEM_H__

//--- �C���N���[�h��
#include <GameSystem/Manager/mapSystemBase.h>
#include <DirectXMath.h>

namespace MySpace
{
	namespace Game
	{
		class CRenderer;
	}
}

namespace MySpace
{
	namespace Game
	{
		using MySpace::Game::CRenderer;

		//--- �N���X��`
		class CDrawSystem : public CMapSystemBase<std::weak_ptr<CRenderer>>
		{
		private:
			// �G�C���A�X
			using RenderWeak = std::vector<std::weak_ptr<CRenderer>>;
			using InstancingMap = std::map<std::string, std::vector<DirectX::XMFLOAT4X4>>;

		private:
			//--- �����o�ϐ�
			bool m_bIsSortNecessary = false;
			RenderWeak m_pDrawSortList;			// �Ǘ����Ă���map���\�[�g�������ʂ�����ϐ�
			InstancingMap m_aInstancingMap;

#if BUILD_MODE
			// �m�F�p�ϐ�
			int m_nSkipCnt;
			int m_nDrawCnt;
#endif // _DEBUG

		private:
			//--- �����o�֐�
		public:
			CDrawSystem();
			~CDrawSystem();
			void Update();

			//--- �`�悷��R���|�[�l���g
			// *@�o�^ override
			_NODISCARD int RegistToSystem(std::weak_ptr<CRenderer> render)
			{
				int ret = CMapSystemBase::RegistToSystem(render);
				m_pDrawSortList.push_back(render);
				return ret;
			}
			// *@�j�� overlide
			std::weak_ptr<CRenderer> ExecutSystem(int idx)
			{
				auto release = CMapSystemBase::ExecutSystem(idx).lock();

				// �j�����ꂽ���߰��Ă�T���A���񃊃X�g���珜�O
				for (auto it = m_pDrawSortList.begin(); it != m_pDrawSortList.end();)
				{
					if (release == (*it).lock())
					{
						m_pDrawSortList.erase(it);
						break;
					}
				}
				return release;
			}

			// *@�C���X�^���V���O�`��̂��߂ɏ����i�[����
			void SetInstanching(std::string name, DirectX::XMFLOAT4X4 mtx)
			{
				m_aInstancingMap[name].push_back(mtx);
			}

			// *@
			_NODISCARD inline std::vector<std::weak_ptr<CRenderer>> GetList()
			{
				return m_pDrawSortList;
			}

#if BUILD_MODE

			void ImGuiDebug();

#endif // BUILD_MODE

		};
	}
}
#endif