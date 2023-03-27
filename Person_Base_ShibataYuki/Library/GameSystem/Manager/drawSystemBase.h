//=========================================================
// [drawSystem.h]
//---------------------------------------------------------
// �쐬:2023/03/??
// �X�V:2023/03/15 �V�F�[�_�[�����\���̂ɕύX
//---------------------------------------------------------
// �h���N���X
// �e�`���ʏ�`��A��{�����̒�`�A����
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __DRAW_SYSTEM_BASE_H__
#define __DRAW_SYSTEM_BASE_H__

//--- �C���N���[�h��
#include <GameSystem/Manager/mapSystemBase.h>
#include <GraphicsSystem/DirectX/depthShadow.h>
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
#pragma region Struct
			//--- �\���̒�`
			// ID���L���b�V������
			struct ST3DData
			{
				std::vector<int> aID;
			};
			// �p�����A�`��̂��߂��߲����ێ�
			// �����̊֌W�ŁAnullptr��delete�̐S�z�͂��Ȃ�
			struct STMeshData : public ST3DData
			{
				CMesh* pMesh;
			};

			// �Q�[���I�u�W�F�N�g����n�����`����i�[
			// �V�F�[�_�[�ؑւɑΉ����邽��
			// map��Key�Ƃ��Ďg��
			struct STDrawInfo
			{
				std::string strName;
				std::string strPixel;
				std::string strVertex;

				// *@�R���X�g���N�^
				STDrawInfo() 
				{}
				// *@�R���X�g���N�^
				STDrawInfo(std::string name, std::string ps, std::string vs)
					:strName(name),strPixel(ps), strVertex(vs)
				{}

				// *@���Z�q�̃I�[�o�[���[�h
				// *@map�͑}���ʒu���Q�Ƃ��邽�߂ɔ�r���邽�߁A�쐬���Ȃ���΂Ȃ�Ȃ�
				bool operator <(const STDrawInfo& other)const
				{
					if (strName != other.strName)
						return strName < other.strName;
					else if (strPixel != other.strPixel)
						return strPixel < other.strPixel;
					else
						return strVertex < other.strVertex;
				}
				// *@�����ꂩ�����݂��Ȃ��G���[
				bool IsError()const
				{
					return (strName.empty() | strPixel.empty() | strVertex.empty());
				}
			};
#pragma endregion

			//--- �G�C���A�X
			using PolygonRenderWeakList = std::vector<std::weak_ptr<CPolygonRenderer>>;
			using InstancingMap = std::map<STDrawInfo, ST3DData>;
			using InstancingMeshMap = std::map<STDrawInfo, STMeshData>;

		protected:
			//--- �����o�ϐ�
			bool m_bIsSortNecessary;							// ������s�t���O
			PolygonRenderWeakList m_aPolygonList;				// �Ǘ����Ă���map���\�[�g�������ʂ�����
			InstancingMap m_aInstancingModelMap;				// �C���X�^���V���O�`��i�[�p
			InstancingMeshMap m_aInstancingMeshMap;				// �C���X�^���V���O�`��i�[�p
			std::unique_ptr<Graphics::CDepthShadow> m_pDepthShadow;	// �[�x�������ݗp

#if BUILD_MODE
			// �m�F�p�ϐ�
			int m_nSkipCnt;
			int m_nDrawCnt;
			int m_nInstancingCnt;
			bool m_bFrustum;
			bool m_bShadowView;
			std::map<std::string, std::vector<CMesh*>> m_aDebugMeshMap;		// �C���X�^���V���O�`��i�[�p
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
			// *@3D�I�u�W�F�N�g�̕`��m�F
			virtual void CheckRenderedObjectsIn3D();

		protected:
			// *@�`��˗����ɘA������������𕪊�
			// *@0:ø��������ٖ� 1:pixel 2:vertex
			const STDrawInfo TextSplitToNamePSVS(const std::string name);

		public:
			CDrawSystemBase();
			virtual ~CDrawSystemBase();

			void Init();
			virtual void Update();

			// *@�o�^ override
			_NODISCARD int RegistToSystem(std::weak_ptr<CRenderer> render)
			{
				int ret = CMapSystemBase::RegistToSystem(render);
				return ret;
			}

			// *@�o�^
			_NODISCARD int PolygonRegist(std::weak_ptr<CPolygonRenderer> render);

			// *@�j�� overlide
			bool ExecutSystem(const int idx);

			// *@�C���X�^���V���O�`��̂��߂ɏ����i�[����
			void SetInstanchingModel(std::string name, std::string ps, std::string vs, const int id);

			// *@�C���X�^���V���O�`��̂��߂ɏ����i�[����
			void SetInstanchingMesh(std::string name, std::string ps, std::string vs, const int id, CMesh* mesh);

			// *@�������X�g
			_NODISCARD std::vector<std::weak_ptr<CRenderer>> GetList();

#if BUILD_MODE
			virtual void ImGuiDebug();
			// *@�C���X�^���V���O�`��̂��߂ɏ����i�[����
			void SetDebugMesh(std::string name, CMesh* mesh);
			void ReleaseDebugMesh(const CMesh* mesh);
#endif // BUILD_MODE

		};

	}
}

#endif