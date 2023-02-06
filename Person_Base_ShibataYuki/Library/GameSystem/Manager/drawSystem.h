//=========================================================
// [drawSystem.h]
// �h���N���X
//---------------------------------------------------------
// �쐬:2022/06/07 �׽���ύX���邩��
// �X�V:2022/11/09 �N���X���ύX(DrawManager) -> (drawSystem)
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __DRAW_SYSTEM_H__
#define __DRAW_SYSTEM_H__

//--- �C���N���[�h��
#include <GameSystem/Manager/mapSystemBase.h>
#include <GameSystem/Shader/depthShadow.h>
#include <GraphicsSystem/Shader/instancingData.h>

#include <DirectXMath.h>

namespace MySpace
{
	namespace Game
	{
		class CRenderer;
		class CPolygonRenderer;
	}
	namespace Graphics
	{
		class CMesh;
	}
}

namespace MySpace
{
	namespace Game
	{
		using MySpace::Game::CRenderer;
		using MySpace::Graphics::CMesh;

		//--- �N���X��`
		class CDrawSystem : public CMapSystemBase<std::weak_ptr<CRenderer>>
		{
			friend class CRenderer;
			friend class CPolygonRenderer;
		private:
			struct STMeshData
			{
				CMesh* pMesh;
				std::vector<MySpace::Graphics::RENDER_DATA> aData;
			};

			//--- �G�C���A�X
			using PolygonRenderWeakList = std::vector<std::weak_ptr<CPolygonRenderer>>;
			using InstancingMap = std::map<std::string, std::vector<MySpace::Graphics::RENDER_DATA>>;
			using InstancingMeshMap = std::map<std::string, STMeshData>;
			
		private:
			//--- �����o�ϐ�
			bool m_bIsSortNecessary;					// ������s�t���O
			PolygonRenderWeakList m_aPolygonList;		// �Ǘ����Ă���map���\�[�g�������ʂ�����
			InstancingMap m_aInstancingModelMap;		// �C���X�^���V���O�`��i�[�p
			InstancingMeshMap m_aInstancingMeshMap;		// �C���X�^���V���O�`��i�[�p
			std::unique_ptr<Game::CDepthShadow> m_pDepthShadow;	// �[�x�������ݗp

#if BUILD_MODE
			struct STDebugMeshData
			{
				CMesh* pMesh;
				std::vector<DirectX::XMFLOAT4X4> mtx;
			};
			// �m�F�p�ϐ�
			int m_nSkipCnt;
			int m_nDrawCnt;
			int m_nInstancingCnt;
			bool m_bFrustum;
			bool m_bShadowView;
			std::map<std::string,STDebugMeshData> m_aDebugMeshMap;		// �C���X�^���V���O�`��i�[�p
#endif // _DEBUG

		private:
			//--- �����o�֐�
			// *@�|���S������
			void Sort();
			// *@�����׸�
			inline void SortOn() { m_bIsSortNecessary = true; }
			// *@3D�C���X�^���V���O�e�`��
			void Draw3DShadow();
			// *@3D�C���X�^���V���O�`��
			void Draw3D();
			
		public:
			CDrawSystem();
			~CDrawSystem();

			void Init();
			void Update();

			//--- �`�悷��R���|�[�l���g
			// *@�o�^ override
			_NODISCARD int RegistToSystem(std::weak_ptr<CRenderer> render)
			{
				int ret = CMapSystemBase::RegistToSystem(render);
				return ret;
			}
			
			// *@�o�^
			_NODISCARD int PolygonRegist(std::weak_ptr<CPolygonRenderer> render);

			// *@�j�� overlide
			std::weak_ptr<CRenderer> ExecutSystem(int idx);

			// *@�C���X�^���V���O�`��̂��߂ɏ����i�[����
			inline void SetInstanchingModel(std::string name, MySpace::Graphics::RENDER_DATA data)
			{
				m_aInstancingModelMap[name].push_back(data);
			}
			
			// *@�C���X�^���V���O�`��̂��߂ɏ����i�[����
			void SetInstanchingMesh(std::string name, MySpace::Graphics::RENDER_DATA data, CMesh* mesh);

			// *@�������X�g
			_NODISCARD inline std::vector<std::weak_ptr<CRenderer>> GetList()
			{
				std::vector<std::weak_ptr<CRenderer>> ret;
				for (auto & it : m_aIntMap)
				{
					ret.push_back(it.second);
				}
				return ret;
			}

#if BUILD_MODE

			void ImGuiDebug();
			// *@�C���X�^���V���O�`��̂��߂ɏ����i�[����
			void SetDebugMesh(std::string name, DirectX::XMFLOAT4X4 mtx, CMesh* mesh);
#endif // BUILD_MODE

		};


#define POST_TEST	0
#if POST_TEST

		class CDrawSystemMap
		{
		private:
			std::unordered_map<int, CDrawSystem> m_aLayerMap;

		public:
			CDrawSystemMap();
			~CDrawSystemMap();

			void Update();

			// *@�o�^
			int Regist(std::weak_ptr<CRenderer> add, int layer) 
			{
				return m_aLayerMap[layer].RegistToSystem(add); 
			}

			// *@���O
			bool Remove(int layer, int idx)
			{
				auto res = m_aLayerMap[layer].ExecutSystem(idx);
				return (res.lock() ? false : true);
			}

			// *@���C���[�ړ�
			int Move(int oldLayer, int nextLayer, int idx)
			{
				// �߲���擾
				auto render = m_aLayerMap[oldLayer].IDToData(idx);
				// ����layer���O
				if (Remove(oldLayer, idx))
				{
					// �V�K�o�^
					return Regist(render.lock(), nextLayer);
				}
			}

		};

#endif // POST

	}
}

#endif