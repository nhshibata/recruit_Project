//==========================================================
// [funcPtr.cpp]
// 作成:2022/06/17
//==========================================================

//--- インクルード部
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
// 更新
bool CFuncPtr::Update()
{
	// nullptr以外が返ってくれば関数終了
	if (m_Func())
	{
		m_Func = nullptr;
		return true;
	}
	return false;
}