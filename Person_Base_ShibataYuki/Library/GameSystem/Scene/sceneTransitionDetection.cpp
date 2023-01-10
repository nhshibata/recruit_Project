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

//==========================================================
// �R���X�g���N�^
//==========================================================
CSceneTransitionDetection::CSceneTransitionDetection()
{
	m_pChangeFunc.clear();
	m_pLoadFunc.clear();
	m_pUnloadFunc.clear();
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CSceneTransitionDetection::~CSceneTransitionDetection()
{
	m_pChangeFunc.clear();
	m_pLoadFunc.clear();
	m_pUnloadFunc.clear();
}

//==========================================================
// Change���Ăяo��
//==========================================================
void CSceneTransitionDetection::Call(CScene* prev, CScene* next)
{
	auto list = m_pChangeFunc;
	m_pChangeFunc.clear();

	for (auto it = list.begin(); it != list.end(); ++it)
	{
		(*it).Call(prev,next);
	}
}

//==========================================================
// Load���Ăяo��
//==========================================================
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

//==========================================================
// Unload���Ăяo��
//==========================================================
void CSceneTransitionDetection::Call(CScene* scene)
{
	auto list = m_pUnloadFunc;
	m_pUnloadFunc.clear();

	for (auto it = list.begin(); it != list.end(); ++it)
	{
		(*it).Call(scene);
	}
}

//==========================================================
// ���O
//==========================================================
bool CSceneTransitionDetection::StopFunc(void* ptr)
{
	//--- ����
	for (auto it = m_pChangeFunc.begin(); it != m_pChangeFunc.end(); ++it)
	{
		if (ptr == it->GetAdress()) 
		{
			m_pChangeFunc.erase(it);
			return true;
		}
	}

	for (auto it = m_pLoadFunc.begin(); it != m_pLoadFunc.end(); ++it)
	{
		if (ptr == it->GetAdress())
		{
			m_pLoadFunc.erase(it);
			return true;
		}
	}

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