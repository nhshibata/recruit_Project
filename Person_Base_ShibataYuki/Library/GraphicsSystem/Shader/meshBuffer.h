//=========================================================
// [meshBuffer.h] 
// 作成: 2022/08/05
//---------------------------------------------------------
//=========================================================

#ifndef __MESH_BUFFER_H__
#define __MESH_BUFFER_H__

//--- インクルード部
#include <GraphicsSystem/Shader/GraphicsBase.h>
#include <Application/main.h>

namespace MySpace
{
	namespace Graphics
	{
		class CMeshBuffer : public CGraphicsBase
		{
		public:
			enum INDEX
			{
				INDEX8 = 1,
				INDEX16 = 2,
				INDEX32 = 4
			};
			enum TOPOLOGY
			{
				LINE_LIST,
				TRIANGLE_STRIP,
				TRIANGLE_LIST
			};
			struct Description
			{
				const void* pVtx;
				UINT vtxSize;
				UINT vtxCount;
				bool isWrite;
				const void* pIdx;
				INDEX idxSize;
				UINT idxCount;
				TOPOLOGY topology;
				Description()
					:pVtx(nullptr), vtxSize(0), vtxCount(0), isWrite(false), pIdx(nullptr), 
					idxSize(INDEX::INDEX16), idxCount(0), topology(TOPOLOGY::LINE_LIST)
				{};
			};
		private:
			//--- メンバ変数
			ID3D11Buffer* m_pVtxBuffer;
			ID3D11Buffer* m_pIdxBuffer;
			Description m_stDesc;

		public:
			//--- メンバ関数
			CMeshBuffer(Description Desc);
			CMeshBuffer();
			~CMeshBuffer();

			void Bind(UINT slot = 0);
			void Write(void* data);

			HRESULT Make(Description desc);

			HRESULT CreateIndex();
			HRESULT CreateVertex();

			//--- ゲッター・セッター
			ID3D11Buffer* GetVtxBuffer() { return m_pVtxBuffer; }
			ID3D11Buffer* GetIdxBuffer() { return m_pIdxBuffer; }
		};
	}
}

#endif // !__MESH_BUFFER_H__
