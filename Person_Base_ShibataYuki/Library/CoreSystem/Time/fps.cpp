//=========================================================
// [fps.h]
//---------------------------------------------------------
// �쐬:2022/04/21
//---------------------------------------------------------
// fps�A���Ԑ���
//=========================================================

// --- �C���N���[�h�� ---
#include <CoreSystem/Time/fps.h>
#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::System;

//==========================================================
// �R���X�g���N�^
//==========================================================
CFps::CFps()
	:m_nSlowFramPerSec(60), m_dwExecLastTime(0), m_eSlow(ESlow::SLOW_NONE), m_bUpdate(true), m_dwCurrentTime(0),
	m_dwDeltaTime(0), m_dwSlowTime(0), m_fTimeScale(1.0f),m_nHitStopFrame(0)
{
	m_FixedData.m_bUpdate = true;
	m_FixedData.m_dwFixedExecLastTime = 0;
	m_FixedData.m_dwFixedTime = 20;

#ifdef BUILD_MODE
	m_dwFPSLastTime = 0;
	m_dwFrameCount = 0;
	m_nCountFPS = 0;
	m_dwDebugSlow = 0;
	m_nDebugFPS = FPS;
#endif // DEBUG

	SetSlow(FPS);
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CFps::~CFps()
{
	
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
	srand(unsigned int(::GetTickCount64()) );
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

#if BUILD_MODE
	if ((m_dwCurrentTime - m_dwFPSLastTime) >= 500)
	{	// 0.5�b���ƂɎ��s
		m_nCountFPS = m_dwFrameCount * 1000 /(m_dwCurrentTime - m_dwFPSLastTime);
		m_dwFPSLastTime = m_dwCurrentTime;
		m_dwFrameCount = 0;
	}
	++m_dwFrameCount;
#endif // BUILD_MODE

	//--- deltaTime
	// ���ݎ��ԂƑO��̍X�V���Ԃ̍������ 1f�̎��Ԃ����߂�
	m_dwDeltaTime = (m_dwCurrentTime - m_dwExecLastTime);
	// �O������̉�����������ۂ̂��߁A��������߂�
	if (m_dwDeltaTime > 50) 
		m_dwDeltaTime = 50;

	//--- �Œ莞��
	m_FixedData.m_bUpdate = false;
	if (auto diffTime = float(m_dwCurrentTime - m_FixedData.m_dwFixedExecLastTime) * m_fTimeScale;  
		(DWORD)diffTime >= m_FixedData.m_dwFixedTime)
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
CFps* CFps::Get()
{
	static CFps pInstance;
	return &pInstance;
}


#if BUILD_MODE

void CFps::ImGuiDebug()
{
	//--- ���\��
	ImGui::Checkbox(u8"UpdateON", &m_bUpdate);
	
	Debug::SetTextAndAligned("DeltaTime:");
	ImGui::Text("%.5f", CFps::Get()->DeltaTime());

	Debug::SetTextAndAligned("UnScaleDeltaTime:");
	ImGui::Text("%.5f", CFps::Get()->UnScaleDeltaTime());

	Debug::SetTextAndAligned("TimeScale:");
	ImGui::Text("%.5f", CFps::Get()->GetTimeScale());
	
	Debug::SetTextAndAligned("Count:");
	ImGui::Text("%d", CFps::Get()->GetFPSCount());

	//--- �ݒ�
	/*if (ImGui::Button("FPS Set"))
		SetSlow(m_nDebugFPS);*/
	
	Debug::SetTextAndAligned(u8"FPS ������");
	ImGui::DragInt("##fps", &m_nDebugFPS, 1, 1, 60);

	Debug::SetTextAndAligned("TimeScale");
	if(ImGui::DragFloat("##TimeScale", &m_fTimeScale))
		SetTimeScale(m_fTimeScale);

	Debug::SetTextAndAligned("HitStop Num");
	ImGui::DragInt("##HitStopNum", &m_nHitStopFrame);
	
	Debug::SetTextAndAligned("Slow Time");
	ImGui::DragInt("##SlowTime", (int*)&m_dwDebugSlow);
	
	if (ImGui::Button("Slow OK?"))
	{
		m_dwSlowTime = m_dwDebugSlow;
	}
	ImGui::SameLine();
	if (ImGui::Button("Slow ON/OFF"))
	{
		m_eSlow = (ESlow)(m_eSlow ^ ESlow::SLOW_ON);
	}

	//--- �Œ莞��
	ImGui::Checkbox("Fixed Update", &m_FixedData.m_bUpdate);

	int newFixedTime = (int)m_FixedData.m_dwFixedTime;
	Debug::SetTextAndAligned("Fixed Time");
	if (ImGui::DragInt("##FixedTime", &newFixedTime))
	{
		m_FixedData.m_dwFixedTime = newFixedTime;
	}
	
}

#endif // BUILD_MODE
