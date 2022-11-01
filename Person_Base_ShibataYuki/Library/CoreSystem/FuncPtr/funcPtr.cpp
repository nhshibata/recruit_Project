//==========================================================
// [funcPtr.cpp]
// �쐬:2022/06/17
//==========================================================

//--- �C���N���[�h��
#include <CoreSystem/FuncPtr/funcPtr.h>

using namespace MySpace::System;

//
CFuncPtr::CFuncPtr(PTR ptr)
	:m_Func(ptr)
{

}
//
CFuncPtr::~CFuncPtr()
{
	m_Func = nullptr;
}
// �X�V
bool CFuncPtr::Update()
{
	// nullptr�ȊO���Ԃ��Ă���Ί֐��I��
	if (m_Func())
	{
		m_Func = nullptr;
		return true;
	}
	return false;
}