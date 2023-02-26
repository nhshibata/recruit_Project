//==========================================================
// [postProcess.h]
//---------------------------------------------------------
// 作成:2023/02/15 
//---------------------------------------------------------
// 基底クラス
//==========================================================

#ifndef __POST_PROCESS_H__
#define __POST_PROCESS_H__

//--- インクルード部
#include <d3d11.h>
#include <GraphicsSystem/DirectX/renderTarget.h>

namespace MySpace
{
	namespace Graphics
	{
		class CPostProcess
		{
		private:

		public:
			virtual ID3D11ShaderResourceView* GetResource() = 0;

		};
	}
}

#endif // !__POST_PROCESS_H__


