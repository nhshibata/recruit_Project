//=========================================================
// [GraphicsBase.h] 
// �쐬: 2022/08/01
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __GRAPHICS_BASE_H__
#define __GRAPHICS_BASE_H__

//--- �C���N���[�h��
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
