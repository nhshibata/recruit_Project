//==========================================================
// [imageSwitch.h]
//----------------------------------------------------------
// çÏê¨:2023/06/08
// 
//==========================================================

#ifndef __IMAGE_SWITCH_H__
#define __IMAGE_SWITCH_H__

#include <GameSystem/DarkGame/animUI.h>
#include <GraphicsSystem/Texture/imageResource.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CImageSwitch : public CAnimeUI
	{
	public:
		enum class EDispImage
		{
			Start,
			Over,
			Goal,
			Max,
		};
		static constexpr LPCSTR TAG = "MainUI";
	private:
		std::vector<std::weak_ptr<MySpace::Graphics::CImageResource>> m_aImages;

	public:
		//--- ÉÅÉìÉoä÷êî
		CImageSwitch();
		CImageSwitch(CGameObject::Ptr ptr);

		/*void OnLoad() { Awake(); };*/
		virtual void Awake();
		void Init();
		virtual void Update();

		void SwicthImage(const EDispImage image);
		void StartAnim(const EDispImage image);
	};

}

#endif // !__IMAGE_SWITCH_H__
