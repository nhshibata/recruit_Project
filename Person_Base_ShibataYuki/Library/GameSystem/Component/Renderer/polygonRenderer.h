//=========================================================
// [polygonRenderer.h] 
// �쐬: 2022/06/28
//---------------------------------------------------------
// 2D�|���S���p�BRectTransform�����B
// �Ȃ���Βǉ����邪�A�Ȃ��ꍇ�Ȃɂ�����ُ̈킪����ƍl����ׂ�
// �����~�X��������?
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __POLYGON_RENDERER_COMPONENT_H__
#define __POLYGON_RENDERER_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Renderer/renderer.h>
#include <GameSystem/Component/Transform/rectTransform.h>
#include <GraphicsSystem/Texture/spriteAnimation.h>

namespace MySpace
{
	namespace Game
	{
		using namespace MySpace::Graphics;

		//--- �N���X��`
		class CPolygonRenderer : public CRenderer
		{
		private:
			// �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("polygonRender", cereal::base_class<CRenderer>(this)),
					CEREAL_NVP(m_pSprite), CEREAL_NVP(m_pRectTransform)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("polygonRender", cereal::base_class<CRenderer>(this)),
					CEREAL_NVP(m_pSprite), CEREAL_NVP(m_pRectTransform)
				);
			}
		public:
			enum class EZValue : int
			{
				BG = 0,
				DEFAULT = 50,
				FOG = 1000,
			};

		private:
			std::unique_ptr<CSpriteAnimation> m_pSprite;	// �`��e�N�X�`��
			RectTransWeakPtr m_pRectTransform;				// �`����
			int m_nZValue;

		public:
			CPolygonRenderer();
			CPolygonRenderer(std::shared_ptr<CGameObject> owner);
			~CPolygonRenderer();

			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();

			inline RectTransSharedPtr GetRectTransform() { return m_pRectTransform.lock(); }
			inline CSpriteAnimation* GetSprite() { return m_pSprite.get(); }
			inline std::string GetImageName() { return m_pSprite->GetImageName(); }
			inline int GetZ() { return m_nZValue; }

			inline void SetRectTransform(RectTransSharedPtr ptr) { m_pRectTransform = ptr; }
			inline void SetImageName(std::string name)
			{
				m_pSprite->SetImage(name);
			}
			void SetZ(const int z);
			void SetZ(const EZValue z);

#if BUILD_MODE
			virtual void ImGuiDebug();
#endif // BUILD_MODE

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CPolygonRenderer)

#endif // !__RENDERER_H_
