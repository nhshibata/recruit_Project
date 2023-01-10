//==========================================================
// [timeFuncPtr.cpp]
// 作成:2022/06/23
//==========================================================

//--- インクルード部
#include <CoreSystem/FuncPtr/timeFuncPtr.h>
#include <CoreSystem/Time/fps.h>

using namespace MySpace::System;

//==========================================================
// コンストラクタ
//==========================================================
CTimeFuncPtr::CTimeFuncPtr(PTR ptr, float time, bool loop)
	:/*m_Func(ptr),*/m_fTime(time), m_bLoop(loop)
{

}

//==========================================================
// デストラクタ
//==========================================================
CTimeFuncPtr::~CTimeFuncPtr()
{
	m_Func = nullptr;
}

//==========================================================
// 更新
//==========================================================
bool CTimeFuncPtr::Update()
{
	m_fTime -= CFps::Get().DeltaTime();
	
	// 0以下
	if (m_fTime <= 0)
	{
		m_Func();
		m_Func = nullptr;
		return true;
	}
	return false;
}