//=========================================================
// [GraphicsBase.h] 
// 作成: 2022/08/01
//---------------------------------------------------------
//=========================================================

//--- インクルードガード
#ifndef __GRAPHICS_BASE_H__
#define __GRAPHICS_BASE_H__

//--- インクルード部
#include <d3d11.h>
#include <string>
#include <Application/main.h>

namespace MySpace
{
	namespace Graphics
	{
		class CGraphicsBase
		{
		private:

		public:
			CGraphicsBase() = default;
			virtual ~CGraphicsBase() = default;

			virtual void Bind(UINT slot) = 0;
			//virtual HRESULT Make(std::string) = 0;
		};
	}
}

#endif // !__SHADER_BASE_H__
