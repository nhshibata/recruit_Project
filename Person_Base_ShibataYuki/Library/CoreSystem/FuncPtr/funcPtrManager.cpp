//==========================================================
// [funcPtrMnagaer.cpp]
// �쐬:2022/06/23
//==========================================================

//--- �C���N���[�h��
#include <CoreSystem/FuncPtr/funcPtrManager.h>
#include <DebugSystem/imguiManager.h>

using namespace MySpace::System;

void CFuncManager::Uninit()
{
	FUNC_VEC::iterator it;
	for (it = m_FuncPtr.begin(); it != m_FuncPtr.end(); ++it)
	{
		(*it).reset();
	}
	m_FuncPtr.clear();
}
//
void CFuncManager::Update()
{
	bool ret = false;
	for (FUNC_VEC::iterator it = m_FuncPtr.begin(); it != m_FuncPtr.end(); )
	{
		// �X�V
		// time or �J��Ԃ�����
		ret = (*it)->Update();

		if (ret)
		{
			(*it).reset();
			it = m_FuncPtr.erase(it);
			continue;
		}

		++it;
	}
}

// �߂�l���|�C���^�^
void CFuncManager::RequestFunc(CFuncPtr::PTR func)
{
	m_FuncPtr.push_back(std::make_shared<CFuncPtr>(func));
}

// �߂�l��void
void CFuncManager::RequestFunc(CTimeFuncPtr::PTR func, float time, bool loop)
{
	m_FuncPtr.push_back(std::make_shared<CTimeFuncPtr>(func, time, loop));
}

bool CFuncManager::StopFunc(void* ptr)
{
	FUNC_VEC::iterator it;
	for (it = m_FuncPtr.begin(); it != m_FuncPtr.end(); ++it)
	{
		// �A�h���X����v������
		if ((*it)->GetAddress() == ptr)
		{
			(*it).reset();
			m_FuncPtr.erase(it);
			return true;
		}
	}
	// �o�^���Ă��Ȃ��̂�false
	return false;
}

CFuncManager* CFuncManager::Get()
{
	static CFuncManager instance;
	return &instance;
}

#ifdef BUILD_MODE

void CFuncManager::ImGuiDebug()
{
	FUNC_VEC::iterator it;
	
	for (it = m_FuncPtr.begin(); it != m_FuncPtr.end(); ++it)
	{
		ImGui::Text(u8"*���O:%s", address2str((*it)));
		ImGui::SameLine();

	}
}

#endif // BUILD_MODE