//=========================================================
// [spriteAnimation.cpp]
// �쐬: 2022/07/04
//=========================================================

//--- �C���N���[�h��
#include <Application/Application.h>
#include <GraphicsSystem/Texture/spriteAnimation.h>
#include <GraphicsSystem/Manager/assetsManager.h>
#include <GraphicsSystem/Manager/imageResourceManager.h>

#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Graphics;

//==========================================================
// �R���X�g���N�^
//==========================================================
CSpriteAnimation::CSpriteAnimation() 
	:m_nAnimNo(0),m_nSplitX(1),m_nSplitY(1),m_nCnt(3)
{

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CSpriteAnimation::~CSpriteAnimation()
{
	//m_pImage.reset();
}

//==========================================================
// �X�V
//==========================================================
void CSpriteAnimation::Update()
{
	if (m_nSplitX == 1 && m_nSplitY == 1)
		return;

	//--- ��Ұ��ݎ��ԍX�V
	--m_nCnt;

	//--- �R�}�X�V
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

//==========================================================
// �摜�ݒ�
//==========================================================
void CSpriteAnimation::SetImage(std::string name)
{
	auto assets = Application::Get()->GetSystem<CAssetsManager>();
	if (!assets)
		return;

	// �Ǘ��N���X����|�C���^���󂯎��
	// �ǂݍ��܂�ĂȂ���Γǂ݂��܂��
	m_pImage = assets->GetImageManager()->GetResource(name);

	if(m_pImage)
	{
		m_ImageName = name;
	}

}

//==========================================================
// UV�擾
// �R�}�ԍ�����v�Z
//==========================================================
Vector2 CSpriteAnimation::GetUV(int no)
{
	//int u = no % m_nSplitX;
	//int v = no / m_nSplitY;
	//return Vector2((float)u / (float)m_nSplitX, (float)v / 1.0f / (float)m_nSplitY);
	const int u = no % m_nSplitX;
	const int v = no / m_nSplitX;
	return Vector2((float)u / (float)m_nSplitX, (float)v / (float)m_nSplitY);
}

//==========================================================
// UV�擾
//==========================================================
Vector2 CSpriteAnimation::GetUV()
{
	if (m_stParam.size() <= m_nAnimNo)
		return Vector2(0, 0);
	return GetUV(m_stParam[m_nAnimNo].nAnimNo);
}

//==========================================================
// ����������UV�T�C�Y�擾
//==========================================================
Vector2 CSpriteAnimation::GetFrameSize() 
{
	return Vector2(1.0f / m_nSplitX, 1.0f / m_nSplitY);
};


#ifdef BUILD_MODE

void CSpriteAnimation::ImGuiDebug()
{
	using namespace MySpace::Debug;

	ImVec2 texSize = (GetFrameSize().Convert<ImVec2>());

	ImGui::BeginTabBar("SpriteAnimation");
	
	if (ImGui::BeginTabItem("Texture Data"))
	{
		ImGui::Text(u8"filaName : %s", GetImageName().c_str());
		ImGui::Text(u8"ø����T�C�Y %f %f", texSize.x, texSize.y);
		ImGui::Text(u8"ø������W %f %f", GetUV().x, GetUV().y);
		ImGui::EndTabItem();
	}

	//--- �C���[�W���Ȃ��Ȃ珈�����Ȃ�
	if (!GetImage().lock())
	{
		ImGui::EndTabBar();
		return;
	}

	if (ImGui::BeginTabItem("param"))
	{
		int size = static_cast<int>(m_stParam.size());
		//--- �T�C�Y����
		Debug::SetTextAndAligned("Param Size");
		if (ImGui::InputInt("##size", &size) && size >= 0)
		{
			m_stParam.resize(size);
			m_stParam.back().nAnimNo = m_stParam[size - 1].nAnimNo + 1;
			m_stParam.back().nFrame = m_stParam[size - 1].nFrame;
		}
		Debug::SetTextAndAligned("AnimeNo:");
		ImGui::InputInt("##No", &m_nAnimNo);
		if (m_nAnimNo < 0)
			m_nAnimNo = 0;

		//--- �������ύX
		Debug::SetTextAndAligned("SplitX:");
		ImGui::InputInt("##splitX", &m_nSplitX);
		Debug::SetTextAndAligned("SplitY:");
		ImGui::InputInt("##splitY", &m_nSplitY);
		if (m_nSplitX < 0)
			m_nSplitX = 0;
		if (m_nSplitY < 0)
			m_nSplitY = 0;

		for (int i = 0; i < m_stParam.size(); i++)
		{
			std::string ii = "AnimNo" + std::to_string(i);
			ImGui::InputInt(ii.c_str(), &m_stParam[i].nAnimNo);
			ii = "Frame" + std::to_string(i);
			ImGui::InputInt(ii.c_str(), &m_stParam[i].nFrame);
		}

		//--- image�\��
		static int animationFrame = 0;
		static int animaNo = 0;

		auto split = GetSplit();
		texSize = (GetFrameSize().Convert<ImVec2>());

		if (++animationFrame % 30 == 0)
		{
			++animaNo;
			animationFrame = 0;
			if (animaNo >= split.x * split.y)
				animaNo = 0;
		}

		ImVec2 uv = GetUV(animaNo).Convert<ImVec2>();
		ImVec2 uv0 = ImVec2((uv.x) / m_nSplitX, (uv.y) / m_nSplitY);
		ImVec2 uv1 = ImVec2(uv.x + texSize.x, uv.y + texSize.y);
		ImGui::Text("uv%f%f", uv1.x, uv1.y);
		ImGui::Image(
			(void*)GetImage().lock()->GetSRV(),	// �摜
			ImVec2(80, 80),						// �T�C�Y
			uv,									// UV
			uv1//texSize						// �t���[���T�C�Y
		);

		//--- UV�\��
		for (int cnt = 0; cnt < split.x * split.y; ++cnt)
		{
			uv = GetUV(cnt).Convert<ImVec2>();
			ImGui::Image(
				(void*)GetImage().lock()->GetSRV(),	// �摜
				ImVec2(80, 80),						// �T�C�Y
				uv,									// UV
				texSize								// �t���[���T�C�Y
			);

			// �i��
			if ((cnt + 1) % 3 == 0)
				ImGui::SameLine();
		}
		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();

}

#endif // BUILD_MODE