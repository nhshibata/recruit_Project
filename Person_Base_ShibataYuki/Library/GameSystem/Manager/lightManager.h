//=========================================================
// [lightManager.h]
// --------------------------------------------------------
// ���̉e������������lightMap�쐬�p�X�N���v�g
//=========================================================

#ifndef __LIGHT_MANAGER_H__
#define __LIGHT_MANAGER_H__

//--- �C���N���[�h��
#include <CoreSystem/Util/stl.h>
#include <CoreSystem/Math/myVector.h>
#include <GameSystem/Manager/mapSystemBase.h>
#include <GameSystem/Component/Light/spotLight.h>
#include <GraphicsSystem/Shader/shaderStruct.h>
#include <GraphicsSystem/DirectX/renderTarget.h>

namespace MySpace
{
	namespace Graphics
	{
		class CGBuffer;
	}
}

namespace MySpace
{
	namespace Game
	{

		class CLightManager : public CMapSystemBase<std::weak_ptr<CLight>>
		{
		private:
			std::unique_ptr<Graphics::CRenderTarget> m_pRenderTarget;

		public:
			CLightManager();
			~CLightManager();

			static CLightManager* Get();

			HRESULT InitShader();

			void BeginRender(Graphics::CGBuffer* GBuff)const;
			void SetUpTexture()const;
		};

	}
}

#endif // __LIGHT_MANAGER_H__