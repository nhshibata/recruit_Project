//=========================================================
// [drawSystem.h]
// �h���N���X
//---------------------------------------------------------
// 
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __DRAW_SYSTEM_BASE_H__
#define __DRAW_SYSTEM_BASE_H__

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
		class CDrawSystemBase : public CMapSystemBase<std::weak_ptr<CRenderer>>
		{
			friend class CRenderer;
			friend class CPolygonRenderer;
		protected:
			struct STMeshData
			{
				CMesh* pMesh;
				std::vector<MySpace::Graphics::RENDER_DATA> aData;
			};

			//--- �G�C���A�X
			using PolygonRenderWeakList = std::vector<std::weak_ptr<CPolygonRenderer>>;
			using InstancingMap = std::map<std::string, std::vector<MySpace::Graphics::RENDER_DATA>>;
			using InstancingMeshMap = std::map<std::string, STMeshData>;

		protected:
			//--- �����o�ϐ�
			bool m_bIsSortNecessary;							// ������s�t���O
			PolygonRenderWeakList m_aPolygonList;				// �Ǘ����Ă���map���\�[�g�������ʂ�����
			InstancingMap m_aInstancingModelMap;				// �C���X�^���V���O�`��i�[�p
			InstancingMeshMap m_aInstancingMeshMap;				// �C���X�^���V���O�`��i�[�p
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
			std::map<std::string, STDebugMeshData> m_aDebugMeshMap;		// �C���X�^���V���O�`��i�[�p
#endif // _DEBUG

		protected:
			//--- �����o�֐�
			// *@�|���S������
			void Sort();
			// *@�����׸�
			inline void SortOn() { m_bIsSortNecessary = true; }
			// *@3D�C���X�^���V���O�e�`��
			void Draw3DShadow();
			// *@3D�C���X�^���V���O�`��
			virtual void Draw3D();

		protected:
			const std::vector<std::string> GetPSVSName(const std::string name);

		public:
			CDrawSystemBase();
			virtual ~CDrawSystemBase();

			void Init();
			virtual void Update();

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
			void SetInstanchingModel(std::string name, std::string ps, std::string vs, MySpace::Graphics::RENDER_DATA data);

			// *@�C���X�^���V���O�`��̂��߂ɏ����i�[����
			void SetInstanchingMesh(std::string name, std::string ps, std::string vs, MySpace::Graphics::RENDER_DATA data, CMesh* mesh);

			// *@�������X�g
			_NODISCARD std::vector<std::weak_ptr<CRenderer>> GetList();

#if BUILD_MODE
			virtual void ImGuiDebug();
			// *@�C���X�^���V���O�`��̂��߂ɏ����i�[����
			virtual void SetDebugMesh(std::string name, DirectX::XMFLOAT4X4 mtx, CMesh* mesh) = 0;
#endif // BUILD_MODE

		};

	}
}

#endif