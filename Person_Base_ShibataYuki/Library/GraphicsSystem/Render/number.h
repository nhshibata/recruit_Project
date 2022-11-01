//=========================================================
// �쐬:2022/04/24
// �����\�� : ���׽
//=========================================================
#ifndef __NUMBER_H__
#define __NUMBER_H__

//---�C���N���[�h��
#include <GraphicsSystem/Render/polygon.h>
#include <vector>
#include <DirectXTex.h>

//---�萔��`
#define NUMBER		CNumber::Get()

namespace MySpace
{
	namespace Graphics
	{
		using namespace DirectX;

		//---�N���X��`
		class CNumber
		{
		private:
			typedef struct
			{
				XMFLOAT2 pos;
				unsigned Number;
				int width;
				XMFLOAT2 size;
			}STNumberData;
		private:
			ID3D11ShaderResourceView* m_pTexture;
			static CNumber* m_pInstance;
			std::vector<STNumberData> m_numberData;
		public:
			CNumber();
			~CNumber();
			void Set(XMFLOAT2 vPos,		// �\���ʒu(����)
				unsigned uNumber,			// �\�����l
				int nWidth,					// �\������
				float fSizeX,				// 1�����̕�
				float fSizeY);				// 1�����̍���
			void Update();
			void Draw();
			// 
			static void Start() { m_pInstance = new CNumber(); };
			static void End() { delete m_pInstance; };
			static CNumber* Get() { return m_pInstance; };
		};

	}
}

#endif // !__NUMBER_H__
