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
#include <functional>
#include <CoreSystem/Math/MyMath.h>
#include <CoreSystem/Util/stl.h>

#include <GraphicsSystem/Shader/vertexShader.h>
#include <GraphicsSystem/Shader/pixelShader.h>
#include <GraphicsSystem/Shader/constantBuffer.h>
#include <GraphicsSystem/Shader/domainShader.h>
#include <GraphicsSystem/Shader/hullShader.h>
#include <GraphicsSystem/Shader/meshBuffer.h>

//--- 定数定義
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
			//--- メンバ変数
			VertexMap m_aVtxMap;
			PixelMap m_aPixelMap;
			ConstantMap m_aConstantBufferMap;
			MeshBufferMap m_aMeshBuffMap;
			HullShaderMap m_aHullMap;
			DomainShaderMap m_aDomainMap;
			float m_fTessellationAmount;

			std::map<std::string, std::function<void()>> m_aCallbackFunc; // 名前で登録された関数の呼び出し用

		public:
			//--- メンバ関数
			CShaderManager();
			~CShaderManager() = default;

			HRESULT Init();
			void Uninit();
			void Update();

			// *@コールバック設定
			// *@void(void)型静的関数のみ
			void AddFunction(std::string name, std::function<void(void)> func);

			// *@コールバック呼び出し
			void CallBackFuncAndBind(std::string ps, std::string vs);

			// *@PSバインド
			void BindPS(std::string name, UINT slot = 0);
			
			// *@VSバインド
			void BindVS(std::string name, UINT slot = 0);
			
			// *@CBバインド
			void BindCB(std::string name, UINT slot = 0);

			// *@MBバインド
			void BindMB(std::string name, UINT slot = 0);

			// *@配列内探索
			template <class T>
			bool Find(std::string name, std::map<std::string , T> map)
			{
				if (auto it = map.find(name); it != map.end())
					return true;
				return false;
			}

			// *@名前指定したシェーダーへﾃﾞｰﾀ書き込み
			void CBWrite(std::string name, void* data);
			
			// *@名前指定したシェーダーへ動的ﾃﾞｰﾀ書き込み
			void CBWrite(std::string name, void* data, UINT size);

			// *@ConstantBuffer格納 : 名前設定必須
			void SetCB(std::string name, ConstantBufferSharedPtr ptr);
		
			// *@VertexShader格納 : 名前設定必須
			void SetVS(std::string name, VertexShaderSharedPtr vs);
			
			// *@PixelShader格納 : 名前設定必須
			void SetPS(std::string name, PixelShaderSharedPtr ps);
			
			// *@MS格納 : 名前設定必須
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
			// *@ps取得
			std::string ImGuiGetPixelShader(std::string preview);
			// *@vs取得
			std::string ImGuiGetVertexShader(std::string preview);
#endif // BUILD_MODE

			//// *@使っていない
			//inline void SetTessellation(std::string name, HullShaderSharedPtr hs, DomainShaderSharedPtr ds)
			//{ 
			//	m_pHullMap.insert(HullShaderPair(name, hs)); m_pDomainMap.insert(DomainShaderPair(name, ds));
			//}

			//// *@使っていない
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
