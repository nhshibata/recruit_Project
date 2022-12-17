//==========================================================
// [sceneTransitionDetection.h]
// �쐬:2022/08/20
// �X�V:2022/08/25 �ēx�ǉ������֐������邽�߁A�ϐ��ɑ�����Ă��珈�����s��
//----------------------------------------------------------
// �V�[������Ăяo�������֐��|�C���^��o�^����׽
//==========================================================

//--- �C���N���[�h��
#include <GameSystem/Scene/sceneTransitionDetection.h>

using namespace MySpace::SceneManager;


CSceneTransitionDetection::CSceneTransitionDetection()
{
	m_pChangeFunc.clear();
	m_pLoadFunc.clear();
	m_pUnloadFunc.clear();
}
CSceneTransitionDetection::~CSceneTransitionDetection()
{
	m_pChangeFunc.clear();
	m_pLoadFunc.clear();
	m_pUnloadFunc.clear();
}
// *Change
void CSceneTransitionDetection::Call(CScene* prev, CScene* next)
{
	auto list = m_pChangeFunc;
	m_pChangeFunc.clear();

	for (auto it = list.begin(); it != list.end(); ++it)
	{
		(*it).Call(prev,next);
	}
}
// *Load
void CSceneTransitionDetection::Call(CScene* scene, int mode)
{
	auto list = m_pLoadFunc;
	m_pLoadFunc.clear();
	if (list.size() == 0)
		return;

	for (auto it = list.begin(); it != list.end(); ++it)
	{
		(*it).Call(scene, mode);
	}
}
// *Unload
void CSceneTransitionDetection::Call(CScene* scene)
{
	auto list = m_pUnloadFunc;
	m_pUnloadFunc.clear();

	for (auto it = list.begin(); it != list.end(); ++it)
	{
		(*it).Call(scene);
	}
}
bool CSceneTransitionDetection::StopFunc(void* ptr)
{
	// ����
	//if (auto it = std::find(m_pChangeFunc.begin(), m_pChangeFunc.end(),ptr); it != m_pChangeFunc.end())
	for (auto it = m_pChangeFunc.begin(); it != m_pChangeFunc.end(); ++it)
	{
		if (ptr == it->GetAdress()) 
		{
			m_pChangeFunc.erase(it);
			return true;
		}
	}
	//if (auto it = std::find(m_pLoadFunc.begin(), m_pLoadFunc.end(),ptr); it != m_pLoadFunc.end())
	for (auto it = m_pLoadFunc.begin(); it != m_pLoadFunc.end(); ++it)
	{
		if (ptr == it->GetAdress())
		{
			m_pLoadFunc.erase(it);
			return true;
		}
	}
	//if (auto it = std::find(m_pUnloadFunc.begin(), m_pUnloadFunc.end(), ptr); it != m_pUnloadFunc.end())
	for (auto it = m_pUnloadFunc.begin(); it != m_pUnloadFunc.end(); ++it)
	{
		if (ptr == it->GetAdress())
		{
			m_pUnloadFunc.erase(it);
			return true;
		}
	}
	
	// ������Ȃ�����
	return false;
}