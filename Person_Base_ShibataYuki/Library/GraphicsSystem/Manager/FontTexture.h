//=========================================================
// [FontTexture.h] 
// �쐬: 2022/08/04
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __FONT_TEXTURE_H__
#define __FONT_TEXTURE_H__

//--- �C���N���[�h��
#include <d3d11.h>
#include <CoreSystem/system.h>
#include <CoreSystem/Util/stl.h>

namespace MySpace
{
	namespace Graphics
	{
		class CFontTexture : public CAppSystem< CFontTexture>
		{
			friend class Application;
			friend class CAppSystem<CFontTexture>;

		public:
			//--- �\���̒�`
			struct STCharaData
			{
				ID3D11ShaderResourceView* pTex;
				int Width;
				int Height;
				int BMPWidth;
				int BMPHeight;
				int OffSetX;
				int OffSetY;
			};
		private:
			// ��ر�
			using CharTexMap = std::map<LPCWSTR, STCharaData>;		// �Ȃ�̕������A�ǂ�ȃp�����[�^��
			using CharTexPair = std::pair<LPCWSTR, STCharaData>;
			using FontMap = std::map<std::wstring, CharTexMap>;		// �t�H���g
			using FontPair = std::pair<std::wstring, CharTexMap>;
		private:
			HDC m_hdc;					// 
			HFONT m_hFont;				// 
			HFONT m_hOldFont;			// 

			FontMap m_StringMap;		// �i�[�p
			std::wstring m_FontName;	// ���ݕێ��t�H���g��
		
		public:
			//--- �����o�֐�
			CFontTexture();
			~CFontTexture();

			HRESULT Init();
			void Uninit();

			void DesignFont(std::string fontPath, std::wstring fontName);	// �t�H���g�ύX�p ���ύX�ɂ͂Ă����Ă��Ȃ����

			// ����ø����̍쐬(��Ȃ�l�r �o����)
			// *@������:wstring L""
			CFontTexture::STCharaData CreateTex(LPCWSTR character, std::wstring fontName = std::wstring());
			// ��������󂯎��Aø�����Ԃ�
			std::vector<CFontTexture::STCharaData> GetString(std::wstring text, std::wstring fontName = std::wstring());	

		};
	}
}

#endif //__FONT_TEXTURE_H__