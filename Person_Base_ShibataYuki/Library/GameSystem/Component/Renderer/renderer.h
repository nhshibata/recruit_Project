//=========================================================
// [renderer.h] 
// �쐬: 2022/06/27
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __BASE_RENDERER_COMPONENT_H__
#define __BASE_RENDERER_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/component.h>

namespace MySpace
{
	namespace Game
	{
		using namespace MySpace::MyMath;

		class CRenderer : public CComponent
		{
		private:
			// �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("baseRender", cereal::base_class<CComponent>(this)),
						CEREAL_NVP(m_bVisible), CEREAL_NVP(m_vColor),
						CEREAL_NVP(m_strPixelShader), CEREAL_NVP(m_strVertexShader)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("baseRender", cereal::base_class<CComponent>(this)),
						CEREAL_NVP(m_bVisible), CEREAL_NVP(m_vColor),
						CEREAL_NVP(m_strPixelShader), CEREAL_NVP(m_strVertexShader)
				);
			}

		private:
			//--- �����o�ϐ�
			bool m_bVisible;		// �`��t���O
			Color m_vColor;
		protected:
			int m_nDrawIdx = -1;	// DrawSystem�ɓo�^�����ۂɓn�����ID
			std::string m_strPixelShader;
			std::string m_strVertexShader;

		private:
			//--- �����o�֐�
			void DrawRequest();
		public:
			CRenderer()
				:m_bVisible(true), m_nDrawIdx(-1)
			{};
			CRenderer(std::shared_ptr<CGameObject> owner);
			virtual ~CRenderer();

			virtual void OnLoad();
			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();

			//--- �Z�b�^�[�E�Q�b�^�[
			_NODISCARD inline Color GetColor() { return m_vColor; }
			_NODISCARD inline XMFLOAT4 GetColor(int num) { return XMFLOAT4(m_vColor.r, m_vColor.g, m_vColor.b, m_vColor.a); }
			// *@���ޯ���擾
			_NODISCARD inline UINT GetIdx() { return m_nDrawIdx; }
			inline std::string GetPSName() { return m_strPixelShader; }
			inline std::string GetVSName() { return m_strVertexShader; }

			void SetLayer(int value);
			inline void SetColor(Color color) { m_vColor = color; }
			inline void SetVisible(bool value) { m_bVisible = value; }
			inline void SetPSName(std::string value) { m_strPixelShader = value; }
			inline void SetVSName(std::string value) { m_strVertexShader = value; }

			// *�`���Ԋm�F
			inline bool IsVisible() { return m_bVisible; }

#ifdef BUILD_MODE

			virtual void ImGuiDebug();

#endif // BUILD_MODE
		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CRenderer)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(CBaseComponent<CComponent>, MySpace::CRenderer)

#endif // !__RENDERER_H_
