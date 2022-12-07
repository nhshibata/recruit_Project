//=========================================================
// [fadeController.h] 
// �쐬:2022/08/20
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __FADE_CONTROLLER_COMPONENT_H__
#define __FADE_CONTROLLER_COMPONENT_H__

//--- �C���N���[�h��
#include <functional>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>

class CFadeController : public CComponent
{
public:
	enum class E_FadeState
	{
		NONE,			// *@�j���[�g����
		FADE_OUT_START,	// *@�t�F�[�h�A�E�g�J�n
		FADE_OUT,		// *@�t�F�[�h�A�E�g��(�֐��|�C���^�ł͏I����)
		FADE_IN_START,	// *@�t�F�[�h�C���J�n
		FADE_IN,		// *@�t�F�[�h�C��(�֐��|�C���^�ł͏I����)
		WAIT,			// *@�҂����
		MAX,
	};
private:
	std::weak_ptr<CPolygonRenderer> m_pPolygon;										// *@�|���S��ptr
	E_FadeState m_eState;															// *@���
	float m_fFadeTime;																// *@�t�F�[�h���s����
	static inline std::map<E_FadeState, std::vector<std::function<void(void)>>> m_pFuncMap;		// *@�t�F�[�h���s���֐��|�C���^
	static inline std::map<E_FadeState, std::vector<std::function<void(void)>>> m_pLoopFuncMap;	// *@�t�F�[�h���s���֐��|�C���^(���[�v)
	
private:
	// *�o�^���ꂽ�֐��̎��s
	void Call(E_FadeState state)
	{
		if (m_pLoopFuncMap[state].size() != 0)
		{// �J��Ԃ�
			for (auto & func : m_pLoopFuncMap[state])
			{
				func();
			}
		}

		// ���
		if (m_pFuncMap.size() == 0)
			return;

		for (auto & func : m_pFuncMap[state])
		{
			func();
		}
		m_pFuncMap[state].clear();
	}
public:
	CFadeController() {};
	CFadeController(std::shared_ptr<CGameObject> owner);
	~CFadeController() {};

	void Awake();
	void Init();
	void Update();

	// *@�t�F�[�h�A�E�g�J�n
	void StartFadeOut();
	// *@�t�F�[�h�C���J�n
	void StartFadeIn();

	// *@�Ăяo���^�C�~���O�w��
	// *@������:void(class)(void)�^�֐��|�C���^, �C���X�^���X�|�C���^, (bool�^�ǉ��Ŕj������Ȃ�)
	template<class T>
	void SetTypeFunction(E_FadeState state, void (T::*func)(void), T* ptr)
	{
		m_pFuncMap[state].push_back(std::bind(func, ptr));
	}
	// *@�Ăяo���^�C�~���O�w��
	// *@������:void(class)(void)�^�֐��|�C���^, �C���X�^���X�|�C���^, (bool�^�ǉ��Ŕj������Ȃ�)
	template<class T>
	void SetTypeFunction(E_FadeState state, void (T::*func)(void), T* ptr, bool flg)
	{
		m_pLoopFuncMap[state].push_back(std::bind(func, ptr));
	}

	//--- �Q�b�^�[�E�Z�b�^�[

	// *@fade�󋵎擾
	E_FadeState GetState() { return m_eState; }
	// *@�����Ă����Ԃ��m�F
	// *@fade���Ȃ�true
	bool IsFadeProcess() { return (m_eState != E_FadeState::NONE); }
};


#endif // !__FADECONTROLLER_COMPONENT_H__
