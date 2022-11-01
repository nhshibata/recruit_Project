//=========================================================
// [spriteAnimation.cpp]
// 作成: 2022/07/04
//=========================================================
#include <GraphicsSystem/Texture/spriteAnimation.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Graphics;
using namespace MySpace::System;


CSpriteAnimation::CSpriteAnimation() 
	:m_nAnimNo(0),m_nSplitX(1),m_nSplitY(1),m_nCnt(3)
{
	
}
CSpriteAnimation::~CSpriteAnimation()
{
	//m_pImage.reset();
}
void CSpriteAnimation::Update()
{
	if (m_nSplitX == 1 && m_nSplitY == 1)
		return;

	--m_nCnt;
	if (m_nCnt <= 0)
	{
		++m_nAnimNo;
		if (m_stParam.size() <= m_nAnimNo)
			m_nAnimNo = 0;
		else
			m_nCnt = m_stParam[m_nAnimNo].nFrame;
		if (m_nAnimNo >= m_nSplitX * m_nSplitY)
		{
			m_nAnimNo = 0;
		}
	}
}
void CSpriteAnimation::SetImage(std::string name)
{
	// 管理クラスからポインタを受け取る
	//if (CImageResourceManager::Get().Load(name))
	{
		m_ImageName = name;
		m_pImage = CImageResourceManager::Get().GetResource(m_ImageName);
	}
}
Vector2 CSpriteAnimation::GetUV(int no)
{
	int u = no % m_nSplitX;
	int v = no / m_nSplitY;
	return Vector2((float)u / m_nSplitX, (float)v / m_nSplitY);
}
Vector2 CSpriteAnimation::GetUV()
{
	if (m_stParam.size() <= m_nAnimNo)return Vector2(0, 0);
	return GetUV(m_stParam[m_nAnimNo].nAnimNo);
}
Vector2 CSpriteAnimation::GetFrameSize() 
{
	return Vector2(1.0f / m_nSplitX, 1.0f / m_nSplitY);
};

#ifdef BUILD_MODE

void CSpriteAnimation::ImGuiDebug()
{
	using namespace MySpace::Debug;

	ImGui::Text(u8"filaName : %s", GetImageName().c_str());
	ImGui::Text(u8"ﾃｸｽﾁｬサイズ %f %f", (float*)&GetFrameSize());
	ImGui::Text(u8"ﾃｸｽﾁｬ座標 %f %f", (float*)&GetUV());

	auto param = m_stParam;
	int size = static_cast<int>(param.size());
	// サイズ調整
	if (ImGui::InputInt(u8"size", &size) && size > 0)
	{
		/*if (param.size() - size == 1)
			param.pop_back();
		else if(param.size() - size == -1)
			param.push_back(ST_FRAME());*/
		param.resize(size);
		m_stParam = param;
	}
	ImGui::InputInt(u8"分割x", &m_nSplitX);
	ImGui::InputInt(u8"分割y", &m_nSplitY);
	for (auto & it : param)
	{
		ImGui::InputInt(u8"AnimNo", &it.nAnimNo);
		ImGui::InputInt(u8"yFrame", &it.nFrame);
	}
	
	if (!GetImage().lock())
		return;

	// ImGui.hにコンストラクタと演算子のオーバーロード追加
	auto split = GetSplit();
	ImVec2 texSize = static_cast<ImVec2>(GetFrameSize());
	for (int cnt = 0; cnt < split.x * split.y; ++cnt)
	{
		ImVec2 uv = static_cast<XMFLOAT2>(GetUV(cnt));
		ImGui::Image(
			(void*)GetImage().lock()->GetSRV(),	// 画像
			ImVec2(10, 10),									// サイズ
			uv,												// UV
			texSize											// フレームサイズ
		);
		// 段落
		if (cnt % split.x != 0)
			ImGui::SameLine();
	}	
}
#endif // BUILD_MODE