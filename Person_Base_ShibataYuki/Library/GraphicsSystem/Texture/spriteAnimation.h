//=========================================================
// [spriteAnimation.h]
// �쐬: 2022/07/04
//-------------------------------------
// �����~�X?
// 2DImage�̑���(ø����̕ێ��Auv��)���s���׽
//=========================================================

#ifndef __SPRITE_ANIMATION_H__
#define __SPRITE_ANIMATION_H__

//--- �C���N���[�h��
#include <GraphicsSystem/Texture/imageResource.h>
#include <DebugSystem/debug.h>
#include <CoreSystem/Math/MyMath.h>
#include <CoreSystem/Util/cerealCommon.h>

//--- �萔��`
#define TEXTURE_PATH			FORDER_DIR(Data/Texture/)
#define TEXTURE_NAME(name)		FORDER_DIR(Data/Texture/name)	// \�ŕ�����Ƃ��Ĉ����邷��

// �摜�A�j���[�V�����p
namespace MySpace
{
	namespace Graphics
	{
		using namespace MySpace::MyMath;

		class CSpriteAnimation 
		{
		private:
			// �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				archive(CEREAL_NVP(m_nSplitX), CEREAL_NVP(m_nSplitY),CEREAL_NVP(m_nAnimNo),CEREAL_NVP(m_nCnt),
						CEREAL_NVP(m_ImageName) /*,CEREAL_NVP(m_pImage)*/
					);
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(CEREAL_NVP(m_nSplitX), CEREAL_NVP(m_nSplitY), CEREAL_NVP(m_nAnimNo), CEREAL_NVP(m_nCnt),
					CEREAL_NVP(m_ImageName) /*,CEREAL_NVP(m_pImage)*/
				);
			}
		public:
			struct ST_FRAME
			{
				int nAnimNo;
				int nFrame;
				ST_FRAME():nAnimNo(0),nFrame(4) {}
			};
		private:	
			int m_nSplitX;			// ����
			int m_nSplitY;			// ����
			int m_nAnimNo;			// ���ݔԍ�
			int m_nCnt;				// 
			std::vector<ST_FRAME> m_stParam;

			ImageWeakPtr m_pImage;		// ø����擾�p	�擾���ĂȂ��Ă��`��ł���悤��
			std::string m_ImageName;	// ø�����
		public:
			CSpriteAnimation();
			~CSpriteAnimation();
			void Update();

			inline void SetImagePtr(ImageWeakPtr image) { m_pImage = image; };
			void SetImage(std::string name);

			inline ImageWeakPtr GetImage() { return m_pImage; }
			inline std::string GetImageName() { return m_ImageName; }
			inline ID3D11ShaderResourceView* GetTexture() { return m_pImage.lock() ? m_pImage.lock()->GetSRV() : nullptr; }
			Vector2 GetUV();
			Vector2 GetUV(int no);
			Vector2 GetFrameSize();
			inline XMINT2 GetSplit() { return { m_nSplitX ,m_nSplitY }; }

			inline std::vector<ST_FRAME>* GetFrameParam() { return &m_stParam; }

#ifdef BUILD_MODE

			void ImGuiDebug();

#endif // BUILD_MODE
		};
	}
}
#endif // !__SPRITE_ANIMATION_H__
