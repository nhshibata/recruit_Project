//=========================================================
// [shaderManager.cpp] 
// �쐬: 2022/06/27
// �X�V: 2022/08/01 ����
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __SHADER_MANAGER_H__
#define __SHADER_MANAGER_H__

//--- �C���N���[�h��
#include <d3d11.h>
#include <functional>
#include <CoreSystem/Math/MyMath.h>
#include <CoreSystem/Util/stl.h>

#include <GraphicsSystem/Shader/vertexShader.h>
#include <GraphicsSystem/Shader/pixelShader.h>
#include <GraphicsSystem/Shader/constantBuffer.h>
#include <GraphicsSystem/Shader/domainShader.h>
#include <GraphicsSystem/Shader/hullShader.h>
#include <GraphicsSystem/Shader/meshBuffer.h>

//--- �萔��`
#define CSO_PATH(name)		FORDER_DIR(Data/shader/name)

namespace MySpace
{
	namespace Graphics
	{
		using ShaderSharedPtr = std::shared_ptr<CGraphicsBase>;
		using ShaderWeakPtr = std::weak_ptr<CGraphicsBase>;
		using VertexShaderSharedPtr = std::shared_ptr<CVertexShader>;
		using VertexShaderWeakPtr = std::weak_ptr<CVertexShader>;
		using PixelShaderSharedPtr = std::shared_ptr<CPixelShader>;
		using PixelShaderWeakPtr = std::weak_ptr<CPixelShader>;
		using ConstantBufferSharedPtr = std::shared_ptr<CConstantBuffer>;
		using ConstantBufferWeakPtr = std::weak_ptr<CConstantBuffer>;
		using MeshBufferSharedPtr = std::shared_ptr<CMeshBuffer>;
		using MeshBufferWeakPtr = std::weak_ptr<CMeshBuffer>;
		using HullShaderSharedPtr = std::shared_ptr<CHullShader>;
		using HullShaderWeakPtr = std::weak_ptr<CHullShader>;
		using DomainShaderSharedPtr = std::shared_ptr<CDomainShader>;
		using DomainShaderWeakPtr = std::weak_ptr<CDomainShader>;
	}
}

namespace MySpace
{
	namespace Graphics
	{

		class CShaderManager
		{
		public:
			//--- �񋓑�
			enum class EShaderType
			{
				//Isoline,
				PointToTriangle,
				//Quad,
				Triangle,
				MAX
			};
			//--- �\����
			struct MatrixBufferType
			{
				DirectX::XMMATRIX world;
				DirectX::XMMATRIX view;
				DirectX::XMMATRIX projection;
			};
			struct TessellationBufferType
			{
				float tessellationAmount;
				XMFLOAT3 padding;
			};

			//--- �G�C���A�X
			using VertexMap = std::map<std::string, VertexShaderSharedPtr>;
			using VertexPair = std::pair<std::string, VertexShaderSharedPtr>;
			using PixelMap = std::map<std::string, PixelShaderSharedPtr> ;
			using PixelPair = std::pair<std::string, PixelShaderSharedPtr>;
			using ConstantMap = std::map<std::string, ConstantBufferSharedPtr> ;
			using ConstantPair = std::pair<std::string, ConstantBufferSharedPtr>;
			using MeshBufferMap = std::map<std::string, MeshBufferSharedPtr> ;
			using MeshBufferPair = std::pair<std::string, MeshBufferSharedPtr>;
			using HullShaderMap = std::map<std::string, HullShaderSharedPtr> ;
			using HullShaderPair = std::pair<std::string, HullShaderSharedPtr>;
			using DomainShaderMap = std::map<std::string, DomainShaderSharedPtr> ;
			using DomainShaderPair = std::pair<std::string, DomainShaderSharedPtr>;

		private:
			//--- �����o�ϐ�
			VertexMap m_aVtxMap;
			PixelMap m_aPixelMap;
			ConstantMap m_aConstantBufferMap;
			MeshBufferMap m_aMeshBuffMap;
			HullShaderMap m_aHullMap;
			DomainShaderMap m_aDomainMap;
			float m_fTessellationAmount;

			std::map<std::string, std::function<void()>> m_aCallbackFunc; // ���O�œo�^���ꂽ�֐��̌Ăяo���p

		public:
			//--- �����o�֐�
			CShaderManager();
			~CShaderManager() = default;

			HRESULT Init();
			void Uninit();
			void Update();

			// *@�R�[���o�b�N�ݒ�
			// *@void(void)�^�ÓI�֐��̂�
			void AddFunction(std::string name, std::function<void(void)> func);

			// *@�R�[���o�b�N�Ăяo��
			void CallBackFuncAndBind(std::string ps, std::string vs);

			// *@PS�o�C���h
			void BindPS(std::string name, UINT slot = 0);
			
			// *@VS�o�C���h
			void BindVS(std::string name, UINT slot = 0);
			
			// *@CB�o�C���h
			void BindCB(std::string name, UINT slot = 0);

			// *@MB�o�C���h
			void BindMB(std::string name, UINT slot = 0);

			// *@�z����T��
			template <class T>
			bool Find(std::string name, std::map<std::string , T> map)
			{
				if (auto it = map.find(name); it != map.end())
					return true;
				return false;
			}

			// *@���O�w�肵���V�F�[�_�[���ް���������
			void CBWrite(std::string name, void* data);
			
			// *@���O�w�肵���V�F�[�_�[�֓��I�ް���������
			void CBWrite(std::string name, void* data, UINT size);

			// *@ConstantBuffer�i�[ : ���O�ݒ�K�{
			void SetCB(std::string name, ConstantBufferSharedPtr ptr);
		
			// *@VertexShader�i�[ : ���O�ݒ�K�{
			void SetVS(std::string name, VertexShaderSharedPtr vs);
			
			// *@PixelShader�i�[ : ���O�ݒ�K�{
			void SetPS(std::string name, PixelShaderSharedPtr ps);
			
			// *@MS�i�[ : ���O�ݒ�K�{
			void SetMB(std::string name, MeshBufferSharedPtr mb);

			inline ConstantBufferSharedPtr GetCB(std::string name)
			{
				return m_aConstantBufferMap[name];
			}
			
			inline PixelShaderSharedPtr GetPS(std::string name)
			{
				return m_aPixelMap[name];
			}

			inline VertexShaderSharedPtr GetVS(std::string name)
			{
				return m_aVtxMap[name];
			}

#ifdef BUILD_MODE
			// *@ps�擾
			std::string ImGuiGetPixelShader(std::string preview);
			// *@vs�擾
			std::string ImGuiGetVertexShader(std::string preview);
#endif // BUILD_MODE

			//// *@�g���Ă��Ȃ�
			//inline void SetTessellation(std::string name, HullShaderSharedPtr hs, DomainShaderSharedPtr ds)
			//{ 
			//	m_pHullMap.insert(HullShaderPair(name, hs)); m_pDomainMap.insert(DomainShaderPair(name, ds));
			//}

			//// *@�g���Ă��Ȃ�
			//inline void SetTessellation(float value)
			//{
			//	m_fTessellationAmount = value; if (m_fTessellationAmount < 1)m_fTessellationAmount = 1;
			//}

			/*void BindTessellation(std::string name)
			{
				if (!Find<HullShaderSharedPtr>(name, m_pHullMap) || !Find<DomainShaderSharedPtr>(name, m_pDomainMap))
					return;
				m_pHullMap[name]->Bind(); m_pDomainMap[name]->Bind();
			}*/

		};

	}
}
#endif // !__SHADER_MANAGER_H__
