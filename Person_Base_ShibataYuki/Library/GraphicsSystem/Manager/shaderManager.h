//=========================================================
// [shaderManager.cpp] 
// 作成: 2022/06/27
// 更新: 2022/08/01 実装
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __SHADER_MANAGER_H__
#define __SHADER_MANAGER_H__

//--- インクルード部
#include <d3d11.h>
#include <CoreSystem/Math/MyMath.h>
#include <CoreSystem/Util/stl.h>
#include <CoreSystem/Singleton.h>
#include <GraphicsSystem/Shader/vertexShader.h>
#include <GraphicsSystem/Shader/pixelShader.h>
#include <GraphicsSystem/Shader/constantBuffer.h>
#include <GraphicsSystem/Shader/domainShader.h>
#include <GraphicsSystem/Shader/hullShader.h>
#include <GraphicsSystem/Shader/meshBuffer.h>

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
		class CShaderManager : public CSingleton<CShaderManager>
		{
			friend class CSingleton<CShaderManager>;
		public:
			//--- 列挙体
			enum class EShaderType
			{
				//Isoline,
				PointToTriangle,
				//Quad,
				Triangle,
				MAX
			};
			//--- 構造体
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
			//--- エイリアス
			using VertexMap = std::map<std::string, VertexShaderSharedPtr>;
			using VertexPair = std::pair<std::string, VertexShaderSharedPtr>;
			using PixelMap = std::map <std::string, PixelShaderSharedPtr> ;
			using PixelPair = std::pair<std::string, PixelShaderSharedPtr>;
			using ConstantMap = std::map < std::string, ConstantBufferSharedPtr> ;
			using ConstantPair = std::pair<std::string, ConstantBufferSharedPtr>;
			using MeshBufferMap = std::map < std::string, MeshBufferSharedPtr> ;
			using MeshBufferPair = std::pair<std::string, MeshBufferSharedPtr>;
			using HullShaderMap = std::map < std::string, HullShaderSharedPtr> ;
			using HullShaderPair = std::pair<std::string, HullShaderSharedPtr>;
			using DomainShaderMap = std::map < std::string, DomainShaderSharedPtr> ;
			using DomainShaderPair = std::pair<std::string, DomainShaderSharedPtr>;

		private:
			//--- メンバ変数
			VertexMap m_VtxMap;
			PixelMap m_PixelMap;
			ConstantMap m_ConstantMap;
			MeshBufferMap m_MeshBuffMap;
			HullShaderMap m_pHullMap;
			DomainShaderMap m_pDomainMap;
			float m_fTessellationAmount;

		private:
			//--- メンバ関数
			CShaderManager();
			~CShaderManager() = default;

		public:
			HRESULT Init();
			void Uninit();
			void Update();

			// *@今使ってない
			bool SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix,
									XMMATRIX projectionMatrix, float tessellationAmount);
			void Render(EShaderType, std::string cb, std::string vs, std::string ps, std::string mb = "");
			void EndRender();
			void Load();

			// *@PSバインド
			inline void BindPS(std::string name, UINT slot = 0) { 
				if(!Find<PixelShaderSharedPtr>(name, m_PixelMap)){return;} m_PixelMap[name]->Bind(slot); 
			}
			// *@VSバインド
			inline void BindVS(std::string name, UINT slot = 0) {
				if (!Find<VertexShaderSharedPtr>(name, m_VtxMap)) { return; } m_VtxMap[name]->Bind(slot);
			}
			// *@CBバインド
			inline void BindCB(std::string name, UINT slot = 0) { 
				if (!Find<ConstantBufferSharedPtr>(name, m_ConstantMap)) { return; } m_ConstantMap[name]->Bind(slot);
			}
			// *@MBバインド
			inline void BindMB(std::string name, UINT slot = 0) { 
				if (!Find<MeshBufferSharedPtr>(name, m_MeshBuffMap)) { return; } m_MeshBuffMap[name]->Bind(slot);
			}
			void BindTessellation(std::string name) 
			{ 
				if (!Find<HullShaderSharedPtr>(name, m_pHullMap) || !Find<DomainShaderSharedPtr>(name, m_pDomainMap)) { return; }
				m_pHullMap[name]->Bind(); m_pDomainMap[name]->Bind();
			}

			// *@配列内探索
			template <class T>
			bool Find(std::string name, std::map<std::string , T> map)
			{
				if (auto it = map.find(name); it != map.end())
					return true;
				return false;
			}

			// *@名前指定したシェーダーへﾃﾞｰﾀ書き込み
			void ConstantWrite(std::string name, void* data);


			// *@CB格納 : 名前設定必須
			inline void SetConstantBuffer(std::string name, ConstantBufferSharedPtr ptr) { m_ConstantMap.insert(ConstantPair(name, ptr)); }
			// *@VS格納 : 名前設定必須
			inline void SetVS(std::string name, VertexShaderSharedPtr vs) { m_VtxMap.insert(VertexPair(name,vs)); }
			// *@PS格納 : 名前設定必須
			inline void SetPS(std::string name, PixelShaderSharedPtr ps) { m_PixelMap.insert(PixelPair(name,ps)); }
			// *@MS格納 : 名前設定必須
			inline void SetMB(std::string name, MeshBufferSharedPtr mb) { m_MeshBuffMap.insert(MeshBufferPair(name,mb)); }
			// *@使っていない
			inline void SetTessellation(std::string name, HullShaderSharedPtr hs, DomainShaderSharedPtr ds)
			{ 
				m_pHullMap.insert(HullShaderPair(name, hs)); m_pDomainMap.insert(DomainShaderPair(name, ds));
			}

			// *@使っていない
			inline void SetTessellation(float value)
			{
				m_fTessellationAmount = value; if (m_fTessellationAmount < 1)m_fTessellationAmount = 1;
			}
		};
	}
}
#endif // !__SHADER_MANAGER_H__
