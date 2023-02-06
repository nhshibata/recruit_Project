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
//#include <GameSystem/Component/Transform/rectTransform.h>
//#include <GraphicsSystem/Texture/spriteAnimation.h>
#include <CoreSystem/Util/cerealCommon.h>

namespace MySpace
{
	namespace Game
	{
		class CRectTransform;
	}
	namespace Graphics
	{
		class CSpriteAnimation;
	}
}

namespace MySpace
{
	namespace Game
	{
		//--- �N���X��`
		class CPolygonRenderer : public CRenderer
		{
		private:
			// �V���A���C�Y
			friend cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("PolygonRender", cereal::base_class<CRenderer>(this)),
					CEREAL_NVP(m_pSprite), CEREAL_NVP(m_pRectTransform), CEREAL_NVP(m_nZValue)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("PolygonRender", cereal::base_class<CRenderer>(this)),
					CEREAL_NVP(m_pSprite), CEREAL_NVP(m_pRectTransform), CEREAL_NVP(m_nZValue)
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
			std::shared_ptr<MySpace::Graphics::CSpriteAnimation> m_pSprite;	// �`��e�N�X�`��
			std::weak_ptr<CRectTransform> m_pRectTransform;
			int m_nZValue;

		public:
			CPolygonRenderer();
			CPolygonRenderer(std::shared_ptr<CGameObject> owner);
			//CPolygonRenderer(const CPolygonRenderer&);
			~CPolygonRenderer();

			virtual void OnLoad();
			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();

			//--- �Q�b�^�[�E�Z�b�^�[
			std::shared_ptr<CRectTransform> GetRectTransform();
			MySpace::Graphics::CSpriteAnimation* GetSprite();
			std::string GetImageName();
			inline const int GetZ() { return m_nZValue; }

			void SetImageName(std::string name);
			void SetZ(const int z);
			void SetZ(const EZValue z);

#if BUILD_MODE
			virtual void ImGuiDebug();
#endif // BUILD_MODE

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CPolygonRenderer)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(MySpace::Game::CRenderer, MySpace::Game::CPolygonRenderer)

#endif // !__POLYGON_RENDERER_COMPONENT_H__
