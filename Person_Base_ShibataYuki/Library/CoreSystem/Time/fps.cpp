//=========================================================
// [fps.h]
// fps�A���Ԑ���
//----------------------------
// �쐬:2022/04/21
//=========================================================

// --- �C���N���[�h�� ---
#include <CoreSystem/Time/fps.h>
#include <ImGui/imgui.h>

//#pragma comment(lib, "winmm.lib")	// winmm.lib���g�p����

using namespace MySpace::System;

//==========================================================
// �R���X�g���N�^
//==========================================================
CFps::CFps()
	:m_nSlowFramPerSec(60), m_dwExecLastTime(0), m_eSlow(ESlow::SLOW_NONE), m_bUpdate(true), m_dwCurrentTime(0),
	m_dwDeltaTime(0), m_dwSlowTime(0), m_fTimeScale(1.0f),m_nHitStopFrame(0), m_dwDebugSlow(0), m_nDebugFPS(FPS)
{
#ifdef BUILD_MODE
	m_dwFPSLastTime = 0;
	m_dwFrameCount = 0;
	m_nCountFPS = 0;
#endif // DEBUG

	SetSlow(FPS);
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CFps::~CFps()
{
	Uninit();
}

//==========================================================
// ������
//==========================================================
void CFps::Init()
{
	// ���x���グ��
	timeBeginPeriod(1);

#if BUILD_MODE
	// ���ݎ���
	m_dwExecLastTime = m_dwFPSLastTime = timeGetTime();
	m_dwCurrentTime = m_dwFrameCount = 0;
#endif // BUILD_MODE
	srand(::GetTickCount());
}

//==========================================================
// �I������
//==========================================================
void CFps::Uninit()
{
	// ���x��߂�
	timeEndPeriod(1);
}

//==========================================================
// �X�V
//==========================================================
void CFps::Update()
{
	// �X�V�t���O������
	m_bUpdate = false;
	// ���ݎ����̎擾
	m_dwCurrentTime = timeGetTime();

#pragma region FPS
#if BUILD_MODE
	if ((m_dwCurrentTime - m_dwFPSLastTime) >= 500)
	{	// 0.5�b���ƂɎ��s
		m_nCountFPS = m_dwFrameCount * 1000 /(m_dwCurrentTime - m_dwFPSLastTime);
		m_dwFPSLastTime = m_dwCurrentTime;
		m_dwFrameCount = 0;
	}
	++m_dwFrameCount;
#endif // BUILD_MODE
#pragma endregion

	//--- deltaTime
	// ���ݎ��ԂƑO��̍X�V���Ԃ̍������ 1f�̎��Ԃ����߂�
	m_dwDeltaTime = (m_dwCurrentTime - m_dwExecLastTime);
	// �O������̉�����������ۂ̂��߁A��������߂�
	if (m_dwDeltaTime > 100) 
		m_dwDeltaTime = 100;

	//--- �Œ莞��
	m_FixedData.m_bUpdate = false;
	if ((m_dwCurrentTime - m_FixedData.m_dwFixedExecLastTime) * m_fTimeScale >= m_FixedData.m_dwFixedTime)
	{
		m_FixedData.m_dwFixedExecLastTime = m_dwCurrentTime;
		m_FixedData.m_bUpdate = true;
	}

	// �ݒ肳�ꂽ���x�ōX�V
	if (m_dwDeltaTime >= (DWORD)(1000 / m_nSlowFramPerSec))
	//if ((m_dwCurrentTime - m_dwExecLastTime) >= (DWORD)(1000 / m_nSlowFramPerSec))
	{
		// �ŏI�X�V���Ԃ��X�V
		m_dwExecLastTime = m_dwCurrentTime;
		m_bUpdate = true;

		// �X���[�Ȃ�Ή񐔂����炷
		if (m_eSlow == ESlow::SLOW_ON && m_dwSlowTime != -1)
		{
			--m_dwSlowTime;			// �񐔂����炷
			if (m_dwSlowTime <= 0)
			{
				m_dwSlowTime = -1;
				m_eSlow = ESlow::SLOW_OFF;
			}
		}

		//--- �X���[
		if (m_eSlow == ESlow::SLOW_OFF)
		{
			// ���X�ɖ߂�
			if (m_nSlowFramPerSec < FPS)
			{
				int fps = static_cast<int>(m_nSlowFramPerSec * 0.9f + FPS * 0.1f);
				m_nSlowFramPerSec = fps;
				if (FPS - fps < 0.2f)
				{
					m_eSlow = ESlow::SLOW_NONE;
					m_nSlowFramPerSec = FPS;
				}
			}
		}

		//--- �q�b�g�X�g�b�v
		// �q�b�g�X�g�b�v�񐔂�0����̎�
		if (m_nHitStopFrame > 0)
		{
			--m_nHitStopFrame;		// �q�b�g�X�g�b�v�񐔂����炷
			m_bUpdate = false;		// �X�V���Ȃ�
		}
	}
}

//==========================================================
// �X���[�J�n
//==========================================================
void CFps::StartSlow(const int nSlowfps)
{
	m_eSlow = ESlow::SLOW_ON;
	SetSlow(nSlowfps);
}

//==========================================================
// �X���[���Ԑݒ�
//==========================================================
void CFps::SetSlow(const int nSlowfps)
{
	// 60����Ȃ�C��
	if (nSlowfps > FPS)
	{
		m_nSlowFramPerSec = FPS;
		return;
	}
	else if (nSlowfps < 1)
		return;
	m_nSlowFramPerSec = nSlowfps;
}

//==========================================================
// �X���[���Ԑݒ�
//==========================================================
void CFps::SetSlow(const int nSlowfps, const int nTime)
{
	// �b�Ō��߂������߁A60��������
	m_dwSlowTime = nTime * 60;
	SetSlow(nSlowfps);
}

//==========================================================
// �V���O���g���擾
//==========================================================
CFps& CFps::Get()
{
	static CFps pInstance;
	return pInstance;
}


#if BUILD_MODE

void CFps::ImGuiDebug()
{
	//--- ���\��
	ImGui::Checkbox(u8"�X�V�t���[��", &m_bUpdate);
	
	ImGui::Text(u8"���݂�DeltaTime : %.5f", CFps::Get().DeltaTime());
	ImGui::Text(u8"���݂�UnScaleDeltaTime : %.5f", CFps::Get().UnScaleDeltaTime());
	ImGui::Text(u8"���݂�TimeScale : %.5f", CFps::Get().GetTimeScale());
	ImGui::Text(u8"���݂�Count : %d", CFps::Get().GetFPSCount());

	//--- �ݒ�
	if (ImGui::Button(u8"FPS Set"))
		SetSlow(m_nDebugFPS);
	ImGui::SameLine();
	ImGui::DragInt(u8"FPS ������", &m_nDebugFPS, 1, 1, 60);

	if(ImGui::InputFloat(u8"TimeScale", &m_fTimeScale))
		SetTimeScale(m_fTimeScale);

	ImGui::DragInt(u8"HitStop", &m_nHitStopFrame);
	
	ImGui::DragInt(u8"�X���[����", (int*)&m_dwDebugSlow);
	
	ImGui::SameLine();
	if (ImGui::Button(u8"SlowOK?"))
	{
		m_dwSlowTime = m_dwDebugSlow;
	}
	if (ImGui::Button(u8"Slow ON/OFF"))
	{
		m_eSlow = (ESlow)(m_eSlow ^ ESlow::SLOW_ON);
	}

	ImGui::Checkbox(u8"Fixed Update", &m_FixedData.m_bUpdate);
	ImGui::SameLine();
	ImGui::Text(u8"Fixed Time : %d", m_FixedData.m_dwFixedTime);

}

#endif // BUILD_MODE
