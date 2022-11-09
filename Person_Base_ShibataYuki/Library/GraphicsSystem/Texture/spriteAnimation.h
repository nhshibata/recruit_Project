//=========================================================
// [spriteAnimation.h]
// 作成: 2022/07/04
//-------------------------------------
// 命名ミス?
// 2DImageの操作(ﾃｸｽﾁｬの保持、uv等)を行うｸﾗｽ
//=========================================================

#ifndef __SPRITE_ANIMATION_H__
#define __SPRITE_ANIMATION_H__

//--- インクルード部
#include <GraphicsSystem/Texture/imageResource.h>
#include <DebugSystem/debug.h>
#include <CoreSystem/Math/MyMath.h>
#include <CoreSystem/Util/cerealCommon.h>

//--- 定数定義
#define TEXTURE_PATH			FORDER_DIR(Data/Texture/)
#define TEXTURE_NAME(name)		FORDER_DIR(Data/Texture/name)	// \で文字列として扱われるする

// 画像アニメーション用
namespace MySpace
{
	namespace Graphics
	{
		using namespace MySpace::MyMath;

		class CSpriteAnimation 
		{
		private:
			// シリアライズ
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
			int m_nSplitX;			// 分割
			int m_nSplitY;			// 分割
			int m_nAnimNo;			// 現在番号
			int m_nCnt;				// 
			std::vector<ST_FRAME> m_stParam;

			ImageWeakPtr m_pImage;		// ﾃｸｽﾁｬ取得用	取得してなくても描画できるように
			std::string m_ImageName;	// ﾃｸｽﾁｬ名
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
