//==========================================================
// [timeFuncPtr.cpp]
// �쐬:2022/06/23
//==========================================================

//--- �C���N���[�h��
#include <CoreSystem/FuncPtr/timeFuncPtr.h>
#include <CoreSystem/Time/fps.h>

using namespace MySpace::System;

//==========================================================
// �R���X�g���N�^
//==========================================================
CTimeFuncPtr::CTimeFuncPtr(PTR ptr, float time, bool loop)
	:/*m_Func(ptr),*/m_fTime(time), m_bLoop(loop)
{

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CTimeFuncPtr::~CTimeFuncPtr()
{
	m_Func = nullptr;
}

//==========================================================
// �X�V
//==========================================================
bool CTimeFuncPtr::Update()
{
	m_fTime -= CFps::Get().DeltaTime();
	
	// 0�ȉ�
	if (m_fTime <= 0)
	{
		m_Func();
		m_Func = nullptr;
		return true;
	}
	return false;
}