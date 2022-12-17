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
		private:
			std::shared_ptr<CSpriteAnimation> m_pSprite;	// �`��e�N�X�`��
			RectTransWeakPtr m_pRectTransform;				// �`����

		public:
			CPolygonRenderer() {};
			CPolygonRenderer(std::shared_ptr<CGameObject> owner);
			~CPolygonRenderer();

			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();

			
			inline void SetRectTransform(RectTransSharedPtr ptr) { m_pRectTransform = ptr; }
			
			inline void SetImageName(std::string name)
			{
				m_pSprite->SetImage(name);
			}
			inline RectTransSharedPtr GetRectTransform() { return m_pRectTransform.lock(); }
			inline std::shared_ptr<CSpriteAnimation> GetSprite() { return m_pSprite; }
			inline std::string GetImageName() { return m_pSprite->GetImageName(); }

			virtual void ImGuiDebug();
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CPolygonRenderer)

#endif // !__RENDERER_H_
