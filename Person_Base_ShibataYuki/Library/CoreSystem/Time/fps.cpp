

// --- �C���N���[�h�� ---
#include <CoreSystem/Time/fps.h>

#pragma comment(lib, "winmm.lib")	// winmm.lib���g�p����
// --- �萔��` ---

using namespace MySpace::System;

// �R���X�g���N�^
CFps::CFps()
	:m_nSlowFramPerSec(60), m_dwExecLastTime(m_dwLastTime), m_bSlow(false), m_bUpdate(true)
{
}
// �����t���R���X�g���N�^
CFps::CFps(int nSlowfps)
	: m_nSlowFramPerSec(60), m_dwExecLastTime(m_dwLastTime), m_bSlow(false), m_bUpdate(true)
{
	SetSlow(nSlowfps);
}
// �f�X�g���N�^
CFps::~CFps()
{
	Uninit();
}
void CFps::Init()
{
	// ���x���グ��
	timeBeginPeriod(1);

	// ���ݎ���

}
void CFps::Uninit()
{
	// ���x��߂�
	timeEndPeriod(1);
}
// �X�V
void CFps::Update()
{
	// ���ݎ����̎擾
	m_dwCurrentTime = timeGetTime();
	// ���ݎ��ԂƑO��̍X�V���Ԃ̍������ 1f�̎��Ԃ����߂�
	m_dwDeltaTime = (m_dwCurrentTime - m_dwExecLastTime);
	if (m_dwDeltaTime > 100) {
		m_dwDeltaTime = 100;
	}
	// �t���O�������Ă��Ȃ���ΐݒ肳�ꂽ�X���[���Ԃ�n��
	if (!m_bSlow)
	{
		if (m_nSlowFramPerSec < FPS)
		{
			m_nSlowFramPerSec = (int)(m_nSlowFramPerSec * 0.9f + FPS * 0.1f);
		}
	}
	// �q�b�g�X�g�b�v�񐔂�0����̎�
	if (m_nHitStopFrame > 0)
	{
		--m_nHitStopFrame;		// �q�b�g�X�g�b�v�񐔂����炷
		m_bUpdate = false;		// �X�V���Ȃ������Ԃ�
		return;
	}

	// �ݒ肳�ꂽ���x�ōX�V
	if ((m_dwCurrentTime - m_dwExecLastTime) >= (DWORD)(1000 / m_nSlowFramPerSec))
	{
		// �X���[�Ȃ�Ή񐔂����炷
		if (m_bSlow)
		{
			--m_dwSlowTime;			// �񐔂����炷
			if (m_dwSlowTime <= 0)
			{
				m_dwSlowTime = 0;
				m_bSlow = false;
			}
		}
		

		// �ŏI�X�V���Ԃ��X�V
		m_dwExecLastTime = m_dwCurrentTime;
		m_bUpdate = true;
		return;
	}

	// �X�V���Ȃ�
	m_bUpdate = false;
}
// �X�V���Ă��������m�F
bool CFps::IsUpdate()
{
	return m_bUpdate;
}
// �X���[�J�n
void CFps::StartSlow(const int nSlowfps)
{
	m_bSlow = true;
	SetSlow(nSlowfps);
}
// �X���[�I��
void CFps::SlowEnd()
{
	m_bSlow = false;
	// ���C���̃t���[���ƍ��킹��
	m_dwExecLastTime = m_dwLastTime;
}
// �X���[���Ԑݒ�
void CFps::SetSlow(const int nSlowfps)
{
	// 60����Ȃ�C��
	if (nSlowfps > FPS)		
	{
		m_nSlowFramPerSec = FPS;
		return;
	}
	m_nSlowFramPerSec = nSlowfps;
}
// �X���[���Ԑݒ�
void CFps::SetSlow(const int nSlowfps, const int nTime)
{
	// �b�Ō��߂������߁A60��������
	m_dwSlowTime = nTime * 60;

	// 60����Ȃ�C��
	if (nSlowfps > FPS)		
	{
		m_nSlowFramPerSec = FPS;
		return;
	}
	m_nSlowFramPerSec = nSlowfps;
}