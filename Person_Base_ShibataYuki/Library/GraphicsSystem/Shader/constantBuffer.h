//=========================================================
// [constantBuffer.h] 
// �쐬: 2022/08/01
//---------------------------------------------------------
//=========================================================

#ifndef __CONSTANT_BUFFER_H__
#define __CONSTANT_BUFFER_H__

//--- �C���N���[�h��
#include <GraphicsSystem/Shader/GraphicsBase.h>

namespace MySpace
{
	namespace Graphics
	{
		class CConstantBuffer : public CGraphicsBase
		{
		public:
			using Ptr = std::shared_ptr< CConstantBuffer>;
			using PtrW = std::weak_ptr< CConstantBuffer>;
		public:
			enum class EType
			{
				Vertex,
				Pixel,
				All,
				MAX,
			};
		private:
			//--- �����o�ϐ�
			ID3D11Buffer* m_pBuffer;
			UINT m_uSlot;
			EType m_eType;

		public:
			//--- �����o�֐�
			CConstantBuffer();
			virtual ~CConstantBuffer();

			void Bind(UINT slot = 0);
			void Write(void* data);
			HRESULT DynamicWrite(void* data, UINT size);
			
			HRESULT Make(UINT size, UINT slot, EType type, D3D11_SUBRESOURCE_DATA* init = nullptr);
			HRESULT MakeCPU(UINT size, UINT slot, EType type, D3D11_SUBRESOURCE_DATA* init = nullptr);

			inline ID3D11Buffer* GetBuffer() { return m_pBuffer; }
		};
	}
}

#endif // !__CONSTANT_BUFFER_H__
