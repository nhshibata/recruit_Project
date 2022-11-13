//=========================================================
// [textRenerer.h] 
// �쐬: 2022/08/04
//---------------------------------------------------------
// polygon��ʂ��ĕ����`����s��
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __TEXT_RENERER_H__
#define __TEXT_RENERER_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Renderer/renderer.h>
#include <GameSystem/Component/Transform/rectTransform.h>
#include <GraphicsSystem/Manager/FontTexture.h>
#include <CoreSystem/Util/stringConvert.h>

namespace MySpace
{
	namespace Game
	{
		using namespace MySpace::Graphics;

		//--- �N���X��`
		class CTextRenderer : public CRenderer
		{
		private:
			// �V���A���C�Y
#pragma region CEREAL
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				std::string text = WStringToString(m_Text);	// wstring��ۑ����邽��string�Ƃ��ĕϊ�
				archive(cereal::make_nvp("TextRender", cereal::base_class<CRenderer>(this)),
					CEREAL_NVP(m_fOffset), CEREAL_NVP(m_bVerticalWrit),
					CEREAL_NVP(text)
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				std::string text; // ������󂯎��
				archive(cereal::make_nvp("TextRender", cereal::base_class<CRenderer>(this)),
					CEREAL_NVP(m_fOffset), CEREAL_NVP(m_bVerticalWrit),
					CEREAL_NVP(text)
				);
				// �󂯎��������������C�h�����ɕϊ�
				m_Text = StringToWString(text);
			}
#pragma endregion

		private:
			//--- �����o�ϐ�
			RectTransWeakPtr m_pRectTransform;					// �`����
			std::vector<CFontTexture::STCharaData> m_pTexList;	// �����Ԗڂ̕����擾�Ȃǂ����₷��vector
			
			std::wstring m_Font;		// �ǂݍ��݃t�H���g��
			std::wstring m_Text;		// ������
			size_t m_uOldTextSize;		// ������T�C�Y
			float m_fOffset;			// ������Ԋu
			bool m_bVerticalWrit;		// �c�������ۂ�

		public:
			//--- �����o�֐�
			CTextRenderer() {};
			CTextRenderer(std::shared_ptr<CGameObject> owner);
			~CTextRenderer();

			virtual void Awake();
			virtual void Init();
			virtual void Update();
			virtual bool Draw();

			//--- �Q�b�^�[�E�Z�b�^�[
			RectTransSharedPtr GetRectTransform() { return m_pRectTransform.lock(); }

			// *@font�ݒ�
			inline void SetFont(std::wstring text) { m_Font = text; }
			// *@�e�L�X�g�ɉ���\�������邩
			// *@���C�h����
			inline void SetTextWString(std::wstring text) { m_Text = text; m_pTexList = CFontTexture::Get().GetString(m_Text, m_Font);}

			// *@�����ԋ����̐ݒ�
			inline void SetOffSet(float offset) { m_fOffset = offset; }

			// *@�c�`��t���O
			inline void Vertical(bool flg) { m_bVerticalWrit = flg; }

#ifdef BUILD_MODE

			virtual void ImGuiDebug();
			// *@���z�L�[��wstring�Ƃ��Ď擾
			std::wstring VKeyToWString(int nKey);

#endif // BUILD_MODE

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CTextRenderer)

#endif //__TEXT_RENERER_H__