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
	//int u = no % m_nSplitX;
	//int v = no / m_nSplitY;
	//return Vector2((float)u / (float)m_nSplitX, (float)v / 1.0f / (float)m_nSplitY);
	int u = no % m_nSplitX;
	int v = no / m_nSplitX;
	return Vector2((float)u / (float)m_nSplitX, (float)v / (float)m_nSplitY);
}
Vector2 CSpriteAnimation::GetUV()
{
	if (m_stParam.size() <= m_nAnimNo)
		return Vector2(0, 0);
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

	ImVec2 texSize = (GetFrameSize().Convert<ImVec2>());

	ImGui::Text(u8"filaName : %s", GetImageName().c_str());
	ImGui::Text(u8"ﾃｸｽﾁｬサイズ %f %f", texSize.x, texSize.y);
	ImGui::Text(u8"ﾃｸｽﾁｬ座標 %f %f", GetUV().x, GetUV().y);

	int size = static_cast<int>(m_stParam.size());
	// サイズ調整
	if (ImGui::InputInt(u8"size", &size) && size >= 0)
	{
		/*if (param.size() - size == 1)
			param.pop_back();
		else if(param.size() - size == -1)
			param.push_back(ST_FRAME());*/
		m_stParam.resize(size);
		m_stParam.back().nAnimNo = m_stParam[size - 1].nAnimNo+1;
		m_stParam.back().nFrame = m_stParam[size - 1].nFrame;
	}
	ImGui::InputInt(u8"No", &m_nAnimNo);
	if (m_nAnimNo < 0)
		m_nAnimNo = 0;

	ImGui::InputInt(u8"分割x", &m_nSplitX);
	ImGui::InputInt(u8"分割y", &m_nSplitY);
	if (m_nSplitX < 0)
		m_nSplitX = 0;
	if (m_nSplitY < 0)
		m_nSplitY = 0;

	for (int i = 0; i < m_stParam.size(); i++)
	{
		std::string ii = u8"AnimNo" + std::to_string(i);
		ImGui::InputInt(ii.c_str(), &m_stParam[i].nAnimNo);	
		ii = u8"Frame" + std::to_string(i);
		ImGui::InputInt(ii.c_str(), &m_stParam[i].nFrame);
	}

	if (!GetImage().lock())
		return;

	// ImGui.hにコンストラクタと演算子のオーバーロード追加
	static int animationFrame = 0;
	static int animaNo = 0;
	auto split = GetSplit();
	texSize = (GetFrameSize().Convert<ImVec2>());

	if (++animationFrame % 30 == 0)
	{
		++animaNo;
		animationFrame = 0;
		if(animaNo >= split.x * split.y)
			animaNo = 0;
	}
	static float region_sz = 0.0f;
	ImGui::InputFloat("aaa", &region_sz);
	ImVec2 uv = GetUV(animaNo).Convert<ImVec2>();
	ImVec2 uv0 = ImVec2((uv.x) / m_nSplitX, (uv.y) / m_nSplitY);
	ImVec2 uv1 = ImVec2(uv.x, uv.y);
	ImGui::Text("da%f%f", uv1.x, uv1.y);
	ImGui::Image(
		(void*)GetImage().lock()->GetSRV(),	// 画像
		ImVec2(80, 80),									// サイズ
		uv,												// UV
		uv1//texSize											// フレームサイズ
	);

	// UV表示
	for (int cnt = 0; cnt < split.x * split.y; ++cnt)
	{
		uv = GetUV(cnt).Convert<ImVec2>();
		ImGui::Image(
			(void*)GetImage().lock()->GetSRV(),	// 画像
			ImVec2(80, 80),									// サイズ
			uv,												// UV
			texSize											// フレームサイズ
		);
		// 段落
		//if (cnt % split.x != 0)
		if ((cnt+1) % 3 == 0)
			ImGui::SameLine();
	}

}
#endif // BUILD_MODE